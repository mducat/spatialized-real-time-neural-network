
#include "network_object.hpp"

#include <layer.hpp>

// to implement in child (otherwise stop backward pass)
// ReSharper disable once CppMemberFunctionMayBeStatic
void NetworkObject::backwards(double) {}

LayerType NetworkObject::getDestinationLayer() {
    return LayerType::NETWORK;
}

std::shared_ptr<NetworkObject> NetworkObject::getNetworkObjectAt(const std::size_t index) const {
    auto val = std::reinterpret_pointer_cast<NetworkObject>(this->_inputs[index]);

    if (!val)
        throw std::runtime_error("NetworkObject::getNetworkObjectAt: Trying to poll non-network type object in network");

    return val;
}
