
#include "ui_test.hpp"

#include <debug.hpp>
#include <integrate_fire.hpp>
#include <project.hpp>

void UiTest::initProject(const std::shared_ptr<Project> &project) {
    auto const layer = project->createLayer(LayerType::NETWORK);

    std::vector<std::shared_ptr<IntegrateFire>> layer1;

    for (int i = 0; i < 10; ++i) {
        layer1.push_back(layer->create<IntegrateFire>());
    }

    qDebug() << DISP(layer1.at(0)->getObjectName());
}

void UiTest::initWindow(Window *) {

}


