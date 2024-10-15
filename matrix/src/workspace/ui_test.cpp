
#include <debug.hpp>
#include <integrate_fire.hpp>
#include <launcher.hpp>
#include <project.hpp>
#include <QApplication>
#include <window.hpp>

int main(int ac, char **av) {
    launcher::init();
    QApplication app(ac, av);

    auto const &project = std::make_shared<Project>();
    auto const layer = project->createLayer(LayerType::NETWORK);

    std::vector<std::shared_ptr<IntegrateFire>> layer1;

    for (int i = 0; i < 10; ++i) {
        layer1.push_back(layer->create<IntegrateFire>());
    }

    qDebug() << DISP(layer1.at(0)->getObjectName().c_str());

    auto *win = new Window;
    win->setProject(project);

    return launcher::exec(win);
}



