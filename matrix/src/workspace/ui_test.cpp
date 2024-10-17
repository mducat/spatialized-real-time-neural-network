
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

    project->createLayer(LayerType::CNS);

    std::vector<std::shared_ptr<IntegrateFire>> layer1;
    std::vector<std::shared_ptr<IntegrateFire>> layer2;

    for (int i = 0; i < 10; ++i) {
        layer1.push_back(layer->create<IntegrateFire>());
    }

    for (int i = 0; i < 7; ++i) {
        auto item = layer->create<IntegrateFire>();
        layer2.push_back(item);

        for (auto integrate_fire : layer1) {
            item->connect(integrate_fire);
        }
    }

    qDebug() << DISP(layer1.at(0)->getObjectName().c_str());

    auto *win = new Window;
    win->setProject(project);

    return launcher::exec(win);
}



