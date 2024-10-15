
#include <launcher.hpp>
#include <project.hpp>
#include <window.hpp>

#include "sin.hpp"

void initProject(const std::shared_ptr<Project> &project) {
    auto const test = project->createLayer(LayerType::NETWORK);

    std::shared_ptr<Sin> sin;
    std::shared_ptr<Sin> sin2;

    sin = test->create<Sin>();
    sin2 = test->create<Sin>();

    project->scaleTime(5.0);
}

int main(int ac, char **av) {
    launcher::init();

    const std::shared_ptr<Project> &project = std::make_shared<Project>();

    initProject(project);
}


/*void initWindow(Window *win) {
    sin2->update(2.5);

    win->show();

    win->analyze(sin);
    win->analyze(sin2)->setDisplayMode(AnalyzerValue::lines);

    win->runProject(30);
}*/
