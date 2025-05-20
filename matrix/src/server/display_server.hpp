//
// Created by thornhill on 02/05/25.
//

#pragma once

#include <ByteObject.hpp>
#include <project.hpp>
#include <QObject>

class QWebSocketServer;
class QWebSocket;

struct Request {
    ByteObject data;
    uint64_t request_id;
};

typedef std::shared_ptr<Request> ReqPtr;

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

    void process(QWebSocket *, ReqPtr &);

    void create(QWebSocket *, ReqPtr &);
    void read(QWebSocket *, ReqPtr &);
    void update(QWebSocket *, ReqPtr &);
    void del(QWebSocket *, ReqPtr &);
    void command(QWebSocket *, ReqPtr &);

    void cmd_meta(QWebSocket *, ReqPtr &);

    QWebSocketServer *_server;
    QList<QWebSocket *> _clients;

    std::map<uint16_t, std::shared_ptr<Project>> _projects;
};

ByteObject status(uint8_t status, ReqPtr &obj);