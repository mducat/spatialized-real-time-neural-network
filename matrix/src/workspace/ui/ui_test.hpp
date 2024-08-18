
#pragma once

#include "workspace.hpp"

class UiTest final : public Workspace {
public:
    void initProject(const std::shared_ptr<Project> &) override;
    void initWindow(Window*) override;
};

