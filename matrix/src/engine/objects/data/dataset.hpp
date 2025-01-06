//
// Created by thornhill on 03/01/25.
//

#pragma once

#include <algorithm>

#include "data_object.hpp"


class Tensor;

class Dataset final : public DataObject {
public:

    ~Dataset() override;
    template <typename... Args>
    explicit Dataset(Args ...items) : _series{ items... } {
        std::vector<std::size_t> sizes;
        for (const auto& elems : this->_series) {
            sizes.push_back(elems.size());
        }

        this->_size = *std::ranges::min_element(sizes);
    };

    void update(double) override;

    [[nodiscard]] std::shared_ptr<Tensor> entry(std::size_t) const;

private:

    double _counter = 0;
    double _time_step = 1.0;

    std::vector<std::vector<std::shared_ptr<Tensor>>> _series;

    std::size_t _index = 0;

    std::size_t _size = 0;
};
