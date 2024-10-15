//
// Created by thornhill on 15/10/24.
//

#include "scene_tabs.hpp"

#include <project.hpp>
#include <window.hpp>

#include <qtabwidget.h>
#include <QVBoxLayout>
#include <viewer/scene.hpp>

SceneTabs::SceneTabs(Window *window) : QWidget(window), parent(window) {
    init();
};

void SceneTabs::init() {

    w_layout_ = new QVBoxLayout(this);
    this->setLayout(w_layout_);

    layerTabs = new QTabWidget(this);
    this->layout()->addWidget(layerTabs);
}

void SceneTabs::lookupProject() {
    for (auto const &layer : this->parent->getProject()->getLayers()) {
        auto const scene = new LayerScene(this, layer);

        if (this->scenes.find(layer->getLayerId()) != this->scenes.end())
            continue;

        layerTabs->addTab(scene, QString::fromStdString(layer->name()));
        this->scenes[layer->getLayerId()] = scene;
    }
}


