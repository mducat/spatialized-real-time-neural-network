

#include <layer.hpp>
#include <network_object.hpp>
#include <object.hpp>

#include <stdexcept>

#include <qdebug.h>


Layer::Layer(const LayerType layer_type)
    : layerType(layer_type) {
    static int layerIdCounter;
    this->layerId = layerIdCounter++;
}

void Layer::addObject(const std::shared_ptr<Object> &obj) {

    if (obj->getDestinationLayer() != this->layerType &&
        obj->getDestinationLayer() != LayerType::ANY) {

        std::string const dest = layerTypeToString(obj->getDestinationLayer());
        std::string const current = layerTypeToString(this->layerType);

        throw std::logic_error("Can not add " + dest + " object to " + current + " layer type !");
    }

    this->objects.push_back(obj);
}

void Layer::step(const double delta) {
    for (const auto& object : objects) {
        object->update(delta);
    }
}

std::vector<std::shared_ptr<Object>> Layer::getObjects() {
    return this->objects;
}

int Layer::getLayerId() const {
    return this->layerId;
}

std::string Layer::name() const {
    return layerTypeToString(this->layerType) + "_" + std::to_string(this->layerId);
}

