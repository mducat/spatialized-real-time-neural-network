
#include "group.hpp"
#include "layer.hpp"

LayerType Group::getDestinationLayer()
{
    return LayerType::ANY;
}

void Group::update(const double delta)
{
    for (auto const &obj : objects)
        obj->update(delta);
}
