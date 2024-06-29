
#ifndef MAIN_H
#define MAIN_H

#include <memory>

class Project;

class AutoInitMain {
public:
    static void init(const std::shared_ptr<Project> &);
};

#endif //MAIN_H
