//
// Created by thornhill on 15/10/24.
//

#include "scene_tabs.hpp"

#include <project.hpp>
#include <window.hpp>

#include <qtabwidget.h>
#include <QVBoxLayout>
#include <viewer/scene.hpp>

#include "layer_display.hpp"

SceneTabs::SceneTabs(Window *window) : _parent(window) {
    init();
}

void SceneTabs::init() {
    this->setLayout(new QVBoxLayout);

    _layer_tabs = new QTabWidget(this);
    this->layout()->addWidget(_layer_tabs);
    this->layout()->setContentsMargins(0,0,0,0);
}

void SceneTabs::lookupProject() {
    for (auto const &layer : this->_parent->getProject()->getLayers()) {
        auto const scene = new LayerDisplay(layer);

        // @todo remove when deleted
        if (this->_scenes.contains(layer->getLayerId()))
            continue;

        qDebug() << this->size();
        _layer_tabs->addTab(scene, QString::fromStdString(layer->name()));
        this->_scenes[layer->getLayerId()] = scene;
    }
}


