
#include "input_holder.hpp"

InputHolder::~InputHolder() = default;

void InputHolder::update(double) {}

double InputHolder::value() const {
    return this->state;
}

void InputHolder::setValue(const double val) {
    this->state = val;
}

