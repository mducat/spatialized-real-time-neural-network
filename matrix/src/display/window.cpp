

#include <QLabel>
#include <QtWidgets>

#include "window.hpp"

#include <debug.hpp>
#include <project.hpp>

#include <widgets/live_analyzer/display.hpp>

#include "widgets/generic/plot.hpp"

Window::Window()
    : _project(std::make_shared<Project>())
{
    init();
}

void Window::setProject(const std::shared_ptr<Project> &project) {
    this->_project = project;
}

std::shared_ptr<Project> Window::getProject() {
    return this->_project;
}

void Window::init()
{
    setWindowTitle(tr("Matrix"));
    resize(1200, 800);

    initPanels();
    initActions();
    initMenus();
    initToolbar();

    initTimer();

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

void Window::initToolbar() {
    QToolBar *toolbar = addToolBar("main");

    QIcon const startIcon = QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart);
    QIcon const stopIcon = QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause);

    QAction const *start = toolbar->addAction(startIcon, QString::fromStdString("Run project"));
    QAction const *stop = toolbar->addAction(stopIcon, QString::fromStdString("Pause project"));

    auto cb = [&] {this->runProject(50);};
    connect(start, &QAction::triggered, this, cb);
    connect(stop, &QAction::triggered, this, &Window::pauseProject);
}

void Window::initTimer() {
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Window::tick));
}

AnalyzerValue *Window::analyze(const std::shared_ptr<NetworkObject> &obj) {
    if (!display) {
        display = new AnalyzerDisplay(nullptr);
        display->show();
    }

    return display->addAnalyzer(obj);
}

void Window::tick() const {
    this->_project->step();
    // @todo find a better place to record data, project callback ? workspace ?
    if (display) {
        display->record();
        display->repaint();
    }
}

void Window::plot(const std::function<double(double)> &func) {
    auto const p = new Plot(nullptr, func);
    p->show();
}

void Window::hideMatrix() {
    this->_hideMain = true;
}

bool Window::shouldDisplay() const {
    return !this->_hideMain;
}

void Window::newProject()
{
    // this->_project = std::make_shared<Project>();

    qDebug("newProject() called");

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

void Window::runProject(int const msec) const {
    qWarning() << "Starting project with:" << DISP(msec);
    timer->start(msec);
}

void Window::pauseProject() const {
    if (timer)
        timer->stop();
}

void Window::stopProject() const {
    this->pauseProject();

    this->_project->init();
}