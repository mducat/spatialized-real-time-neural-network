//
// Created by thornhill on 02/05/25.
//


#include <launcher.hpp>
#include <qcoreapplication.h>

#include "server/display_server.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    launcher::init();

    DisplayServer server;
    server.start(16180);

    return a.exec();
}
