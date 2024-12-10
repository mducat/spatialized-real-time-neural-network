

#include "integrate_fire.hpp"
#include "layer.hpp"
#include "math/voltage_ode.hpp"

IntegrateFire::IntegrateFire() {
    this->_step = std::make_unique<LinearVoltage>(this->_voltage_rest);
}

IntegrateFire::~IntegrateFire() = default;

double IntegrateFire::value() const {
    return this->_state;
}

void IntegrateFire::setMode(Mode) {
    switch (_mode) {
        case LINEAR:
            this->_step = std::make_unique<LinearVoltage>(this->_voltage_rest);
        break;
        case EXPONENTIAL:
            this->_step = std::make_unique<ExponentialVoltage>(this->_voltage_rest, this->_eC0, this->_eC1);
        break;
        case QUADRATIC:
            this->_step = std::make_unique<QuadraticVoltage>(this->_voltage_rest, this->_qC0, this->_qC1);
            break;
    }
}

void IntegrateFire::setStep(std::unique_ptr<VoltageODE> &&ode) {
    this->_step = std::move(ode);
}

void IntegrateFire::update(const double _delta) {
    double externalCurrent = 0;
    double deltaVoltage = this->_step->compute(this->_state);

    if (this->_repolarization >= 0) {
        this->_repolarization += _delta;

        if (this->_repolarization > this->_refractory_period)
            this->_repolarization = -1;

        this->_state += 10 * deltaVoltage * _delta * (1 / this->_tau);

        return;
    }

    for (auto const &n : _inputs)
        externalCurrent += std::static_pointer_cast<NetworkObject>(n)->value();

    deltaVoltage += externalCurrent * this->_resistance;
    this->_state += deltaVoltage * _delta * (1 / this->_tau);

    if (this->_state > this->_voltage_threshold) {
        this->_state = this->_voltage_fire;

        this->_repolarization = 0.0;
    }
}

