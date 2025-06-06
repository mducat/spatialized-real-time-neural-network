

#include <layer.hpp>
#include <object.hpp>
#include <project.hpp>
#include <ranges>

#include <stdexcept>

#include <qdebug.h>


Layer::Layer(const LayerType layer_type, Project *parent)
    : _layer_type(layer_type), _parent(parent) {
    static int layerIdCounter;
    this->_layer_id = layerIdCounter++;
}

void Layer::addObject(const std::shared_ptr<Object> &obj) {

    if (obj->getDestinationLayer() != this->_layer_type &&
        obj->getDestinationLayer() != LayerType::ANY) {

        std::string const dest = layerTypeToString(obj->getDestinationLayer());
        std::string const current = layerTypeToString(this->_layer_type);

        throw std::logic_error("Can not add " + dest + " object to " + current + " layer type !");
    }

    this->_objects[obj->getObjectId()] = obj;
}

void Layer::step(const double delta) {
    for (const auto& object : _objects | std::views::values) {
        object->update(delta);
    }
}

std::unordered_map<std::size_t, std::shared_ptr<Object>> & Layer::getObjects() {
    return this->_objects;
}

int Layer::getLayerId() const {
    return this->_layer_id;
}

std::string Layer::name() const {
    return layerTypeToString(this->_layer_type) + "_" + std::to_string(this->_layer_id);
}

void Layer::addCallback(const std::function<void()> &func) {
    this->_parent->addCallback(func);
}

std::shared_ptr<Object> & Layer::getObjectById(std::size_t object_id) {
    if (!this->_objects.contains(object_id)) {
        throw std::logic_error("Can not find Object " + std::to_string(object_id));
    }
    return this->_objects[object_id];
}

