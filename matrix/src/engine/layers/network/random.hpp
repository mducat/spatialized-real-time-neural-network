
#ifndef RANDOM_H
#define RANDOM_H

#include <vector>

#include "network_object.hpp"

class Random final : public NetworkObject {
public:

    ~Random() override;

    void update(double) override;
    [[nodiscard]] double value() const override;

private:

    double counter = 0;
    double timeStep = 1.0;
    double state = 0;
};


#endif //RANDOM_H
