
#include "project.hpp"

Project::Project() = default;

void Project::step() const {
    for (const auto &layer : layers) {
        layer->step();
    }
}

std::shared_ptr<Layer> Project::createLayer(LayerType type) {
    auto newLayer = std::make_shared<Layer>(type);
    this->layers.push_back(newLayer);

    return newLayer;
}

