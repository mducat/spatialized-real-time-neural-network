
#ifndef EMPTY_HPP
#define EMPTY_HPP

#include "workspace.hpp"

class Empty final : public Workspace {
public:
    void initProject(const std::shared_ptr<Project> &) override;
    void initWindow(Window*) override;
};

#endif //EMPTY_HPP
