//
// Created by thornhill on 21/05/25.
//

#pragma once


#include "server/client.hpp"

#define DECLARE(x) void x(client_t &, req_t &)

namespace ws {

    namespace create {
        void project(client_t &, req_t &);
        void layer(client_t &, req_t &);

        void demo(client_t &, req_t &);
    }

    /* namespace update {
        void project(client_t &, req_t &);
    } */

    namespace read {
        void project(client_t &, req_t &);
        void layer(client_t &, req_t &);
        void graph(client_t &, req_t &);
        void node(client_t &, req_t &);
        void end_node(client_t &, req_t &);
    }

    namespace command {

        void status(client_t &, req_t &);
        void start(client_t &, req_t &);
        void stop(client_t &, req_t &);

        namespace meta {

            void types(client_t &, req_t &);

        }
    }
}
