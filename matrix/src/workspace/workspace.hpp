
#ifndef WORKSPACE_HPP
#define WORKSPACE_HPP

#include <memory>

class Project;
class Window;

class Workspace {
public:
    virtual ~Workspace() = default;

    virtual void initProject(const std::shared_ptr<Project> &) = 0;
    virtual void initWindow(Window*) = 0;
};


#endif //WORKSPACE_HPP
