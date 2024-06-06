
#include "group.hpp"
#include "layer.hpp"

LayerType Group::getDestinationLayer()
{
    return LayerType::UNDEF;
}

void Group::update()
{
    for (auto const &obj : objects)
        obj->update();
}
