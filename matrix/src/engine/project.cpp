
#include "project.hpp"

#include <qdebug.h>

Project::Project() = default;

void Project::init() {
    _delta.start();
}

void Project::scaleTime(const double time) {
    _time_scale = time;
}

void Project::addCallback(const std::function<void()> &func) {
    this->_callbacks.push_back(func);
}

void Project::step(double const force_delta) {
    double delta = std::min(_delta.delta() * _time_scale, _max_allowed_delta);

    if (force_delta > delta) {
        delta = force_delta;
    }

    for (const auto &layer : _layers) {
        layer->step(delta);
    }

    for (const auto& callback : _callbacks) {
        callback();
    }
}

std::shared_ptr<Layer> Project::createLayer(LayerType type) {
    auto newLayer = std::make_shared<Layer>(type, this);
    this->_layers.push_back(newLayer);

    std::string const current = layerTypeToString(type);
    qInfo() << "Adding layer:" << _layers.size() << "with type:" << current.c_str();

    return newLayer;
}

std::vector<std::shared_ptr<Layer>> Project::getLayers() {
    return this->_layers;
}

