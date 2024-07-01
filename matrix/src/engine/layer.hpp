
#pragma once

#include <vector>
#include <memory>
#include <string>

class Object;

enum class LayerType {
    UNDEF    = -2,
    ANY      = -1,
    CNS      = 0,
    NETWORK  = 1,
    CELL     = 2,
};

static std::string layerTypeToString(LayerType type) {
    switch (type) {
        case LayerType::CNS: return "CNS";
        case LayerType::NETWORK: return "NETWORK";
        case LayerType::CELL: return "CELL";
        case LayerType::ANY: return "ANY";
        case LayerType::UNDEF:
        default: return "UNKNOWN";
    }
}

class Layer {
public:

    explicit Layer(const LayerType layer_type)
        : layerType(layer_type) {}

    template<typename T, typename ...Args, std::enable_if_t<std::is_base_of_v<Object, T>>* = nullptr>
    std::shared_ptr<T> create(Args&&... args) {
        auto const obj = std::make_shared<T>(std::forward<Args>(args)...);
        this->addObject(obj);
        return obj;
    }

    void addObject(const std::shared_ptr<Object> &);
    void step(double);

private:

    LayerType layerType;
    std::vector<std::shared_ptr<Object>> objects;
};
