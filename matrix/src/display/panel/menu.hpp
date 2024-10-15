//
// Created by thornhill on 14/10/24.
//

#pragma once

#include <QWidget>

class Window;
class QMenu;
class QMenuBar;
class QAction;

class MenuBar final : public QWidget
{
    Q_OBJECT

public:
    explicit MenuBar(Window *window);

    [[nodiscard]] QMenuBar *getWidget() const;

private:

    void init();

    void newProject();
    void openProject();
    void saveProject();

    Window *parent = nullptr;

    QMenuBar *menuBar = nullptr;

    QAction *newProjectAction = nullptr;
    QAction *openProjectAction = nullptr;
    QAction *saveProjectAction = nullptr;

    QMenu *fileMenu = nullptr;
    QMenu *editMenu = nullptr;
    QMenu *helpMenu = nullptr;
    QMenu *formatMenu = nullptr;
};

