

#include <layer.hpp>
#include <network_object.hpp>
#include <object.hpp>

#include <stdexcept>

#include <qdebug.h>


Layer::Layer(const LayerType layer_type)
    : _layerType(layer_type) {
    static int layerIdCounter;
    this->_layerId = layerIdCounter++;
}

void Layer::addObject(const std::shared_ptr<Object> &obj) {

    if (obj->getDestinationLayer() != this->_layerType &&
        obj->getDestinationLayer() != LayerType::ANY) {

        std::string const dest = layerTypeToString(obj->getDestinationLayer());
        std::string const current = layerTypeToString(this->_layerType);

        throw std::logic_error("Can not add " + dest + " object to " + current + " layer type !");
    }

    this->_objects.push_back(obj);
}

void Layer::step(const double delta) {
    for (const auto& object : _objects) {
        object->update(delta);
    }
}

std::vector<std::shared_ptr<Object>> Layer::getObjects() {
    return this->_objects;
}

int Layer::getLayerId() const {
    return this->_layerId;
}

std::string Layer::name() const {
    return layerTypeToString(this->_layerType) + "_" + std::to_string(this->_layerId);
}

