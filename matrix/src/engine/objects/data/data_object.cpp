//
// Created by thornhill on 03/01/25.
//

#include "data_object.hpp"

#include <layer.hpp>

LayerType DataObject::getDestinationLayer() {
    return LayerType::DATA;
}
