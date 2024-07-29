
#include "workspace/main/main.hpp"

#include <project.hpp>
#include <QApplication>

#include "sinviz.hpp"
#include "window.hpp"
#include "plot_test.hpp"

#define LOG_PATTERN  "\033[39;1m[%{time process}" \
                     "%{if-debug}\033[95;1m    %{endif}"\
                     "%{if-info}\033[94;1m     %{endif}"\
                     "%{if-warning}\033[93;1m  %{endif}"\
                     "%{if-critical}\033[91;1m %{endif}"\
                     "%{if-fatal}\033[96;1m    %{endif}"\
                     "%{type}\033[39m - %{function} L%{line}] - %{message}\033[39;0m"

QtMessageHandler originalHandler = nullptr;

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    // static FILE *f = fopen("log.txt", "a");
    // printf(f, "%s\n", qPrintable(message));
    // fflush(f);
    // puts((const char *) message.data());

    if (originalHandler)
        (*originalHandler)(type, context, msg);
}

int main(int ac, char **av)
{
    qSetMessagePattern(LOG_PATTERN);
    originalHandler = qInstallMessageHandler(logger);
    QApplication app(ac, av);

    Workspace *current = new Main;

    auto const project = std::make_shared<Project>();
    current->initProject(project);

    Window win;
    win.setProject(project);
    current->initWindow(&win);

    if (!win.isVisible() && win.shouldDisplay()) {
        win.show();
    }

    return QApplication::exec();
}
