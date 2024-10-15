

#include <input_holder.hpp>
#include <integrate_fire.hpp>
#include <project.hpp>
#include <QApplication>
#include <qdebug.h>
#include <window.hpp>
#include <voltage_ode.hpp>

#include "random.hpp"
#include "sin.hpp"
#include "launcher.hpp"

int main(int ac, char **av) {
    launcher::init();
    QApplication app(ac, av);

    const std::shared_ptr<Project> &project = std::make_shared<Project>();
    std::vector<std::shared_ptr<NetworkObject>> objs;


    // const auto cns = project->createLayer(LayerType::CNS);
    auto const net = project->createLayer(LayerType::NETWORK);
    // const auto cell = project->createLayer(LayerType::CELL);
    auto const lif = net->create<IntegrateFire>();
    auto const lif2 = net->create<IntegrateFire>();
    //auto const lif = std::make_shared<IntegrateFire>();
    //net->addObject(lif);

#if 0
    auto const in1 = net->create<InputHolder>(std::vector{0.02, 0.0, 0.2, 0.1});
    auto const in2 = net->create<InputHolder>(std::vector{0.0, 0.1, 0.35});
    auto const in3 = net->create<InputHolder>(std::vector{0.0, 0.1, 0.35});
#else
    auto const in1 = net->create<Random>();
    auto const in2 = net->create<Random>();
    auto const in3 = net->create<Random>();
#endif

    auto const sin = net->create<Sin>();

    // lif->setODE<LinearVoltage>(-0.7);
    lif->setODE<ExponentialVoltage>(-0.7, -0.8, 4.9);
    // lif->setODE<QuadraticVoltage>(2.0, 1.0, 5.0);
    // lif->setMode()

    // auto const sinviz = net->create<Sin>();

    objs.push_back(in1);
    objs.push_back(in2);
    //objs.push_back(sin);
    objs.push_back(lif);
    objs.push_back(in3);
    objs.push_back(lif2);

    sin->update(2.5);

    lif->connect(in1);
    lif->connect(in2);
    //lif->connect(sin);

    lif2->connect(in2);
    lif2->connect(in3);

    // lif->connect(sinviz);

    project->init();
    project->step();
    qWarning() << "Current value:" << lif->value();

    project->scaleTime(8.0);

    auto *win = new Window;

    // Object *test = new InputHolder(5.1);

    /*for (auto const &item : objs) {
        win->analyze(item)->setDisplayMode(AnalyzerValue::lines);
    }*/

    // win->runProject(10);

    return launcher::exec(win);
}
