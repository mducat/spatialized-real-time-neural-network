
#include <../display/window.hpp>

#include <functional>
#include <launcher.hpp>
#include <QApplication>
#include <../engine/math/voltage_ode.hpp>

#define TO_F(func) [func](double const x) -> double { return func->compute(x); };

int main(int ac, char **av) {
    launcher::init();
    QApplication app(ac, av);

    /*Window *win;
    win->show();

    VoltageODE *test = new ExponentialVoltage(1, 1, 1);
    test->setParams({-0.7, -0.8, 4.9});
    std::function const f = TO_F(test);
    win->plot(f);

    VoltageODE *test2 = new QuadraticVoltage(1, 1, 1);
    test2->setParams({5, 6, 3});
    std::function const f2 = TO_F(test2);
    win->plot(f2);

    std::function const test3 = [](double const x) -> double { return std::sin(x); };
    win->plot(test3);*/
}
