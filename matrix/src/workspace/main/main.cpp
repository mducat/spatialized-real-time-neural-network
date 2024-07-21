

#include "main.hpp"

#include <integrate_fire.hpp>
#include <project.hpp>
#include <qdebug.h>
#include <window.hpp>
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

    auto const in1 = net->create<InputHolder>(1.0);
    auto const in2 = net->create<InputHolder>(0.5);

    // auto const sinviz = net->create<Sin>();

    objs.push_back(in1);
    objs.push_back(in2);
    objs.push_back(lif);

    lif->connect(in1);
    lif->connect(in2);

    // lif->connect(sinviz);

    project->init();
    project->step();
    qWarning() << "Current value:" << lif->value();

    project->scaleTime(2.0);

    // Object *test = new InputHolder(5.1);
}

void Main::initWindow(Window *win) {
    // QWidget *wdg = new QWidget;

    win->show();

    for (auto const &item : objs) {
        win->analyze(item);
    }

    win->runProject(50);

    //win->runProject(100);
}
