//
// Created by thornhill on 15/10/24.
//

#pragma once

#include <QWidget>

class LayerDisplay;
class Window;
class QTabWidget;

class SceneTabs final : public QWidget
{
    Q_OBJECT

public:
    explicit SceneTabs(Window *window);

    void lookupProject();

private:

    std::unordered_map<int, LayerDisplay*> _scenes;

    Window *_parent;

    QTabWidget *_layer_tabs = nullptr;

    void init();
};

