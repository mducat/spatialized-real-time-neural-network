

#include <layer.hpp>
#include <object.hpp>
#include <stdexcept>


void Layer::addObject(const std::shared_ptr<Object> &obj) {

    if (obj->getDestinationLayer() != this->layerType &&
        obj->getDestinationLayer() != LayerType::ANY) {

        std::string const dest = layerTypeToString(obj->getDestinationLayer());
        std::string const current = layerTypeToString(this->layerType);

        throw std::logic_error("Can not add " + dest + " object to " + current + " layer type !");
    }

    this->objects.push_back(obj);
}

void Layer::step() {
    for (const auto& object : objects) {
        object->update();
    }
}

