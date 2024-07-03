
#include "sin.hpp"

#include <debug.hpp>

#include <cmath>

Sin::Sin() = default;
Sin::~Sin() = default;

void Sin::update(const double delta) {
    qDebug() << DISP(this->state) << DISP(delta);
    this->state += delta;
}

double Sin::value() const {
    qDebug() << DISP(this->state) << DISP(std::sin(this->state));
    return std::sin(this->state);
}




