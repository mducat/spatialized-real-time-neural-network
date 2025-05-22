//
// Created by thornhill on 02/05/25.
//

#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>

#include "display_server.hpp"
#include "protocol.hpp"

#include <ByteObject.hpp>
// #include <random.hpp>

#include <qaccessible_base.h>

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

    qDebug() << "new client connected" << socket->peerAddress().toString();

    _clients << socket;
    _clients_data[socket] = std::make_shared<ClientData>();
}

void DisplayServer::disconnected() {
    const auto socket = qobject_cast<QWebSocket *>(sender());

    qDebug() << "new client disconnected" << socket->peerAddress();

    _clients.removeOne(socket);
    _clients_data[socket] = nullptr;
}

void DisplayServer::message(QByteArray data) {
    auto socket = qobject_cast<QWebSocket *>(sender());

    ByteObject obj(reinterpret_cast<uint8_t const *>(data.constData()), data.size());
    qDebug() << "new client message" << socket->peerAddress() << obj._size;

    uint64_t request_id;
    obj >> request_id;

    auto request = std::make_shared<Request>(obj, request_id);
    auto client = std::make_shared<WSClient>(socket, _clients_data[socket]);

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

    /*cmd_meta->add_method("test", [](client_t &c, req_t &req) {
        c->socket << status(STATUS_OK, req);
    });*/
}


