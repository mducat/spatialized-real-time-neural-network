
#include "voltage_ode.hpp"

#include <cmath>

LinearVoltage::LinearVoltage(const double rest)
    : c0(rest)
{}

double LinearVoltage::compute(const double x) {
    return - (x - c0);
}

void LinearVoltage::setParams(const std::vector<double> params) {
    if (params.empty())
        return;

    this->c0 = params.at(0);
}

QuadraticVoltage::QuadraticVoltage(const double v0, const double v1, const double v2)
    : c0(v0), c1(v1), c2(v2)
{}

double QuadraticVoltage::compute(const double x) {
    return c1 * std::pow(x - c2, 2) + c0;
}

void QuadraticVoltage::setParams(const std::vector<double> params) {
    if (params.empty() || params.size() < 3)
        return;

    this->c0 = params.at(0);
    this->c1 = params.at(1);
    this->c2 = params.at(2);
}

ExponentialVoltage::ExponentialVoltage(const double v0, const double v1, const double v2)
    : c0(v0), c1(v1), c2(v2)
{}

double ExponentialVoltage::compute(const double x) {
    return - (x - c0) + c1 * std::exp(x - c2);
}

void ExponentialVoltage::setParams(const std::vector<double> params) {
    if (params.empty() || params.size() < 3)
        return;

    this->c0 = params.at(0);
    this->c1 = params.at(1);
    this->c2 = params.at(2);
}

