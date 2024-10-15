//
// Created by thornhill on 14/10/24.
//

#include "toolbar.hpp"

#include <QToolBar>

Toolbar::Toolbar(Window *window) : QWidget(window), parent(window) {
    this->init();
};

QToolBar * Toolbar::getWidget() const {
    return this->toolbar;
}

void Toolbar::init() {
    this->toolbar = new QToolBar(this);

    toolbar->setWindowTitle(tr("main"));

    QIcon const startIcon = QIcon::fromTheme("media-playback-start");
    QIcon const stopIcon = QIcon::fromTheme("media-playback-pause");

    this->start = toolbar->addAction(startIcon, QString::fromStdString("Run project"));
    this->stop = toolbar->addAction(stopIcon, QString::fromStdString("Pause project"));

    /*auto cb = [&] {this->runProject(50);};
    connect(start, &QAction::triggered, this, cb);
    connect(stop, &QAction::triggered, this, &Window::pauseProject);*/
}

