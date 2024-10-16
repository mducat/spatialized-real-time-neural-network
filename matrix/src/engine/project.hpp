
#pragma once

#include <functional>
#include <vector>
#include <time/delta.hpp>

#include "layer.hpp"

class Project {
public:

    Project();

    void init();
    void step();

    void scaleTime(double);

    void addCallback(const std::function<void()> &func);

    std::shared_ptr<Layer> createLayer(LayerType type);
    std::vector<std::shared_ptr<Layer>> getLayers();

private:
    std::vector<std::shared_ptr<Layer>> _layers;
    std::vector<std::function<void()>> _callbacks;

    Delta _delta;
    double _timeScale = 1.0;

    double _maxAllowedDelta = 0.2;

};
