
#pragma once
#include <string>

enum class LayerType;

class Object {
public:
    virtual ~Object() = default;
    Object();

    virtual LayerType getDestinationLayer() = 0;
    virtual void update(double) = 0;

    [[nodiscard]] int getObjectId() const;
    [[nodiscard]] std::string getObjectName() const;

private:

    int objectId = -1;
};
