

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

    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, QOverload<>::of(&Window::tick));

    const QRect screenGeometry = QApplication::primaryScreen()->geometry();
    const int x = (screenGeometry.width() - this->width()) / 2;
    const int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    status("Matrix: init done");
}

void Window::initPanels() {
    this->_central_widget = new QWidget;
    setCentralWidget(this->_central_widget);

    this->_layout = new QVBoxLayout;
    this->_layout->setContentsMargins(1, 1, 1, 1);
    this->_central_widget->setLayout(this->_layout);

    this->_scene_tabs = new SceneTabs(this);
    this->_layout->addWidget(this->_scene_tabs);
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

void Window::status(const std::string &message) const {
    QString const msg = tr(message.c_str());
    statusBar()->showMessage(msg);
    qInfo() << msg;
}

void Window::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    QApplication::quit();
}

void Window::tick() const {
    this->_project->step();
}

void Window::runProject(int msec) {
    this->_timer->start(msec);
}
