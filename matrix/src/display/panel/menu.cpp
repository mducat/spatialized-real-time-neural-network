//
// Created by thornhill on 14/10/24.
//

#include "menu.hpp"

#include <project.hpp>
#include <qmenubar.h>
#include <window.hpp>

void MenuBar::init() {
    this->_menuBar = new QMenuBar(this);

    this->_newProjectAction = new QAction(QIcon::fromTheme("document-new"), tr("&New Project"), this);
    this->_newProjectAction->setShortcuts(QKeySequence::New);
    this->_newProjectAction->setStatusTip(tr("Create a new project"));
    connect(this->_newProjectAction, &QAction::triggered, this, &MenuBar::newProject);

    this->_openProjectAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open Project"), this);
    this->_openProjectAction->setShortcuts(QKeySequence::Open);
    this->_openProjectAction->setStatusTip(tr("Open a project from file"));
    connect(this->_openProjectAction, &QAction::triggered, this, &MenuBar::openProject);

    this->_saveProjectAction = new QAction(QIcon::fromTheme("document-save"), tr("&Save Project"), this);
    this->_saveProjectAction->setShortcuts(QKeySequence::Save);
    this->_saveProjectAction->setStatusTip(tr("Save current project"));
    connect(this->_saveProjectAction, &QAction::triggered, this, &MenuBar::saveProject);

    this->_fileMenu = this->_menuBar->addMenu(tr("&File"));
    this->_fileMenu->addAction(_newProjectAction);
    this->_fileMenu->addAction(_openProjectAction);
    this->_fileMenu->addAction(_saveProjectAction);

    this->_editMenu = this->_menuBar->addMenu(tr("&Edit"));
    this->_helpMenu = this->_menuBar->addMenu(tr("&Help"));
    this->_formatMenu = this->_menuBar->addMenu(tr("&Format"));
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
    return this->_menuBar;
}
