
#pragma once

enum class LayerType;

// @todo sub object types (NetworkObject? => value)

class Object {
public:
    virtual ~Object() = default;

    virtual LayerType getDestinationLayer() = 0;
    virtual void update() = 0;
    [[nodiscard]] virtual double value() const = 0;

private:

};
