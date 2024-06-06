
#pragma once

#include <memory>
#include <vector>

#include "object.hpp"

// https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=58ff012b2b360976bec73752b176b80a50f79cd2

class IntegrateFire : public Object {
public:

    LayerType getDestinationLayer() override;
    void update() override;

    double value();

    void connect(std::shared_ptr<IntegrateFire>);
    void disconnect(std::shared_ptr<IntegrateFire>);

private:

    std::vector<std::shared_ptr<IntegrateFire>> inputs;

    double voltThreshold;
    double voltReset;
    double tau;

    double state;
};
