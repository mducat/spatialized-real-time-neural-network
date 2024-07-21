
#ifndef MAIN_H
#define MAIN_H

#include <vector>

#include "workspace.hpp"

class NetworkObject;

class Main final : public Workspace {
public:
    ~Main() override = default;

    void initProject(const std::shared_ptr<Project> &) override;
    void initWindow(Window*) override;

	std::vector<std::shared_ptr<NetworkObject>> objs;
};

#endif //MAIN_H
