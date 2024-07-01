
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

    void closeEvent(QCloseEvent *event) override;

private:

    void update();

    void init();
    void initPanels();
    void initMenus();
    void initActions();

    std::shared_ptr<Project> _project;

    QTimer *timer = nullptr;

    QAction *newProjectAction{};
    QAction *openProjectAction{};
    QAction *saveProjectAction{};

    QMenu *fileMenu{};
    QMenu *editMenu{};
    QMenu *formatMenu{};
    QMenu *helpMenu{};
};
