//
// Created by thornhill on 03/01/25.
//

#include "dataset.hpp"
#include "dataset_entry.hpp"

#include <dataset/tensor.hpp>

#include "data/dataset.hpp"
#include "debug.hpp"

DatasetEntry::DatasetEntry(
    const std::shared_ptr<Dataset>& from,
    const std::size_t tensor_index,
    const std::size_t series_index)

: _tensor_index(tensor_index), _series_index(series_index) {
    this->addInput(from);
}

DatasetEntry::~DatasetEntry() = default;

void DatasetEntry::setTensorIndex(std::size_t idx) {
    this->_tensor_index = idx;
}

void DatasetEntry::setSeriesIndex(std::size_t idx) {
    this->_series_index = idx;
}

void DatasetEntry::update(double) {}

double DatasetEntry::value() const {
    std::shared_ptr<Dataset> const ds = std::reinterpret_pointer_cast<Dataset>(this->_inputs[0]);
    std::shared_ptr<Tensor> const tensor = ds->entry(this->_series_index);

    qDebug() << tensor->display().c_str();

    return tensor->at(this->_tensor_index);
}
