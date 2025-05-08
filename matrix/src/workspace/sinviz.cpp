
#include <launcher.hpp>
#include <project.hpp>
#include <QApplication>
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
    QApplication app(ac, av);

    const std::shared_ptr<Project> &project = std::make_shared<Project>();

    initProject(project);

    auto *win = new Window;
    win->setProject(project);

    return launcher::exec(win);
}


/*void initWindow(Window *win) {
    sin2->update(2.5);

    win->show();

    win->analyze(sin);
    win->analyze(sin2)->setDisplayMode(AnalyzerValue::lines);

    win->runProject(30);
}*/
