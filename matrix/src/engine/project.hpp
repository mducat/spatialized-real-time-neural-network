
#pragma once

#include <functional>
#include <vector>
#include <time/delta.hpp>

#include "layer.hpp"

class Project {
public:

    Project();

    void init();
    void step(double = 0);

    void scaleTime(double);

    uint32_t addCallback(const std::function<void()> &func);
    void removeCallback(uint32_t);

    std::shared_ptr<Layer> createLayer(LayerType type);
    std::vector<std::shared_ptr<Layer>> getLayers();

private:
    std::vector<std::shared_ptr<Layer>> _layers;
    std::unordered_map<uint32_t, std::function<void()>> _callbacks;

    Delta _delta;
    double _time_scale = 1.0;

    double _max_allowed_delta = 0.2;

};
