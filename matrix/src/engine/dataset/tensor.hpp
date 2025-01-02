//
// Created by thornhill on 02/01/25.
//

#pragma once

#include <memory>
#include <vector>

class Tensor {
public:

    class Shape {
    public:
        template <typename... Args>
        explicit Shape(Args ...shape) : _shape{ shape... } {};

        [[nodiscard]] std::size_t count() const;
    private:
        std::vector<std::size_t> _shape;
    };

    Tensor(std::vector<float> &&values, std::shared_ptr<Shape> &shape);

    std::vector<float> values();
    std::vector<float> data();
    std::shared_ptr<Shape> shape();

private:
    std::vector<float> _data;
    std::shared_ptr<Shape> _shape;
};
