
#pragma once

#include <QMainWindow>

class Project;

class Window final : public QMainWindow
{
    Q_OBJECT

public:
    Window();

    void setProject(const std::shared_ptr<Project> &);

    // actions

    void newProject();
    void openProject();
    void saveProject();

private:

    void init();
    void initPanels();
    void initMenus();
    void initActions();

    std::shared_ptr<Project> _project;

    QAction *newProjectAction{};
    QAction *openProjectAction{};
    QAction *saveProjectAction{};

    QMenu *fileMenu{};
    QMenu *editMenu{};
    QMenu *formatMenu{};
    QMenu *helpMenu{};
};
