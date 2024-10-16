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

    Window *_parent = nullptr;

    QMenuBar *_menuBar = nullptr;

    QAction *_newProjectAction = nullptr;
    QAction *_openProjectAction = nullptr;
    QAction *_saveProjectAction = nullptr;

    QMenu *_fileMenu = nullptr;
    QMenu *_editMenu = nullptr;
    QMenu *_helpMenu = nullptr;
    QMenu *_formatMenu = nullptr;
};

