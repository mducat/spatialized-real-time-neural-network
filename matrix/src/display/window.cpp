

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

    const QRect screenGeometry = QApplication::primaryScreen()->geometry();
    const int x = (screenGeometry.width() - this->width()) / 2;
    const int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    QString const message = tr("Matrix: init done");
    statusBar()->showMessage(message);
    qDebug("Matrix: init done");
}

void Window::initPanels() {
    this->_central_widget = new QWidget(this);
    setCentralWidget(this->_central_widget);

    auto *layout = new QVBoxLayout(this->_central_widget);
    layout->setContentsMargins(1, 1, 1, 1);
    this->_central_widget->setLayout(layout);
    this->_central_widget->setMouseTracking(true);
    this->setMouseTracking(true);

    this->_scene_tabs = new SceneTabs(this);
    this->_central_widget->layout()->addWidget(this->_scene_tabs);
}

void Window::initMenus() {
    this->_menu_bar = new MenuBar(this);
    this->setMenuBar(this->_menu_bar->getWidget());
}

void Window::initToolbar() {
    this->_tool_bar = new Toolbar(this);
    this->addToolBar(this->_tool_bar->getWidget());
}

void Window::lookupProject() const {
    this->_scene_tabs->lookupProject();
}
