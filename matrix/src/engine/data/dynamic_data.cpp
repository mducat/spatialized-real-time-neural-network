
#include "dynamic_data.hpp"

DynamicDataSource::DynamicDataSource(const std::function<double()> &getter)
    : _source(getter) {}

void DynamicDataSource::recordValue() {
    const double val = this->_source();

    if (val < _min_value)
        _min_value = val;

    if (val > _max_value)
        _max_value = val;

    this->_values.push_back(val);

    while (this->_values.size() > this->_size)
        this->_values.pop_front();
}

