

#include "sinviz.hpp"

#include <project.hpp>
#include <window.hpp>
#include <widgets/live_analyzer/display.hpp>

#include "sin.hpp"

void SinViz::initProject(const std::shared_ptr<Project> &project) {
    auto const test = project->createLayer(LayerType::NETWORK);

    sin = test->create<Sin>();
    sin2 = test->create<Sin>();

    project->scaleTime(5.0);
}

void SinViz::initWindow(Window *win) {
    sin2->update(2.5);

    win->show();

    win->analyze(sin);
    win->analyze(sin2)->setDisplayMode(AnalyzerValue::lines);

    win->runProject(50);
}
