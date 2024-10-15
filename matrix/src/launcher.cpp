
#include "launcher.hpp"

#include <iostream>
#include <QApplication>
#include <QWidget>

#define LOG_PATTERN  "\033[39;1m[%{time process}" \
                     "%{if-debug}\033[95;1m    %{endif}"\
                     "%{if-info}\033[94;1m     %{endif}"\
                     "%{if-warning}\033[93;1m  %{endif}"\
                     "%{if-critical}\033[91;1m %{endif}"\
                     "%{if-fatal}\033[96;1m    %{endif}"\
                     "%{type}\033[39m - %{function} L%{line}] - %{message}\033[39;0m"

void launcher::init()
{
    qSetMessagePattern(LOG_PATTERN);
}

int launcher::exec(QWidget *win)
{
    win->show();
    return QApplication::exec();
}
