//
// Created by thornhill on 21/05/25.
//

#include <project.hpp>

#include "controllers.hpp"
#include "debug.hpp"


static uint16_t g_project_id = 0;

void ws::create::project(client_t &client, req_t &req) {
    uint16_t project_id = g_project_id++;

    client->data->projects[project_id] = std::make_shared<Project>();

    client->socket << (status(STATUS_OK, req) << project_id);
}

void ws::create::layer(client_t &client, req_t &req) {
    uint16_t project_id;
    int8_t layer_type;

    req->data >> project_id >> layer_type;

    auto project = client->data->projects[project_id];

    auto test = static_cast<LayerType>(layer_type);

    qDebug() << DISP(layer_type) << DISP(layerTypeToString(test));

    project->createLayer(static_cast<LayerType>(layer_type));
    uint16_t layer_id = project->getLayers().size();

    client->socket << (status(STATUS_OK, req) << layer_id);
}

