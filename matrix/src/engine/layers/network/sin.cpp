
#include "sin.hpp"

#include <cmath>

Sin::Sin() = default;
Sin::~Sin() = default;

void Sin::update(const double delta) {
    this->state += delta;
}

double Sin::value() const {
    return std::sin(this->state);
}




