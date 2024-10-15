
#pragma once

#include <memory>
#include <network_object.hpp>
#include <vector>

#include "object.hpp"

// https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=58ff012b2b360976bec73752b176b80a50f79cd2

class VoltageODE;

class IntegrateFire final : public NetworkObject {
public:

    ~IntegrateFire() override;
    IntegrateFire();

    enum Mode {
        EXPONENTIAL,
        LINEAR,
        QUADRATIC
    };

    void setMode(Mode);
    void setStep(std::unique_ptr<VoltageODE> &&);

    template<typename T, typename ...Args, std::enable_if_t<std::is_base_of_v<VoltageODE, T>>* = nullptr>
    void setODE(Args&&... args) {
        this->step = std::make_unique<T>(args...);
    }

    void update(double) override;

    [[nodiscard]] double value() const override;

private:

    Mode mode = LINEAR;
    std::unique_ptr<VoltageODE> step;

    double voltageThreshold = -0.55;
    double voltageRest = -0.70;
    double voltageReset = voltageRest;
    double voltageFire = +0.40;

    double tau = 2.0;
    double resistance = 0.5;

    double repolarization = -1;
    double refractoryPeriod = (1.0 / 5.0) * 2;

    double state = voltageRest;

    // default quadratic mode eq params
    double qC0 = 1.0;
    double qC1 = 1.0;

    // default exponential mode eq params
    double eC0 = 1.0;
    double eC1 = 1.0;
};
