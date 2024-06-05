
#pragma once

#include <QMainWindow>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();

    void init();
    void initPanels();
    void initMenus();
    void initActions();

    // actions

    void newProject();
    void openProject();
    void saveProject();

private:

    QAction *newProjectAction;
    QAction *openProjectAction;
    QAction *saveProjectAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
};
