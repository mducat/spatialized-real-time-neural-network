
#pragma once

#include <QMainWindow>
#include <widgets/live_analyzer/value.hpp>

class AnalyzerDisplay;
class NetworkObject;
class Project;

class Window final : public QMainWindow
{
    Q_OBJECT

public:
    Window();

    void setProject(const std::shared_ptr<Project> &);

    std::shared_ptr<Project> getProject();

    // actions

    void newProject();
    void openProject();
    void saveProject();

    void closeEvent(QCloseEvent *event) override;

    void runProject(int msec = 50) const;
    void stopProject() const;
    void pauseProject() const;

    AnalyzerValue *analyze(const std::shared_ptr<NetworkObject> &);

    void plot(const std::function<double(double)> &);

    void hideMatrix();
    bool shouldDisplay() const;

private:

    void tick() const;

    void init();
    void initPanels();
    void initMenus();
    void initToolbar();
    void initActions();
    void initTimer();

    bool _hideMain = false;

    std::shared_ptr<Project> _project;

    AnalyzerDisplay *display = nullptr;
    QTimer *timer = nullptr;

    QAction *newProjectAction{};
    QAction *openProjectAction{};
    QAction *saveProjectAction{};

    QMenu *fileMenu{};
    QMenu *editMenu{};
    QMenu *formatMenu{};
    QMenu *helpMenu{};
};
