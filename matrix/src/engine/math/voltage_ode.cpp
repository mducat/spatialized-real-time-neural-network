
#include "voltage_ode.hpp"

#include <cmath>

LinearVoltage::LinearVoltage(const double rest)
    : c0(rest)
{}

double LinearVoltage::compute(const double x) {
    return - (x - c0);
}

QuadraticVoltage::QuadraticVoltage(const double v0, const double v1, const double v2)
    : c0(v0), c1(v1), c2(v2)
{}

double QuadraticVoltage::compute(const double x) {
    return c1 * std::pow(x - c2, 2) + c0;
}

ExponentialVoltage::ExponentialVoltage(const double v0, const double v1, const double v2)
    : c0(v0), c1(v1), c2(v2)
{}

double ExponentialVoltage::compute(const double x) {
    return - (x - c0) + c1 * std::exp(x - c2);
}
