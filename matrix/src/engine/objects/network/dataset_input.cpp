//
// Created by thornhill on 03/01/25.
//

#include "dataset.hpp"
#include "dataset_input.hpp"

#include <dataset/tensor.hpp>

#include "data/dataset.hpp"

DatasetInput::DatasetInput(std::shared_ptr<Dataset> from, std::size_t index)
    : _index(index) {
    this->addInput(std::reinterpret_pointer_cast<Object>(from));
}

DatasetInput::~DatasetInput() = default;

void DatasetInput::update(double) {}

double DatasetInput::value() const {
    std::shared_ptr<Dataset> ds = std::reinterpret_pointer_cast<Dataset>(this->_inputs[0]);
    std::shared_ptr<Tensor> tensor = ds->input();

    return tensor->at(this->_index);
}
