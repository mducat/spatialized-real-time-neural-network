
#include "voltage_ode.hpp"

#include <cmath>

LinearVoltage::LinearVoltage(const double rest)
    : _c0(rest)
{}

double LinearVoltage::compute(const double x) {
    return - (x - _c0);
}

void LinearVoltage::setParams(const std::vector<double> params) {
    if (params.empty())
        return;

    this->_c0 = params.at(0);
}

QuadraticVoltage::QuadraticVoltage(const double v0, const double v1, const double v2)
    : _c0(v0), _c1(v1), _c2(v2)
{}

double QuadraticVoltage::compute(const double x) {
    return _c1 * std::pow(x - _c2, 2) + _c0;
}

void QuadraticVoltage::setParams(const std::vector<double> params) {
    if (params.empty() || params.size() < 3)
        return;

    this->_c0 = params.at(0);
    this->_c1 = params.at(1);
    this->_c2 = params.at(2);
}

ExponentialVoltage::ExponentialVoltage(const double v0, const double v1, const double v2)
    : _c0(v0), _c1(v1), _c2(v2)
{}

double ExponentialVoltage::compute(const double x) {
    return - (x - _c0) + _c1 * std::exp(x - _c2);
}

void ExponentialVoltage::setParams(const std::vector<double> params) {
    if (params.empty() || params.size() < 3)
        return;

    this->_c0 = params.at(0);
    this->_c1 = params.at(1);
    this->_c2 = params.at(2);
}

