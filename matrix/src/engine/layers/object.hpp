
#pragma once

#include <memory>
#include <string>
#include <vector>

enum class LayerType;

class Object {
public:
    virtual ~Object() = default;
    Object();

    virtual LayerType getDestinationLayer() = 0;
    virtual void update(double) = 0;

    [[nodiscard]] std::size_t getObjectId() const;
    [[nodiscard]] std::string getObjectName() const;

    void addInput(const std::shared_ptr<Object> &);
    void removeInput(const std::shared_ptr<Object> &);

    [[nodiscard]] std::vector<std::shared_ptr<Object>> getInputs() const;

    [[nodiscard]] virtual double value() const = 0;

protected:
    std::vector<std::shared_ptr<Object>> _inputs;

    std::size_t _object_id = -1;
};
