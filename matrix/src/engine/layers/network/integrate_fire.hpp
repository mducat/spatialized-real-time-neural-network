
#pragma once

#include <memory>
#include <network_object.hpp>
#include <vector>

#include "object.hpp"

// https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=58ff012b2b360976bec73752b176b80a50f79cd2

class IntegrateFire final : public NetworkObject {
public:

    ~IntegrateFire() override;

    void update(double) override;

    [[nodiscard]] double value() const override;

    void connect(const std::shared_ptr<NetworkObject> &);
    void disconnect(const std::shared_ptr<NetworkObject> &);

private:

    std::vector<std::shared_ptr<NetworkObject>> inputs;

    double voltThreshold = 0;
    double voltReset = 0;
    double tau = 0;

    double conductance = 0;
    double restPotential = 0;

    double refractoryPeriod = 0;

    double state = 0;
};
