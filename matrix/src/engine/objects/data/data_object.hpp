//
// Created by thornhill on 03/01/25.
//

#pragma once

#include "object.hpp"

class DataObject : public Object {
public:
    ~DataObject() override = default;

    LayerType getDestinationLayer() override;

};
