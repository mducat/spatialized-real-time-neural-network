//
// Created by thornhill on 14/10/24.
//

#include "menu.hpp"

#include <project.hpp>
#include <qmenubar.h>
#include <window.hpp>

void MenuBar::init() {
    this->menuBar = new QMenuBar(this);

    this->newProjectAction = new QAction(QIcon::fromTheme("document-new"), tr("&New Project"), this);
    this->newProjectAction->setShortcuts(QKeySequence::New);
    this->newProjectAction->setStatusTip(tr("Create a new project"));
    connect(this->newProjectAction, &QAction::triggered, this, &MenuBar::newProject);

    this->openProjectAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open Project"), this);
    this->openProjectAction->setShortcuts(QKeySequence::Open);
    this->openProjectAction->setStatusTip(tr("Open a project from file"));
    connect(this->openProjectAction, &QAction::triggered, this, &MenuBar::openProject);

    this->saveProjectAction = new QAction(QIcon::fromTheme("document-save"), tr("&Save Project"), this);
    this->saveProjectAction->setShortcuts(QKeySequence::Save);
    this->saveProjectAction->setStatusTip(tr("Save current project"));
    connect(this->saveProjectAction, &QAction::triggered, this, &MenuBar::saveProject);

    this->fileMenu = this->menuBar->addMenu(tr("&File"));
    this->fileMenu->addAction(newProjectAction);
    this->fileMenu->addAction(openProjectAction);
    this->fileMenu->addAction(saveProjectAction);

    this->editMenu = this->menuBar->addMenu(tr("&Edit"));
    this->helpMenu = this->menuBar->addMenu(tr("&Help"));
    this->formatMenu = this->menuBar->addMenu(tr("&Format"));
}


void MenuBar::newProject() {
    qDebug("newProject() called");

    this->parent->setProject(std::make_shared<Project>());
}

void MenuBar::openProject() {
    qDebug("openProject() called");
}

void MenuBar::saveProject() {
    qDebug("saveProject() called");
}

MenuBar::MenuBar(Window *window) : QWidget(window), parent(window) {
    this->init();
}

QMenuBar *MenuBar::getWidget() const {
    return this->menuBar;
}
