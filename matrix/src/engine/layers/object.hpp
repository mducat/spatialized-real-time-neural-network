
#pragma once

enum class LayerType;

class Object {
public:
    virtual ~Object() = default;

    virtual LayerType getDestinationLayer() = 0;
    virtual void update(double) = 0;
};
