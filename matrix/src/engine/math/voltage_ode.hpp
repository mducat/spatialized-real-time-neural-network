
#ifndef VOLTAGE_ODE_HPP
#define VOLTAGE_ODE_HPP

class VoltageODE {
public:
    virtual ~VoltageODE() = default;

    virtual double compute(double) = 0;
};

class LinearVoltage final : public VoltageODE {
public:
    explicit LinearVoltage(double);

    double compute(double) override;

private:
    double c0;
};

class QuadraticVoltage final : public VoltageODE {
public:
    QuadraticVoltage(double, double, double);

    double compute(double) override;

private:
    double c0;
    double c1;
    double c2;
};

class ExponentialVoltage final : public VoltageODE {
public:
    ExponentialVoltage(double, double, double);

    double compute(double) override;

private:
    double c0;
    double c1;
    double c2;
};

#endif //VOLTAGE_ODE_HPP
