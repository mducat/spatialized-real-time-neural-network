//
// Created by thornhill on 19/11/24.
//

#include <qboxlayout.h>

#include <viewer/scene.hpp>

#include "layer_display.hpp"

#include <object.hpp>

#include "scene_tabs.hpp"

#include "inspector/inspector.hpp"
#include "inspector/object_list.hpp"

LayerDisplay::LayerDisplay(std::shared_ptr<Layer> const &layer) : _layer(layer) {
    init();
}

void LayerDisplay::init() {
    this->_layout = new QGridLayout();

    this->_scene = new LayerScene(_layer);
    this->_inspector = new Inspector(_layer);
    this->_objects = new ObjectList(_layer);

    this->setLayout(_layout);
    this->_layout->setContentsMargins(0,0,0,0);

    this->_scene->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->_inspector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->_objects->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->_layout->addWidget(this->_objects, 0, 0, 1, 1);
    this->_layout->addWidget(this->_scene, 0, 1, 1, 1);
    this->_layout->addWidget(this->_inspector, 0, 2, 1, 1);

    // @todo resizable stretch
    this->_layout->setColumnStretch(0, 100);
    this->_layout->setColumnStretch(1, 400);
    this->_layout->setColumnStretch(2, 100);

    connect(this->_objects, &ObjectList::objectSelected, this, &LayerDisplay::selectObject); // NOLINT(*-unused-return-value)
}

void LayerDisplay::selectObject(std::shared_ptr<Object> const &object) const {
    this->_scene->selectObject(object);
}
