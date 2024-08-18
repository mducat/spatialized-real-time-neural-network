
#include "object.hpp"

#include <typeinfo>

Object::Object() {
    static int idCounter = 0;
    this->objectId = idCounter++;
}

int Object::getObjectId() const {
    return this->objectId;
}

std::string Object::getObjectName() const {
    return std::string(typeid(this).name()) + "_" + std::to_string(this->objectId);
}
