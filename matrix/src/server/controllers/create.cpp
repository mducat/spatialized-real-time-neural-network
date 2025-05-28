//
// Created by thornhill on 21/05/25.
//

#include <ranges>

#include <dynamic_data.hpp>
#include <integrate_fire.hpp>
#include <project.hpp>
#include <qtimer.h>
#include <sin.hpp>

#include "controllers.hpp"
#include "debug.hpp"


static uint16_t g_project_id = 0;

void ws::create::project(client_t &client, req_t &req) {
    uint16_t project_id = g_project_id++;

    auto project = std::make_shared<Project>();
    client->data->projects[project_id] = project;

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [project] {
        project->step();
    });

    client->data->threads[project_id] = timer;
    client->data->values[project_id] = {};

    project->addCallback([client, project_id] {
        for (auto value : std::views::values(client->data->values[project_id])) {
            value->recordValue();
        }
    });

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



void ws::create::demo(client_t &client, req_t &req) {
    uint16_t project_id;
    uint16_t layer_id;

    req->data >> project_id >> layer_id;
    auto const project = client->data->projects[project_id];
    auto const layer = project->getLayer(layer_id);

    auto resp = status(STATUS_OK, req);

    std::vector<std::shared_ptr<Object>> layer1;
    std::vector<std::shared_ptr<Object>> layer2;
    std::vector<std::shared_ptr<Object>> layer3;

    for (int i = 0; i < 10; ++i) {
        layer1.push_back(layer->create<Sin>());
    }

    for (int i = 0; i < 10; ++i) {
        auto item = layer->create<IntegrateFire>();
        layer2.push_back(item);

        for (auto object : layer1) {
            item->addInput(object);
        }
    }

    for (int i = 0; i < 10; ++i) {
        auto item = layer->create<IntegrateFire>();
        layer3.push_back(item);

        for (auto object : layer2) {
            item->addInput(object);
        }
    }

    client->socket << resp;
}

