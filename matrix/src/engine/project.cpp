
#include <ranges>

#include "project.hpp"

#include <qdebug.h>

Project::Project() = default;

void Project::init() {
    qWarning() << "Project::init()";
    _delta.start();
}

void Project::scaleTime(const double time) {
    _time_scale = time;
}

uint32_t Project::addCallback(const std::function<void()> &func) {
    static uint32_t handle_id = 0;
    uint32_t id = ++handle_id;

    this->_callbacks[id] = func;

    return id;
}

void Project::removeCallback(uint32_t handle) {
    this->_callbacks.erase(this->_callbacks.find(handle));
}

std::shared_ptr<Layer> Project::getLayer(uint16_t layer_id) {
    if (layer_id < 0 || layer_id >= _layers.size()) {
        throw std::invalid_argument("Invalid layer id");
    }
    return this->_layers[layer_id];
}

void Project::step(double const force_delta) {
    double delta = std::min(_delta.delta() * _time_scale, _max_allowed_delta);

    if (force_delta > delta) {
        delta = force_delta;
    }

    for (const auto &layer : _layers) {
        layer->step(delta);
    }

    for (const auto& callback : std::views::values(_callbacks)) {
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

