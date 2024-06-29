
#pragma once

#include <vector>
#include <memory>

#include "object.hpp"

class Group final : public Object {
public:

    LayerType getDestinationLayer() override;
    void update() override;

private:
    std::vector<std::shared_ptr<Object>> objects;

    std::vector<std::shared_ptr<Object>> inputObjects;
    std::vector<std::shared_ptr<Object>> outputObjects;
};
