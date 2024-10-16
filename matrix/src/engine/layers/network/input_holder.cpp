
#include "input_holder.hpp"

#include <stdexcept>

InputHolder::InputHolder(std::vector<double> vals) : _values(std::move(vals)) {
    if (_values.empty())
        throw std::logic_error("Cannot hold an empty vector");
    this->setValue(_values[0]);
}

InputHolder::InputHolder(const double v)  : _state(v) {
    this->_timeStep = -1;
}

InputHolder::~InputHolder() = default;

void InputHolder::update(const double delta) {
    if (this->_timeStep < 0)
        return;

    this->_counter += delta;

    if (this->_counter > this->_timeStep) {
        this->_counter = 0;
        this->_cursor += 1;
        this->_cursor %= this->_values.size();

        this->setValue(this->_values[this->_cursor]);
    }
}

double InputHolder::value() const {
    return this->_state;
}

void InputHolder::setValue(const double val) {
    this->_state = val;
}

void InputHolder::setValues(std::vector<double> vals) {
    this->_values = std::move(vals);
}

void InputHolder::setTimeStep(double const step) {
    this->_timeStep = step;
}

