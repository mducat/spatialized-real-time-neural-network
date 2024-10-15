
#include "project.hpp"

#include <qdebug.h>

Project::Project() = default;

void Project::init() {
    _delta.start();
}

void Project::scaleTime(const double time) {
    _timeScale = time;
}

void Project::addCallback(const std::function<void()> &func) {
    this->callbacks.push_back(func);
}


void Project::step() {
    double const delta = std::min(_delta.delta() * _timeScale, _maxAllowedDelta);

    for (const auto &layer : layers) {
        layer->step(delta);
    }

    for (const auto& callback : callbacks) {
        callback();
    }
}

std::shared_ptr<Layer> Project::createLayer(LayerType type) {
    auto newLayer = std::make_shared<Layer>(type);
    this->layers.push_back(newLayer);

    std::string const current = layerTypeToString(type);
    qCritical() << "Adding layer:" << layers.size() << "with type:" << current.c_str();

    return newLayer;
}

std::vector<std::shared_ptr<Layer>> Project::getLayers() {
    return this->layers;
}

