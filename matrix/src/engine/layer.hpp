
#pragma once

#include <vector>
#include <memory>

class Object;

enum class LayerType {
    UNDEF    = -1,
    CNS      = 0,
    NETWORK  = 1,
    CELL     = 2,
};

class Layer {

private:

    LayerType layerType;
    std::vector<std::shared_ptr<Object>> objects;
};
