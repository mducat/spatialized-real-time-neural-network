
#include "sin.hpp"

#include <debug.hpp>

#include <cmath>

Sin::Sin() = default;
Sin::~Sin() = default;

void Sin::update(const double delta) {
    this->_state += delta;
}

double Sin::value() const {
    return std::sin(this->_state);
}




