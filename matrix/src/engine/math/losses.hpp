//
// Created by thornhill on 07/01/25.
//

#pragma once

class LossFunction {
public:
    virtual ~LossFunction() = default;

    static double compute(double, double);
};

class QuadraticLoss final : public LossFunction {
public:
    QuadraticLoss();

    static double compute(double, double);
};
