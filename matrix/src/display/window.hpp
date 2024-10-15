
#pragma once

#include <QMainWindow>
#include <value.hpp>

#include "panel/scene_tabs.hpp"

class Toolbar;
class MenuBar;
class MainScene;
class AnalyzerDisplay;
class NetworkObject;
class Project;


class Window final : public QMainWindow
{
    Q_OBJECT

public:
    Window();

    void setProject(const std::shared_ptr<Project> &);

    std::shared_ptr<Project> getProject();

    void lookupProject() const;

private:

    void init();
    void initPanels();
    void initMenus();
    void initToolbar();

    bool _dynamicLookup = false;

    std::shared_ptr<Project> _project = nullptr;

    QWidget *_centralWidget = nullptr;
    MenuBar *_menuBar = nullptr;
    Toolbar *_toolBar = nullptr;
    SceneTabs *_sceneTabs = nullptr;

};
