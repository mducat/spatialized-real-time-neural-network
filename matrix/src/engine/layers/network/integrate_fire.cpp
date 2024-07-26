
#include <algorithm>

#include "integrate_fire.hpp"
#include "layer.hpp"
#include "math/voltage_ode.hpp"

IntegrateFire::IntegrateFire() {
    this->step = std::make_unique<LinearVoltage>(this->voltageRest);
}

IntegrateFire::~IntegrateFire() = default;

double IntegrateFire::value() const {
    return this->state;
}

void IntegrateFire::setMode(Mode) {
    switch (mode) {
        case LINEAR:
            this->step = std::make_unique<LinearVoltage>(this->voltageRest);
        break;
        case EXPONENTIAL:
            this->step = std::make_unique<ExponentialVoltage>(this->voltageRest, this->eC0, this->eC1);
        break;
        case QUADRATIC:
            this->step = std::make_unique<QuadraticVoltage>(this->voltageRest, this->qC0, this->qC1);
            break;
    }
}

void IntegrateFire::setStep(std::unique_ptr<VoltageODE> &&ode) {
    this->step = std::move(ode);
}

void IntegrateFire::update(const double _delta) {
    double externalCurrent = 0;

    for (auto const &n : inputs)
        externalCurrent += n->value();

    double deltaVoltage = this->step->compute(this->state);

    deltaVoltage += externalCurrent * this->resistance;
    this->state += deltaVoltage * _delta * (1 / this->tau);
}

void IntegrateFire::connect(const std::shared_ptr<NetworkObject> &obj) {
    this->inputs.push_back(obj);
}

void IntegrateFire::disconnect(const std::shared_ptr<NetworkObject> &obj) {
    auto const it = std::remove(this->inputs.begin(), this->inputs.end(), obj);
    this->inputs.erase(it);
}


