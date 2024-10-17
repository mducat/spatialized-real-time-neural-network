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

    QMenuBar *_menu_bar = nullptr;

    QAction *_new_project_action = nullptr;
    QAction *_open_project_action = nullptr;
    QAction *_save_project_action = nullptr;

    QMenu *_file_menu = nullptr;
    QMenu *_edit_menu = nullptr;
    QMenu *_help_menu = nullptr;
    QMenu *_format_menu = nullptr;
};

