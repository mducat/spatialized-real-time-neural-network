//
// Created by thornhill on 21/05/25.
//

#pragma once

#include <vector>

#include "ByteObject.hpp"
#include "ByteObject.tcc"
#include "protocol.hpp"


class DisplayServer;
class DynamicDataSource;
class Project;
class QWebSocket;
class QTimer;

struct Request {
    ByteObject data;
    uint64_t request_id;
};

struct ClientData {
    std::unordered_map<uint16_t, std::shared_ptr<Project>> projects;
    std::unordered_map<uint16_t, QTimer*> threads;
    std::unordered_map<uint16_t, std::unordered_map<uint32_t, std::shared_ptr<DynamicDataSource>>> values;
};

class WSClient {
public:
    explicit WSClient(QWebSocket *, const std::shared_ptr<ClientData> &, DisplayServer *);

    QWebSocket *socket;
    std::shared_ptr<ClientData> data;
    DisplayServer *parent;
};

typedef std::shared_ptr<Request> req_t;
typedef std::shared_ptr<WSClient> client_t;

ByteObject status(uint8_t status, req_t &obj);
void operator<<(QWebSocket * const, ByteObject const &);

