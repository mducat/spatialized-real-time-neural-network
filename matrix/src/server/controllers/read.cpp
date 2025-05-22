//
// Created by thornhill on 22/05/25.
//

#include <project.hpp>
#include <ranges>

#include "controllers.hpp"

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

    for (uint16_t i = 0; i < layers.size(); i++) {
        resp << i << layers[i]->name();
    }

    client->socket << resp;
}
