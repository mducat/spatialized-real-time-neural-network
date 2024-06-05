
#pragma once

#include "object.hpp"

#include <vector>

class Group : Object {

private:
    std::vector<Object> objects;
};
