//
// Created by thornhill on 02/01/25.
//

#include "tensor.hpp"

#include <sstream>

#include "debug.hpp"

#include <stdexcept>
#include <utility>

Tensor::Tensor(std::vector<float> &&values, std::shared_ptr<Shape> &shape)
    : _data(std::move(values)), _shape(shape) {
    std::size_t const shape_mod = this->_shape->count();

    if (this->_data.size() % shape_mod != 0) {
        std::string const details = "(" + std::to_string(this->_data.size()) + " % " + std::to_string(shape_mod) + "!= 0)";
        throw std::logic_error("Tensor size must be a multiple of the data size" + details);
    }
}

float Tensor::at(std::size_t index) const {
    if (index >= this->_data.size()) {
        throw std::out_of_range("index out of range in tensor");
    }
    // qDebug() << "Tensor::at(" << index << ") = " << this->_data[index];
    return this->_data[index];
}

std::size_t Tensor::Shape::count() const {
    const std::size_t shape_size = this->_shape.size();

    std::size_t shape_mod = this->_shape[0];
    for (int i = 1; i < shape_size; i++) {
        shape_mod *= this->_shape[i];
    }

    return shape_mod;
}

std::vector<float> Tensor::values() {
    return _data;
}

std::vector<float> Tensor::data() {
    return _data;
}

std::shared_ptr<Tensor::Shape> Tensor::shape() {
    return _shape;
}

std::string Tensor::display() const {
    std::stringstream stream;
    stream << "[";

    if (!this->_data.empty())
        stream << this->_data[0];

    for (std::size_t i = 1; i < this->_data.size(); i++) {
        stream << ", ";
        stream << this->_data[i];
    }

    stream << "]";
    return stream.str();
}
