
#ifndef SIN_H
#define SIN_H

#include "network_object.hpp"

class Sin final : public NetworkObject {
public:

    explicit Sin();
    ~Sin() override;

    void update(double) override;
    [[nodiscard]] double value() const override;

private:

    double state = 0;
};

#endif //SIN_H
