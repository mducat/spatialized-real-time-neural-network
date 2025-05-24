//
// Created by thornhill on 02/05/25.
//

#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>

#include "display_server.hpp"
#include "protocol.hpp"

#include <ByteObject.hpp>

#include "blueprint.hpp"
#include "ByteObject.tcc"
#include "controllers/controllers.hpp"

DisplayServer::DisplayServer() :
    _server(new QWebSocketServer(QStringLiteral("Display Server"),
        QWebSocketServer::NonSecureMode, this)),
    _bp(std::make_unique<Blueprint>()) {
    this->init_blueprint();
}

DisplayServer::~DisplayServer() {
    _server->close();
    qDeleteAll(_clients);
}

void DisplayServer::start(uint16_t port) {
    if (_server->listen(QHostAddress::LocalHost, port)) {
        qInfo() << "Server listening";

        connect(_server, &QWebSocketServer::newConnection, this, &DisplayServer::connected);
    }
}

void DisplayServer::connected() {
    QWebSocket *socket = _server->nextPendingConnection();

    connect(socket, &QWebSocket::binaryMessageReceived, this, &DisplayServer::message);
    connect(socket, &QWebSocket::disconnected, this, &DisplayServer::disconnected);

    auto const addr = socket->peerAddress().toString();

    qDebug() << "new client connected" << addr;

    _clients << socket;
    if (!_clients_data.contains(addr)) {
        _clients_data[addr] = std::make_shared<ClientData>();
    }
}

void DisplayServer::disconnected() {
    const auto socket = qobject_cast<QWebSocket *>(sender());

    qDebug() << "new client disconnected" << socket->peerAddress();

    auto const addr = socket->peerAddress().toString();

    _clients.removeOne(socket);
    // _clients_data[addr] = nullptr;
}

void DisplayServer::message(QByteArray data) {
    auto socket = qobject_cast<QWebSocket *>(sender());
    auto const addr = socket->peerAddress().toString();

    ByteObject obj(reinterpret_cast<uint8_t const *>(data.constData()), data.size());
    qDebug() << "new client message" << addr << obj._size;

    uint64_t request_id;
    obj >> request_id;

    auto request = std::make_shared<Request>(obj, request_id);
    auto client = std::make_shared<WSClient>(socket, _clients_data[addr]);

    try {
        // @TODO safe byteobject (throw on out of range)
        // @TODO whitelist
        this->_bp->process(client, request);
    } catch (const std::exception &e) {
        qCritical() << "Exception: " << e.what();
        socket << (status(STATUS_INTERNAL_PANIC, request) << e.what());
    }
}

void DisplayServer::init_blueprint() const {
    this->_bp->declare_registry();

    this->_bp->add_method("ping", [](client_t &c, req_t &req) {
        c->socket << status(STATUS_OK, req);
    });

    const auto create = this->_bp->add_controller("create");
    create->add_method("project", ws::create::project);
    create->add_method("layer", ws::create::layer);

    const auto read = this->_bp->add_controller("read");
    read->add_method("project", ws::read::project);
    read->add_method("layer", ws::read::layer);

    const auto command = this->_bp->add_controller("command");
    this->_bp->add_alias("cmd", "command");

    command->add_method("status", ws::command::status);

    const auto cmd_meta = command->add_controller("meta");
    cmd_meta->add_method("types", ws::command::meta::types);
}


