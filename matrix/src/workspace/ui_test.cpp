
#include <debug.hpp>
#include <integrate_fire.hpp>
#include <launcher.hpp>
#include <project.hpp>
#include <QApplication>
#include <random.hpp>
#include <window.hpp>

int main(int ac, char **av) {
    launcher::init();
    QApplication app(ac, av);

    auto const &project = std::make_shared<Project>();
    auto const layer = project->createLayer(LayerType::NETWORK);

    project->createLayer(LayerType::CNS);

    std::vector<std::shared_ptr<Object>> layer1;
    std::vector<std::shared_ptr<Object>> layer2;
    std::vector<std::shared_ptr<Object>> layer3;

    for (int i = 0; i < 10; ++i) {
        layer1.push_back(layer->create<Random>());
        qDebug() << DISP(layer1.at(i)->getObjectId());
    }

    for (int i = 0; i < 6; ++i) {
        auto item = layer->create<IntegrateFire>();
        layer2.push_back(item);

        qDebug() << DISP(item->getObjectId());

        item->addInput(layer1.at(i));
        /*for (auto &integrate_fire : layer1) {
            item->addInput(integrate_fire);
        }*/
    }

    for (int i = 0; i < 12; ++i) {
        auto item = layer->create<IntegrateFire>();
        layer3.push_back(item);

        qDebug() << DISP(item->getObjectId());

        for (auto &integrate_fire : layer1) {
            item->addInput(integrate_fire);
        }
    }

    qDebug() << DISP(layer1.at(0)->getObjectName().c_str());

    auto *win = new Window;
    win->setProject(project);

    return launcher::exec(win);
}



