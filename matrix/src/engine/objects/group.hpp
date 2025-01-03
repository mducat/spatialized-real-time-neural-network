
#pragma once

#include <vector>
#include <memory>

#include "object.hpp"

class Group final : public Object {
public:

    LayerType getDestinationLayer() override;
    void update(double) override;

private:
    std::vector<std::shared_ptr<Object>> _objects;

    std::vector<std::shared_ptr<Object>> _inputObjects;
    std::vector<std::shared_ptr<Object>> _outputObjects;
};
