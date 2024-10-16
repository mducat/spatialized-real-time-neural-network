
#include "object.hpp"

#include <typeinfo>
#include <algorithm>

Object::Object() {
    static int idCounter = 0;
    this->_objectId = idCounter++;
}

int Object::getObjectId() const {
    return this->_objectId;
}

std::string Object::getObjectName() const {
    return "<Object: " + std::string(typeid(*this).name()) + "_" + std::to_string(this->_objectId) + ">";
}

void Object::connect(const std::shared_ptr<Object> &obj) {
    this->_inputs.push_back(obj);
}

void Object::disconnect(const std::shared_ptr<Object> &obj) {
    auto const it = std::remove(this->_inputs.begin(), this->_inputs.end(), obj);
    this->_inputs.erase(it);
}

std::vector<std::shared_ptr<Object>> Object::getInputs() const {
    return this->_inputs;
}

