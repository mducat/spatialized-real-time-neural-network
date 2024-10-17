
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

    bool _dynamic_lookup = false;

    std::shared_ptr<Project> _project = nullptr;

    QWidget *_central_widget = nullptr;
    MenuBar *_menu_bar = nullptr;
    Toolbar *_tool_bar = nullptr;
    SceneTabs *_scene_tabs = nullptr;

};
