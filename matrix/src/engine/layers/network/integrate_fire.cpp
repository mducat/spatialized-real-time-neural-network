
#include "integrate_fire.hpp"
#include "layer.hpp"

LayerType IntegrateFire::getDestinationLayer()
{
    return LayerType::NETWORK;
}

void IntegrateFire::update()
{
    double sum = 0;

    for (auto const &n : inputs)
        sum += n->value();
}
