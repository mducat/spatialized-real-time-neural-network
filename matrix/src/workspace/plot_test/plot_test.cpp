
#include "plot_test.hpp"

#include <window.hpp>

#include <functional>
#include <voltage_ode.hpp>

void PlotTest::initProject(const std::shared_ptr<Project> &) {
}

#define TO_F(func) [func](double const x) -> double { return func->compute(x); };

void PlotTest::initWindow(Window *win) {
    win->show();

    VoltageODE *test = new ExponentialVoltage(1, 1, 1);
    test->setParams({2, 1, 5});
    std::function const f = TO_F(test);
    win->plot(f);

    VoltageODE *test2 = new QuadraticVoltage(1, 1, 1);
    test2->setParams({5, 6, 3});
    std::function const f2 = TO_F(test2);
    win->plot(f2);

    std::function const test3 = [](double const x) -> double { return std::sin(x); };
    win->plot(test3);
}
