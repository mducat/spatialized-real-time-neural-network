
#include <algorithm>

#include "integrate_fire.hpp"
#include "layer.hpp"

LayerType IntegrateFire::getDestinationLayer() {
    return LayerType::NETWORK;
}

double IntegrateFire::value() const {
    return this->state;
}

void IntegrateFire::update() {
    double externalCurrent = 0;

    for (auto const &n : inputs)
        externalCurrent += n->value();

    this->state = externalCurrent;
}

void IntegrateFire::connect(const std::shared_ptr<Object> &obj) {
    this->inputs.push_back(obj);
}

void IntegrateFire::disconnect(const std::shared_ptr<Object> &obj) {
    auto const it = std::remove(this->inputs.begin(), this->inputs.end(), obj);
    this->inputs.erase(it);
}


