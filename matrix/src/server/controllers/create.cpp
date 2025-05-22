//
// Created by thornhill on 21/05/25.
//

#include "controllers.hpp"


static uint16_t g_project_id = 0;

void ws::create::project(client_t &client, req_t &req) {
    uint16_t project_id = g_project_id++;

    client->socket << (status(STATUS_OK, req) << project_id);
}

