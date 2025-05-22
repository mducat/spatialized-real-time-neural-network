//
// Created by thornhill on 02/05/25.
//

#include <ranges>

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

    const auto command = this->_bp->add_controller("command");
    this->_bp->add_alias("cmd", "command");

    command->add_method("status", ws::command::status);

    const auto cmd_meta = command->add_controller("meta");
    cmd_meta->add_method("types", ws::command::meta::types);
}

/*
#define T_INSTRUCTION(x) switch (x)
#define CASE(x, y) case x: y; break;

std::unordered_map<uint64_t, void (DisplayServer::*)(QWebSocket *, ReqPtr &)> methods = {
    {CREATE, &DisplayServer::create},
    {READ, &DisplayServer::read},
    {UPDATE, &DisplayServer::update},
    {DELETE, &DisplayServer::del},
    {COMMAND, &DisplayServer::command},
};*/

/*
 * std::shared_ptr<Blueprint> bp = std::make_shared<Blueprint>();
 *
 * auto create = bp->add_controller("create");
 *
 * create->add_method("project", &DisplayServer::create_project);
 * create->add_method("project", &ws::create::project);
 *
 * auto params = std::vector{PARAM(uint16_t, layer_type)};
 *
 * create->add_method("layer", [&] (socket, obj){
 *      uint16_t layer_type;
 *      obj >> layer_type;
 *      this->_project->createLayer(layer_type);
 * });
 *
 */
/*
#define TEST(x, y) x y;

void DisplayServer::process(QWebSocket *socket, ReqPtr &obj) {
    uint8_t magic;
    obj->data >> magic;

    TEST(uint16_t, layer_type)

    layer_type = magic;
    layer_type ++;

    // @TODO impl whitelist

    T_INSTRUCTION(magic) {
        CASE(CREATE, create(socket, obj))
        CASE(READ, read(socket, obj))
        CASE(UPDATE, update(socket, obj))
        CASE(DELETE, del(socket, obj))
        CASE(COMMAND, command(socket, obj))
        default:
            qCritical() << "Unknown instruction";
        socket << status(STATUS_INTERNAL_ERROR, obj);
    }

    /*
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
        default:
            qCritical() << "Unknown instruction";
        socket << status(STATUS_INTERNAL_ERROR, obj);
    }*//*
}

// std::unordered_map<std::string, >

void DisplayServer::create(QWebSocket *socket, ReqPtr &obj) {
    uint8_t magic;
    obj->data >> magic;

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

            socket << (status(STATUS_OK, obj) << project_id);
        break;
        case INSTANCE_LAYER:
            obj->data >> project_id >> layer_type;

            this->_projects[project_id]->createLayer(static_cast<LayerType>(layer_type));
            layer_id = this->_projects[project_id]->getLayers().size();

            socket << (status(STATUS_OK, obj) << layer_id);
            break;
        case INSTANCE_OBJECT:
            obj->data >> project_id >> layer_id;

            layer = this->_projects[project_id]->getLayers().at(layer_id);

            instance = layer->create<Random>();

            socket << (status(STATUS_OK, obj) << instance->getObjectId() << instance->getObjectName());

            break;
        default:
            qCritical() << "Unknown instance";
            socket << status(STATUS_INTERNAL_ERROR, obj);
    }
}

void DisplayServer::read(QWebSocket *socket, ReqPtr &obj) {
    uint8_t magic;
    obj->data >> magic;

    uint16_t project_id;
    auto ks = std::views::keys(this->_projects);
    std::vector<uint16_t> keys{ ks.begin(), ks.end() };

    auto resp = status(STATUS_OK, obj);
    std::vector<std::shared_ptr<Layer>> layers;;

    switch (INSTANCE(magic)) {
        case INSTANCE_PROJECT:

            socket << (status(STATUS_OK, obj) << keys);
            break;
        case INSTANCE_LAYER:
            obj->data >> project_id;

            layers = this->_projects[project_id]->getLayers();

            for (uint16_t i = 0; i < layers.size(); i ++) {
                resp << i << layers[i]->name();
            }
            (void) (socket << resp);
            break;
        default:
            qCritical() << "Unknown instance (read)";
            socket << status(STATUS_INTERNAL_ERROR, obj);
    }
}

void DisplayServer::update(QWebSocket *, ReqPtr &) {
}

void DisplayServer::del(QWebSocket *, ReqPtr &) {
}

void DisplayServer::command(QWebSocket *socket, ReqPtr &obj) {
    uint16_t magic;
    obj->data >> magic;
    auto resp = status(STATUS_OK, obj);

    switch (CMD(magic)) {
        case CMD_STATUS:
            resp << PROTOCOL_VERSION_MAJOR << PROTOCOL_VERSION_MINOR << PROTOCOL_VERSION_PATCHLEVEL;

            (void) (socket << resp); // wtf
            break;
        case CMD_META:
            cmd_meta(socket, obj);
            break;
        default:
            qCritical() << "Unknown command" << CMD(magic);
            socket << status(STATUS_INTERNAL_ERROR, obj);
    }
}

void DisplayServer::cmd_meta(QWebSocket *socket, ReqPtr &obj) {
    auto resp = status(STATUS_OK, obj);
    uint8_t cmd;
    obj->data >> cmd;

    if (cmd & META_LIST_LAYERS_TYPE) {
        int8_t l_start = static_cast<int8_t>(LayerType::_L_START);
        int8_t l_end = static_cast<int8_t>(LayerType::_L_END);

        resp << (l_end - l_start - 2);

        for (int8_t i = l_start + 2; i < l_end; i++) {
            resp << i << layerTypeToString(static_cast<LayerType>(i));
        }
    }

    if (cmd & META_LIST_NODES_TYPE) {
        std::vector<std::string> types = {"sin", "random", "integrate_fire"};
        resp << types.size();
        for (uint32_t i = 0; i < types.size(); i++) {
            resp << i << types[i];
        }
    }

    (void) (socket << resp);
}*/




