
#include "dynamic_data.hpp"

DynamicDataSource::DynamicDataSource(const std::function<double()> &getter)
    : source(getter) {}

void DynamicDataSource::recordValue() {
    const double val = this->source();

    if (val < minValue)
        minValue = val;

    if (val > maxValue)
        maxValue = val;

    this->values.push_back(val);

    while (this->values.size() > this->size)
        this->values.pop_front();
}

