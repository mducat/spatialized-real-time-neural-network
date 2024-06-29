
#include "input_holder.hpp"

#include <layer.hpp>

LayerType InputHolder::getDestinationLayer() {
    return LayerType::ANY;
}

void InputHolder::update() {}

double InputHolder::value() const {
    return this->state;
}

void InputHolder::setValue(const double val) {
    this->state = val;
}

