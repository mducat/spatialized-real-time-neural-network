
#pragma once

enum class LayerType;

class Object {
public:

    virtual LayerType getDestinationLayer() = 0;
    virtual void update() = 0;

private:

};
