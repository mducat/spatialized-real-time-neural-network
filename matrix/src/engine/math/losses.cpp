//
// Created by thornhill on 07/01/25.
//

#include "losses.hpp"

#include <cmath>

double LossFunction::compute(double, double) {
    return 0;
}

QuadraticLoss::QuadraticLoss() {}

double QuadraticLoss::compute(double a, double b) {
    return std::pow(b - a, 2);
}