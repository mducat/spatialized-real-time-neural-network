
#ifndef PLOT_TEST_HPP
#define PLOT_TEST_HPP

#include "workspace.hpp"

class NetworkObject;

class PlotTest final : public Workspace {
public:
    ~PlotTest() override = default;

    void initProject(const std::shared_ptr<Project> &) override;
    void initWindow(Window*) override;
};

#endif //PLOT_TEST_HPP
