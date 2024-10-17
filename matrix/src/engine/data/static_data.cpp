
#include "static_data.hpp"

#include "debug.hpp"

StaticDataSource::StaticDataSource(const std::function<double(double)> &getter)
    : _source(getter) {
    this->generateValues();
}

void StaticDataSource::generateValues() {
    const double step = (this->_max_range - this->_min_range) / static_cast<double>(this->_size);
    double x = this->_min_range;

    this->_values.clear();

    for (std::size_t i = 0; i < this->_size; x += step, i++) {
        double val = this->_source(x);

        if (i == 0) {
            _min_value = val;
            _max_value = val * 1.1;
        }

        if (val < _min_value)
            _min_value = val;

        if (val > _max_value)
            _max_value = val;

        this->_values.push_back(val);
    }
}

void StaticDataSource::range(const double min, const double max) {
    this->_min_range = min;
    this->_max_range = max;

    this->generateValues();
}

void StaticDataSource::resolution(std::size_t _size) {
    this->_size = _size;

    this->generateValues();
}

