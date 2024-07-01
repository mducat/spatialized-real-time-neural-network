
#include "project.hpp"

#include <qdebug.h>

Project::Project() = default;

void Project::init() {
    _delta.start();
}


void Project::step() {
    qDebug() << "Project step: " << layers.size();
    for (const auto &layer : layers) {
        layer->step(_delta.delta());
    }
}

std::shared_ptr<Layer> Project::createLayer(LayerType type) {
    auto newLayer = std::make_shared<Layer>(type);
    this->layers.push_back(newLayer);
    qCritical() << "Adding layer: " << layers.size();

    return newLayer;
}

