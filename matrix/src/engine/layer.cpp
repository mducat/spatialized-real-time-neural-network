

#include <layer.hpp>
#include <network_object.hpp>
#include <object.hpp>

#include <stdexcept>

#include <qdebug.h>


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
        //const std::shared_ptr<Object> &test = object;
        // const std::shared_ptr<NetworkObject> &try_cast = std::dynamic_pointer_cast<NetworkObject>(object);

        // qDebug() << "Object " << typeid(object.get()).name() << " has value " << try_cast->value();
    }
}

