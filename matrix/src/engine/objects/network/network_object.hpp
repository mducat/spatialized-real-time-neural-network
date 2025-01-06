
#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include <object.hpp>

enum class LayerType;

class
NetworkObject : public Object {
public:

    ~NetworkObject() override = default;

    [[nodiscard]] virtual double value() const = 0;
    virtual void backwards(double);

    LayerType getDestinationLayer() override;
    [[nodiscard]] std::shared_ptr<NetworkObject> getNetworkObjectAt(std::size_t index) const;

};

#endif //NETWORK_OBJECT_H
