
#include "project.hpp"

#include <qdebug.h>

Project::Project() = default;

void Project::init() {
    _delta.start();
}

void Project::scaleTime(const double time) {
    _timeScale = time;
}

void Project::step() {
    double const delta = _delta.delta() * _timeScale;

    for (const auto &layer : layers) {
        layer->step(delta);
    }
}

std::shared_ptr<Layer> Project::createLayer(LayerType type) {
    auto newLayer = std::make_shared<Layer>(type);
    this->layers.push_back(newLayer);

    std::string const current = layerTypeToString(type);
    qCritical() << "Adding layer:" << layers.size() << "with type:" << current;

    return newLayer;
}

