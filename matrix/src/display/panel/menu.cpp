//
// Created by thornhill on 14/10/24.
//

#include "menu.hpp"

#include <project.hpp>
#include <qmenubar.h>
#include <window.hpp>

void MenuBar::init() {
    this->_menu_bar = new QMenuBar(this);

    this->_new_project_action = new QAction(QIcon::fromTheme("document-new"), tr("&New Project"), this);
    this->_new_project_action->setShortcuts(QKeySequence::New);
    this->_new_project_action->setStatusTip(tr("Create a new project"));
    connect(this->_new_project_action, &QAction::triggered, this, &MenuBar::newProject);

    this->_open_project_action = new QAction(QIcon::fromTheme("document-open"), tr("&Open Project"), this);
    this->_open_project_action->setShortcuts(QKeySequence::Open);
    this->_open_project_action->setStatusTip(tr("Open a project from file"));
    connect(this->_open_project_action, &QAction::triggered, this, &MenuBar::openProject);

    this->_save_project_action = new QAction(QIcon::fromTheme("document-save"), tr("&Save Project"), this);
    this->_save_project_action->setShortcuts(QKeySequence::Save);
    this->_save_project_action->setStatusTip(tr("Save current project"));
    connect(this->_save_project_action, &QAction::triggered, this, &MenuBar::saveProject);

    this->_file_menu = this->_menu_bar->addMenu(tr("&File"));
    this->_file_menu->addAction(_new_project_action);
    this->_file_menu->addAction(_open_project_action);
    this->_file_menu->addAction(_save_project_action);

    this->_edit_menu = this->_menu_bar->addMenu(tr("&Edit"));
    this->_help_menu = this->_menu_bar->addMenu(tr("&Help"));
    this->_format_menu = this->_menu_bar->addMenu(tr("&Format"));
}


void MenuBar::newProject() {
    qDebug("newProject() called");

    this->_parent->setProject(std::make_shared<Project>());
}

void MenuBar::openProject() {
    qDebug("openProject() called");
}

void MenuBar::saveProject() {
    qDebug("saveProject() called");
}

MenuBar::MenuBar(Window *window) : QWidget(window), _parent(window) {
    this->init();
}

QMenuBar *MenuBar::getWidget() const {
    return this->_menu_bar;
}
