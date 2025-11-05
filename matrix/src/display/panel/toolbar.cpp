//
// Created by thornhill on 14/10/24.
//

#include "toolbar.hpp"

#include <QToolBar>

Toolbar::Toolbar(Window *parent)
    : _parent(parent){
    this->init();
};

QToolBar * Toolbar::getWidget() const {
    return this->_toolbar;
}

void Toolbar::init() {
    this->_toolbar = new QToolBar(this);

    _toolbar->setWindowTitle(tr("main"));

    QIcon const startIcon = QIcon::fromTheme("media-playback-start");
    QIcon const stopIcon = QIcon::fromTheme("media-playback-pause");

    this->_start = _toolbar->addAction(startIcon, QString::fromStdString("Run project"));
    this->_stop = _toolbar->addAction(stopIcon, QString::fromStdString("Pause project"));

    auto cb = [&] {this->_parent->runProject(50);};
    connect(this->_start, &QAction::triggered, this, cb);

    auto cbStop = [&] {this->_parent->pauseProject();};
    connect(this->_stop, &QAction::triggered, this, cbStop);
}

