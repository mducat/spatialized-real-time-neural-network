//
// Created by thornhill on 15/10/24.
//

#pragma once

#include <QVBoxLayout>
#include <QWidget>

class LayerScene;
class Window;
class QTabWidget;

class SceneTabs final : public QWidget
{
    Q_OBJECT

public:
    explicit SceneTabs(Window *window);

    void lookupProject();

private:

    std::unordered_map<int, LayerScene*> scenes;

    Window *parent;

    QTabWidget *layerTabs = nullptr;
    QVBoxLayout *w_layout_ = nullptr;

    void init();
};

