//
// Created by thornhill on 02/01/25.
//

#pragma once

#include <vector>
#include <memory>

class Tensor;

class Dataset {
public:

    Dataset(std::vector<std::shared_ptr<Tensor>> ins, std::vector<std::shared_ptr<Tensor>> outs);

private:
    std::vector<std::shared_ptr<Tensor>> _inputs;
    std::vector<std::shared_ptr<Tensor>> _outputs;

    std::size_t _size;
};
