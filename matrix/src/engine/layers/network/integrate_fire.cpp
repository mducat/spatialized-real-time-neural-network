
#include <algorithm>

#include "integrate_fire.hpp"
#include "layer.hpp"

double IntegrateFire::value() const {
    return this->state;
}

IntegrateFire::~IntegrateFire() = default;

void IntegrateFire::update(double _delta) {
    double externalCurrent = 0;

    for (auto const &n : inputs)
        externalCurrent += n->value();

    this->state = externalCurrent;
}

void IntegrateFire::connect(const std::shared_ptr<NetworkObject> &obj) {
    this->inputs.push_back(obj);
}

void IntegrateFire::disconnect(const std::shared_ptr<NetworkObject> &obj) {
    auto const it = std::remove(this->inputs.begin(), this->inputs.end(), obj);
    this->inputs.erase(it);
}


