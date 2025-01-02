//
// Created by thornhill on 02/01/25.
//

#pragma once
#include "dataset/tensor.hpp"

class DataSet;

std::vector<std::shared_ptr<Tensor>> loadMNIST(const char *filename, int expected_magic, std::shared_ptr<Tensor::Shape> shape);

