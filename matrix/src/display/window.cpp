

#include <QLabel>
#include <QtWidgets>

#include "window.hpp"

#include <project.hpp>

#include <widgets/live_analyzer/display.hpp>

#include "sin.hpp"

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

void Window::update() {
    this->_project->step();
    qDebug("step!");

    display->record();
    display->repaint();
}

void Window::newProject()
{
    // this->_project = std::make_shared<Project>();

    qDebug("newProject() called");
    // QWidget *wdg = new QWidget;
    display = new AnalyzerDisplay;
    display->show();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Window::update));
    timer->start(100);

    auto const test = this->_project->createLayer(LayerType::NETWORK);
    auto const sin = test->create<Sin>();

    // @todo move UI to AutoInit
    display->addAnalyzer(sin);

    this->_project->init();

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

void Window::closeEvent(QCloseEvent *event) {

    if (timer)
        timer->stop();

    event->accept();
}
