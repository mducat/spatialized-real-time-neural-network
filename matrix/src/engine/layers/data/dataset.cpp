//
// Created by thornhill on 03/01/25.
//

#include "dataset.hpp"
#include "debug.hpp"

Dataset::~Dataset() = default;

Dataset::Dataset(std::vector<std::shared_ptr<Tensor>> ins, std::vector<std::shared_ptr<Tensor>> outs)
    : _inputs(std::move(ins)), _outputs(std::move(outs)), _size(std::min(_inputs.size(), _outputs.size()))
{}

void Dataset::update(double delta) {
    if (this->_time_step < 0)
        return;

    this->_counter += delta;

    if (this->_counter > this->_time_step) {
        this->_counter = 0;
        this->_index += 1;
        this->_index %= this->_size;
    }
}

std::shared_ptr<Tensor> Dataset::input() const {
    return this->_inputs[this->_index];
}

std::shared_ptr<Tensor> Dataset::output() const {
    return this->_outputs[this->_index];
}
