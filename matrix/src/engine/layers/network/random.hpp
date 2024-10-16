
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

    double _counter = 0;
    double _timeStep = 1.0;
    double _state = 0;
};


#endif //RANDOM_H
