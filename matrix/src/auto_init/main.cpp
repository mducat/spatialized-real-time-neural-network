

#include "main.hpp"

#include <integrate_fire.hpp>
#include <project.hpp>
#include <qdebug.h>

#include "input_holder.hpp"
#include "sin.hpp"

void AutoInitMain::init(const std::shared_ptr<Project> &project) {
    // const auto cns = project->createLayer(LayerType::CNS);
    auto const net = project->createLayer(LayerType::NETWORK);
    // const auto cell = project->createLayer(LayerType::CELL);
    auto const lif = net->create<IntegrateFire>();
    //auto const lif = std::make_shared<IntegrateFire>();
    //net->addObject(lif);

    auto const in1 = net->create<InputHolder>(1.0);
    auto const in2 = net->create<InputHolder>(1.0);

    // auto const sin = net->create<Sin>();

    lif->connect(in1);
    lif->connect(in2);

    // lif->connect(sin);

    project->init();
    project->step();
    qWarning() << "Current value:" << lif->value();

    // Object *test = new InputHolder(5.1);
}
