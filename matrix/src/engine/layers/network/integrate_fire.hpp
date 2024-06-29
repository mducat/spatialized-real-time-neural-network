
#pragma once

#include <memory>
#include <vector>

#include "object.hpp"

// https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=58ff012b2b360976bec73752b176b80a50f79cd2

class IntegrateFire final : public Object {
public:

    LayerType getDestinationLayer() override;
    void update() override;

    [[nodiscard]] double value() const override;

    void connect(const std::shared_ptr<Object> &);
    void disconnect(const std::shared_ptr<Object> &);

private:

    std::vector<std::shared_ptr<Object>> inputs;

    double voltThreshold = 0;
    double voltReset = 0;
    double tau = 0;

    double conductance = 0;
    double restPotential = 0;

    double refractoryPeriod = 0;

    double state = 0;
};
