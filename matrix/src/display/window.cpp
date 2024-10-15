

#include <QLabel>
#include <QtWidgets>

#include "window.hpp"

#include <project.hpp>
#include <panel/menu.hpp>
#include <panel/toolbar.hpp>

#include "panel/scene_tabs.hpp"

Window::Window()
{
    init();
}

void Window::setProject(const std::shared_ptr<Project> &project) {
    this->_project = project;
    this->lookupProject();
}

std::shared_ptr<Project> Window::getProject() {
    return this->_project;
}

void Window::init() {
    setWindowTitle(tr("Matrix"));
    resize(1200, 800);

    initPanels();
    initToolbar();
    initMenus();

    QString const message = tr("Matrix: init done");
    statusBar()->showMessage(message);
    qDebug("Matrix: init done");
}

void Window::initPanels() {
    this->_centralWidget = new QWidget(this);
    setCentralWidget(this->_centralWidget);

    auto *layout = new QVBoxLayout(this->_centralWidget);
    this->_centralWidget->setLayout(layout);

    this->_sceneTabs = new SceneTabs(this);
    this->_centralWidget->layout()->addWidget(this->_sceneTabs);
}

void Window::initMenus() {
    this->_menuBar = new MenuBar(this);
    this->setMenuBar(this->_menuBar->getWidget());
}

void Window::initToolbar() {
    this->_toolBar = new Toolbar(this);
    this->addToolBar(this->_toolBar->getWidget());
}

void Window::lookupProject() const {
    this->_sceneTabs->lookupProject();
}
