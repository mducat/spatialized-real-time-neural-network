
#include "group.hpp"
#include "layer.hpp"

LayerType Group::getDestinationLayer()
{
    return LayerType::ANY;
}

void Group::update(const double delta)
{
    for (auto const &obj : _objects)
        obj->update(delta);
}
