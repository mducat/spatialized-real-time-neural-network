//
// Created by thornhill on 21/05/25.
//

#include "client.hpp"

#include <qbytearray.h>
#include <QtWebSockets>

#include "ByteObject.tcc"


WSClient::WSClient(QWebSocket *socket, std::shared_ptr<ClientData> data)
    : socket(socket), client_data(data) {
}

void operator<<(QWebSocket * const socket, ByteObject const &obj) {
    QByteArray resp_bin;
    resp_bin.append(reinterpret_cast<const char *>(obj.dumpForNetwork().data()), obj._size);

    socket->sendBinaryMessage(resp_bin);
}

ByteObject status(const uint8_t status, req_t &obj) {
    ByteObject resp;

    // header
    resp << obj->request_id;
    resp << status;

    return resp;
}

