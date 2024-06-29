
#pragma once

#include <vector>

#include "layer.hpp"

class Project {
public:

    Project();

    void step() const;

    std::shared_ptr<Layer> createLayer(LayerType type);

private:
    std::vector<std::shared_ptr<Layer>> layers;

};
