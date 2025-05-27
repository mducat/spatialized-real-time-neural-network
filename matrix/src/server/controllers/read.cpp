//
// Created by thornhill on 22/05/25.
//

#include <QWebSocket>

#include <dynamic_data.hpp>
#include <network_object.hpp>
#include <object.hpp>
#include <project.hpp>
#include <ranges>

#include "controllers.hpp"
#include "debug.hpp"
#include "display_server.hpp"

void ws::read::project(client_t &client, req_t &req) {
    auto ks = std::views::keys(client->data->projects);
    const std::vector<uint16_t> keys{ ks.begin(), ks.end() };

    client->socket << (status(STATUS_OK, req) << keys);
}



void ws::read::layer(client_t &client, req_t &req) {
    uint16_t project_id;

    req->data >> project_id;
    auto const project = client->data->projects[project_id];
    auto const layers = project->getLayers();

    auto resp = status(STATUS_OK, req);

    resp << static_cast<uint32_t>(layers.size());

    for (uint16_t i = 0; i < layers.size(); i++) {
        resp << static_cast<uint16_t>(layers[i]->getLayerId()) << layers[i]->name();
    }

    client->socket << resp;
}



#define UINT32_T(x) static_cast<uint32_t>(x)

void ws::read::graph(client_t &client, req_t &req) {
    uint16_t project_id;
    uint16_t layer_id;

    req->data >> project_id >> layer_id;
    auto const project = client->data->projects[project_id];
    auto const layer = project->getLayers().at(layer_id);

    auto resp = status(STATUS_OK, req);

    auto const objects = layer->getObjects();

    resp << UINT32_T(objects.size());

    for (auto [id, object] : objects) {
        resp << UINT32_T(id) << object->getObjectName();
    }

    resp << UINT32_T(objects.size());

    for (auto [id, object] : objects) {
        auto const self_id = UINT32_T(id);

        resp << UINT32_T(object->getInputs().size());

        for (auto input : object->getInputs()) {
            resp << UINT32_T(input->getObjectId()) << self_id;
        }
    }

    client->socket << resp;
}



void ws::read::node(client_t &client, req_t &req) {
    uint16_t project_id;
    uint16_t layer_id;
    uint16_t node_id;

    req->data >> project_id >> layer_id >> node_id;

    auto const project = client->data->projects[project_id];
    auto const layer = project->getLayers().at(layer_id);
    auto const node = layer->getObjectById(node_id);

    auto network_node = std::static_pointer_cast<NetworkObject>(node);

    auto value_recorder = std::make_shared<DynamicDataSource>([network_node] {
        return network_node->value();
    });

    std::shared_ptr<Request> request = req;
    auto addr = client->socket->peerAddress().toString();

    uint32_t handle = project->addCallback([value_recorder, &client, &req, addr] {
        static int counter = 0;

        counter++;

        // with msec = 10, 100 * 10ms = 1s
        if (counter % 100 == 0) {
            auto resp = status(STATUS_OK, req);

            resp << value_recorder->getValues();
            if (const auto socket = client->parent->findSocket(addr)) {
                (void) (socket << resp);
            }
        }
    });

    client->data->values[project_id][handle] = value_recorder;

    auto resp = status(STATUS_OK, req) << handle;

    client->socket << resp;
}



void ws::read::end_node(client_t &client, req_t &req) {
    uint16_t project_id;
    uint32_t handle_id;

    req->data >> project_id >> handle_id;

    auto const project = client->data->projects[project_id];
    project->removeCallback(handle_id);
    client->data->values[project_id][handle_id] = nullptr;
}
