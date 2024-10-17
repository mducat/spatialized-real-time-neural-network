//
// Created by thornhill on 15/10/24.
//

#include "scene_tabs.hpp"

#include <project.hpp>
#include <window.hpp>

#include <qtabwidget.h>
#include <QVBoxLayout>
#include <viewer/scene.hpp>

SceneTabs::SceneTabs(Window *window) : QWidget(window), _parent(window) {
    init();
};

void SceneTabs::init() {

    this->setLayout(new QVBoxLayout);

    _layer_tabs = new QTabWidget(this);
    this->layout()->addWidget(_layer_tabs);
    this->layout()->setContentsMargins(0,0,0,0);
}

void SceneTabs::lookupProject() {
    for (auto const &layer : this->_parent->getProject()->getLayers()) {
        auto const scene = new LayerScene(this, layer);

        if (this->_scenes.find(layer->getLayerId()) != this->_scenes.end())
            continue;

        _layer_tabs->addTab(scene, QString::fromStdString(layer->name()));
        this->_scenes[layer->getLayerId()] = scene;
    }
}


