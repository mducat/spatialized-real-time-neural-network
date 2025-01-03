//
// Created by thornhill on 03/01/25.
//

#pragma once

#include "data_object.hpp"


class Tensor;

class Dataset final : public DataObject {
public:

    ~Dataset() override;
    Dataset(std::vector<std::shared_ptr<Tensor>> ins, std::vector<std::shared_ptr<Tensor>> outs);

    void update(double) override;

    [[nodiscard]] std::shared_ptr<Tensor> input() const;
    [[nodiscard]] std::shared_ptr<Tensor> output() const;

private:

    double _counter = 0;
    double _time_step = 1.0;

    std::vector<std::shared_ptr<Tensor>> _inputs;
    std::vector<std::shared_ptr<Tensor>> _outputs;

    std::size_t _index = 0;

    std::size_t _size;
};
