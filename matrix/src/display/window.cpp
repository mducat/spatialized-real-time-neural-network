

#include <QLabel>
#include <QVBoxLayout>
#include <QtWidgets>

#include "window.hpp"

#include <project.hpp>

#include <widgets/live_analyzer/display.hpp>

Window::Window()
    : _project(std::make_shared<Project>())
{
    init();
}

void Window::setProject(const std::shared_ptr<Project> &project) {
    this->_project = project;
}

void Window::init()
{
    setWindowTitle(tr("Matrix"));
    resize(1200, 800);

    initPanels();
    initActions();
    initMenus();

    QString const message = tr("Matrix: init done");
    statusBar()->showMessage(message);
    qDebug("Matrix: init done");
}

void Window::initPanels()
{
    const auto widget = new QWidget;
    setCentralWidget(widget);

    /* QLabel *nativeLabel = new QLabel(tr("Test"), this);
    nativeLabel->setAlignment(Qt::AlignHCenter);

    QPushButton *test = new QPushButton(tr("Button"), this);
    test->move(100, 100);*/

    const auto layout = new QVBoxLayout;
    // layout->addWidget(nativeLabel);

    widget->setLayout(layout);
}

void Window::initActions()
{
    newProjectAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew), tr("&New Project"), this);
    newProjectAction->setShortcuts(QKeySequence::New);
    newProjectAction->setStatusTip(tr("Create a new project"));
    connect(newProjectAction, &QAction::triggered, this, &Window::newProject);

    openProjectAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), tr("&Open Project"), this);
    openProjectAction->setShortcuts(QKeySequence::Open);
    openProjectAction->setStatusTip(tr("Open a project from file"));
    connect(openProjectAction, &QAction::triggered, this, &Window::openProject);

    saveProjectAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), tr("&Save Project"), this);
    saveProjectAction->setShortcuts(QKeySequence::Save);
    saveProjectAction->setStatusTip(tr("Save current project"));
    connect(saveProjectAction, &QAction::triggered, this, &Window::saveProject);
}

void Window::initMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newProjectAction);
    fileMenu->addAction(openProjectAction);
    fileMenu->addAction(saveProjectAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    formatMenu = menuBar()->addMenu(tr("&Format"));
}

void Window::newProject()
{
    this->_project = std::make_shared<Project>();

    qDebug("newProject() called");
    // QWidget *wdg = new QWidget;
    auto *wdg = new AnalyzerDisplay;
    wdg->show();
    // hide();//this will disappear main window
}

void Window::openProject()
{
    qDebug("openProject() called");
}

void Window::saveProject()
{
    qDebug("saveProject() called");
}
