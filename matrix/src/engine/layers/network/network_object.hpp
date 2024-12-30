
#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include <object.hpp>

enum class LayerType;

class
NetworkObject : public Object {
public:

    ~NetworkObject() override = default;

    LayerType getDestinationLayer() override;

};

#endif //NETWORK_OBJECT_H
