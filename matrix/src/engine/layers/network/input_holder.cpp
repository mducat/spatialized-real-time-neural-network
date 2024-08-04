
#include "input_holder.hpp"

#include <stdexcept>

InputHolder::InputHolder(std::vector<double> vals) : values(std::move(vals)) {
    if (values.empty())
        throw std::logic_error("Cannot hold an empty vector");
    this->setValue(values[0]);
}

InputHolder::InputHolder(const double v)  : state(v) {
    this->timeStep = -1;
}

InputHolder::~InputHolder() = default;

void InputHolder::update(const double delta) {
    if (this->timeStep < 0)
        return;

    this->counter += delta;

    if (this->counter > this->timeStep) {
        this->counter = 0;
        this->cursor += 1;
        this->cursor %= this->values.size();

        this->setValue(this->values[this->cursor]);
    }
}

double InputHolder::value() const {
    return this->state;
}

void InputHolder::setValue(const double val) {
    this->state = val;
}

void InputHolder::setValues(std::vector<double> vals) {
    this->values = std::move(vals);
}

void InputHolder::setTimeStep(double const step) {
    this->timeStep = step;
}

