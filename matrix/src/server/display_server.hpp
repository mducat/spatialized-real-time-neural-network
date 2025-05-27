//
// Created by thornhill on 02/05/25.
//

#pragma once

#include <QObject>

#include "project.hpp"
#include "client.hpp"

class Blueprint;
class QWebSocketServer;
class QWebSocket;


class DisplayServer final : public QObject {
    Q_OBJECT
public:
    explicit DisplayServer();

    ~DisplayServer() override;

    void start(uint16_t port);
    QWebSocket *findSocket(QString addr);

private Q_SLOTS:

    void connected();
    void disconnected();
    void message(QByteArray data);

private:

    void init_blueprint() const;

    // void process(client_t &, req_t &);

    QWebSocketServer *_server;
    QList<QWebSocket *> _clients;

    std::unique_ptr<Blueprint> _bp;

    std::unordered_map<QString, std::shared_ptr<ClientData>> _clients_data;

    //std::unordered_map<uint16_t, std::shared_ptr<Project>> _projects;
};

