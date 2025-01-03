
#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class Project;
class Object;

enum class LayerType {
    UNDEF    = -2,
    ANY      = -1,
    CNS      = 0,
    NETWORK  = 1,
    CELL     = 2,
    DATA     = 3,
};

static std::string layerTypeToString(const LayerType type) {
    switch (type) {
        case LayerType::CNS: return "CNS";
        case LayerType::NETWORK: return "NETWORK";
        case LayerType::CELL: return "CELL";
        case LayerType::DATA: return "DATA";
        case LayerType::ANY: return "ANY";
        case LayerType::UNDEF:
        default: return "UNKNOWN";
    }
}

class Layer {
public:

    explicit Layer(LayerType layer_type, Project *parent);

    template<typename T, typename ...Args, std::enable_if_t<std::is_base_of_v<Object, T>>* = nullptr>
    std::shared_ptr<T> create(Args&&... args) {
        auto const obj = std::make_shared<T>(std::forward<Args>(args)...);
        this->addObject(obj);
        return obj;
    }

    void addObject(const std::shared_ptr<Object> &);
    void step(double);

    std::unordered_map<std::size_t, std::shared_ptr<Object>> &getObjects();

    [[nodiscard]] int getLayerId() const;
    [[nodiscard]] std::string name() const;
    void addCallback(const std::function<void()> &func);

    std::shared_ptr<Object> & getObjectById(std::size_t object_id);

private:

    Project *_parent;
    LayerType _layer_type;
    std::unordered_map<std::size_t, std::shared_ptr<Object>> _objects;
    int _layer_id = -1;
};
