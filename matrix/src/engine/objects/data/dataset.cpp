//
// Created by thornhill on 03/01/25.
//

#include "dataset.hpp"
#include "debug.hpp"

Dataset::~Dataset() = default;

void Dataset::update(const double delta) {
    if (this->_time_step < 0)
        return;

    this->_counter += delta;

    if (this->_counter > this->_time_step) {
        this->_counter = 0;
        this->_index += 1;
        this->_index %= this->_size;
    }
}

std::shared_ptr<Tensor> Dataset::entry(const std::size_t series_idx) const {
    return this->_series[series_idx][this->_index];
}
