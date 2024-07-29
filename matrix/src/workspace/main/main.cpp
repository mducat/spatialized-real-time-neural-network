

#include "main.hpp"

#include <integrate_fire.hpp>
#include <project.hpp>
#include <qdebug.h>
#include <window.hpp>
#include <math/voltage_ode.hpp>
#include <widgets/live_analyzer/display.hpp>

#include "input_holder.hpp"
#include "sin.hpp"

void Main::initProject(const std::shared_ptr<Project> &project) {
    // const auto cns = project->createLayer(LayerType::CNS);
    auto const net = project->createLayer(LayerType::NETWORK);
    // const auto cell = project->createLayer(LayerType::CELL);
    auto const lif = net->create<IntegrateFire>();
    //auto const lif = std::make_shared<IntegrateFire>();
    //net->addObject(lif);

    auto const in1 = net->create<InputHolder>(0.02);
    auto const in2 = net->create<InputHolder>(0.5);
    auto const sin = net->create<Sin>();

    // lif->setODE<ExponentialVoltage>(1.0, 1.0, 1.0);
    // lif->setMode()

    // auto const sinviz = net->create<Sin>();

    objs.push_back(in1);
    objs.push_back(in2);
    //objs.push_back(sin);
    objs.push_back(lif);

    sin->update(2.5);

    lif->connect(in1);
    lif->connect(in2);
    //lif->connect(sin);

    // lif->connect(sinviz);

    project->init();
    project->step();
    qWarning() << "Current value:" << lif->value();

    project->scaleTime(0.5);

    // Object *test = new InputHolder(5.1);
}

void Main::initWindow(Window *win) {
    // QWidget *wdg = new QWidget;

    win->hideMatrix();

    for (auto const &item : objs) {
        win->analyze(item)->setDisplayMode(AnalyzerValue::lines);
    }

    win->runProject(50);

    //win->runProject(100);
}
