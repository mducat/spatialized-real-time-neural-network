
#include "network_object.hpp"

#include <layer.hpp>

LayerType NetworkObject::getDestinationLayer() {
    return LayerType::NETWORK;
}
