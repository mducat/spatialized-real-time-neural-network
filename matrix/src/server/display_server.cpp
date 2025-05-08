//
// Created by thornhill on 02/05/25.
//

#include <ranges>

#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>

#include "display_server.hpp"
#include "protocol.hpp"

#include <ByteObject.hpp>
#include <random.hpp>

#include "ByteObject.tcc"

DisplayServer::DisplayServer() :
    _server(new QWebSocketServer(QStringLiteral("Display Server"),
        QWebSocketServer::NonSecureMode, this))
{}

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
}

void DisplayServer::disconnected() {
    const auto socket = qobject_cast<QWebSocket *>(sender());

    qDebug() << "new client disconnected" << socket->peerAddress();

    _clients.removeOne(socket);
}

void operator<<(QWebSocket * const socket, ByteObject const &obj) {
    QByteArray resp_bin;
    resp_bin.append(reinterpret_cast<const char *>(obj.dumpForNetwork().data()), obj._size);

    socket->sendBinaryMessage(resp_bin);
}

void DisplayServer::message(QByteArray data) {
    auto socket = qobject_cast<QWebSocket *>(sender());

    ByteObject obj(reinterpret_cast<uint8_t const *>(data.constData()), data.size());

    try {
        process(socket, obj);
    } catch (const std::exception &e) {
        qCritical() << "Exception: " << e.what();
        socket << (status(STATUS_INTERNAL_PANIC) << e.what());
    }

    qDebug() << "new client message" << socket->peerAddress() << obj._size;
}

void DisplayServer::process(QWebSocket *socket, ByteObject &obj) {
    uint16_t magic;
    obj >> magic;

    switch (INSTRUCTION(magic)) {
        case CREATE: create(socket, obj);
            break;
        case READ: read(socket, obj);
            break;
        case UPDATE: update(socket, obj);
            break;
        case DELETE: del(socket, obj);
            break;
        case COMMAND: command(socket, obj);
            break;
        default: socket << status(STATUS_INTERNAL_ERROR);
    }
}

void DisplayServer::create(QWebSocket *socket, ByteObject &obj) {
    uint16_t magic;
    obj >> magic;

    static uint16_t g_project_id = 0;
    uint16_t project_id;
    uint16_t layer_id;
    int8_t layer_type;

    std::shared_ptr<Layer> layer;
    std::shared_ptr<Object> instance;

    switch (INSTANCE(magic)) {
        case INSTANCE_PROJECT:

            project_id = g_project_id++;

            this->_projects[project_id] = std::make_shared<Project>();

            socket << (status(STATUS_OK) << project_id);
        break;
        case INSTANCE_LAYER:
            obj >> project_id >> layer_type;

            this->_projects[project_id]->createLayer(static_cast<LayerType>(layer_type));
            layer_id = this->_projects[project_id]->getLayers().size();

            socket << (status(STATUS_OK) << layer_id);
            break;
        case INSTANCE_OBJECT:
            obj >> project_id >> layer_id;

            layer = this->_projects[project_id]->getLayers().at(layer_id);

            instance = layer->create<Random>();

            socket << (status(STATUS_OK) << instance->getObjectId() << instance->getObjectName());

            break;
        default: socket << status(STATUS_INTERNAL_ERROR);
    }

}

void DisplayServer::read(QWebSocket *socket, ByteObject &obj) {
    uint16_t magic;
    obj >> magic;

    uint16_t project_id;
    auto ks = std::views::keys(this->_projects);
    std::vector<int> keys{ ks.begin(), ks.end() };

    auto resp = status(STATUS_OK);
    auto layers = this->_projects[project_id]->getLayers();

    switch (INSTANCE(magic)) {
        case INSTANCE_PROJECT:

            socket << (status(STATUS_OK) << keys);
            break;
        case INSTANCE_LAYER:
            obj >> project_id;

            for (uint16_t i = 0; i < layers.size(); i ++) {
                resp << i << layers[i]->name();
            }
            socket << resp;
            break;
        default: socket << status(STATUS_INTERNAL_ERROR);
    }
}

void DisplayServer::update(QWebSocket *, ByteObject &) {
}

void DisplayServer::del(QWebSocket *, ByteObject &) {
}

void DisplayServer::command(QWebSocket *socket, ByteObject &obj) {
    uint16_t magic;
    obj >> magic;
    auto resp = status(STATUS_OK);

    switch (CMD(magic)) {
        case CMD_STATUS:
            resp << PROTOCOL_VERSION_MAJOR << PROTOCOL_VERSION_MINOR << PROTOCOL_VERSION_PATCHLEVEL;

            (void) (socket << resp); // wtf
            break;
        case CMD_META:
            cmd_meta(socket, obj);
            break;
        default: socket << status(STATUS_INTERNAL_ERROR);
    }
}

void DisplayServer::cmd_meta(QWebSocket *socket, ByteObject &obj) {
    auto resp = status(STATUS_OK);
    uint8_t cmd;
    obj >> cmd;

    if (cmd & META_LIST_LAYERS_TYPE) {
        int8_t l_start = static_cast<int8_t>(LayerType::_L_START);
        int8_t l_end = static_cast<int8_t>(LayerType::_L_START);

        for (int8_t i = l_start; i < l_end; i++) {
            resp << i << layerTypeToString(static_cast<LayerType>(i));
        }
    }

    if (cmd & META_LIST_NODES_TYPE) {
        resp << 0 << "sin" << 1 << "random" << 2 << "integrate_fire";
    }
}

ByteObject status(const uint8_t status) {
    ByteObject resp;

    resp << status;

    return resp;
}




