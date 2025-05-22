//
// Created by thornhill on 21/05/25.
//

#include <layer.hpp>

#include "controllers.hpp"

void ws::command::status(client_t &client, req_t &req) {
    auto resp = status(STATUS_OK, req);

    resp << PROTOCOL_VERSION_MAJOR << PROTOCOL_VERSION_MINOR << PROTOCOL_VERSION_PATCHLEVEL;

    client->socket << resp;
}

void ws::command::meta::types(client_t &client, req_t &req) {
    auto resp = status(STATUS_OK, req);

    int8_t l_start = static_cast<int8_t>(LayerType::_L_START);
    int8_t l_end = static_cast<int8_t>(LayerType::_L_END);

    resp << (l_end - l_start - 2);

    for (int8_t i = l_start + 2; i < l_end; i++) {
        resp << i << layerTypeToString(static_cast<LayerType>(i));
    }

    std::vector<std::string> types = {"sin", "random", "integrate_fire"};
    resp << types.size();
    for (uint32_t i = 0; i < types.size(); i++) {
        resp << i << types[i];
    }

    client->socket << resp;
}
