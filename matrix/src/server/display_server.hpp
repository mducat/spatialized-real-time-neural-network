//
// Created by thornhill on 02/05/25.
//

#pragma once

#include <ByteObject.hpp>
#include <project.hpp>
#include <QObject>

class QWebSocketServer;
class QWebSocket;

class DisplayServer final : public QObject {
    Q_OBJECT
public:
    explicit DisplayServer();

    ~DisplayServer() override;

    void start(uint16_t port);

private Q_SLOTS:

    void connected();
    void disconnected();
    void message(QByteArray data);

private:

    void process(QWebSocket *, ByteObject &);

    void create(QWebSocket *, ByteObject &);
    void read(QWebSocket *, ByteObject &);
    void update(QWebSocket *, ByteObject &);
    void del(QWebSocket *, ByteObject &);
    void command(QWebSocket *, ByteObject &);

    void cmd_meta(QWebSocket *, ByteObject &);

    QWebSocketServer *_server;
    QList<QWebSocket *> _clients;

    std::map<uint16_t, std::shared_ptr<Project>> _projects;
};

ByteObject status(uint8_t status);