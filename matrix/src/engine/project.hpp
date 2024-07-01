
#pragma once

#include <vector>
#include <time/delta.hpp>

#include "layer.hpp"

class Project {
public:

    Project();

    void init();
    void step();

    std::shared_ptr<Layer> createLayer(LayerType type);

private:
    std::vector<std::shared_ptr<Layer>> layers;

    Delta _delta;

};
