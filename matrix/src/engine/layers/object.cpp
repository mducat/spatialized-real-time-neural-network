
#include "object.hpp"

#include <typeinfo>
#include <algorithm>

Object::Object() {
    static int idCounter = 0;
    this->objectId = idCounter++;
}

int Object::getObjectId() const {
    return this->objectId;
}

std::string Object::getObjectName() const {
    return "<Object: " + std::string(typeid(*this).name()) + "_" + std::to_string(this->objectId) + ">";
}

void Object::connect(const std::shared_ptr<Object> &obj) {
    this->inputs.push_back(obj);
}

void Object::disconnect(const std::shared_ptr<Object> &obj) {
    auto const it = std::remove(this->inputs.begin(), this->inputs.end(), obj);
    this->inputs.erase(it);
}

std::vector<std::shared_ptr<Object>> Object::getInputs() const {
    return this->inputs;
}

