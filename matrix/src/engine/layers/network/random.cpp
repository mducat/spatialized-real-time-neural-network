
#include "random.hpp"

#include <cstdlib>

double Random::value() const {
    return this->state;
}

Random::~Random() = default;

void Random::update(double const delta) {
    if (this->timeStep < 0)
        return;

    this->counter += delta;

    if (this->counter > this->timeStep) {
        this->counter = 0;

        int val = std::rand() % 30;
        double cast = static_cast<double>(val) / 100;

        this->state = cast;
    }
}
