
#include "static_data.hpp"

#include "debug.hpp"

StaticDataSource::StaticDataSource(const std::function<double(double)> &getter)
    : source(getter) {
    this->generateValues();
}

void StaticDataSource::generateValues() {
    const double step = (this->maxRange - this->minRange) / static_cast<double>(this->size);
    double x = this->minRange;

    this->values.clear();

    for (std::size_t i = 0; i < this->size; x += step, i++) {
        double val = this->source(x);

        if (i == 0) {
            minValue = val;
            maxValue = val * 1.1;
        }

        if (val < minValue)
            minValue = val;

        if (val > maxValue)
            maxValue = val;

        this->values.push_back(val);
    }
}

void StaticDataSource::range(const double min, const double max) {
    this->minRange = min;
    this->maxRange = max;

    this->generateValues();
}

void StaticDataSource::resolution(std::size_t _size) {
    this->size = _size;

    this->generateValues();
}

