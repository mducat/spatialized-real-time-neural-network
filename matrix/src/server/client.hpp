//
// Created by thornhill on 21/05/25.
//

#pragma once


#include "ByteObject.hpp"
#include "ByteObject.tcc"
#include "protocol.hpp"


class Project;
class QWebSocket;

struct Request {
    ByteObject data;
    uint64_t request_id;
};

struct ClientData {
    std::unordered_map<uint16_t, std::shared_ptr<Project>> projects;
};

class WSClient {
public:
    explicit WSClient(QWebSocket *, std::shared_ptr<ClientData>);

    QWebSocket *socket;
    std::shared_ptr<ClientData> client_data;
};

typedef std::shared_ptr<Request> req_t;
typedef std::shared_ptr<WSClient> client_t;

ByteObject status(uint8_t status, req_t &obj);
void operator<<(QWebSocket * const, ByteObject const &);

