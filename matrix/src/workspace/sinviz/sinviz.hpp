
#ifndef SINVIZ_H
#define SINVIZ_H

#include "workspace.hpp"

class Sin;

class SinViz final : public Workspace {
public:
    ~SinViz() override = default;

    void initProject(const std::shared_ptr<Project> &) override;
    void initWindow(Window*) override;

    std::shared_ptr<Sin> sin;
    std::shared_ptr<Sin> sin2;
};

#endif //SINVIZ_H
