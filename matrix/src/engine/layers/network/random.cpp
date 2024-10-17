
#include "random.hpp"

#include <cstdlib>

double Random::value() const {
    return this->_state;
}

Random::~Random() = default;

void Random::update(double const delta) {
    if (this->_time_step < 0)
        return;

    this->_counter += delta;

    if (this->_counter > this->_time_step) {
        this->_counter = 0;

        int val = std::rand() % 30;
        double cast = static_cast<double>(val) / 100;

        this->_state = cast;
    }
}
