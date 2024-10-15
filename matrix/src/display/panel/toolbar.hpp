//
// Created by thornhill on 14/10/24.
//

#pragma once

#include <qtoolbar.h>
#include <QWidget>
#include <window.hpp>

class Toolbar final : public QWidget
{
    Q_OBJECT

public:

    explicit Toolbar(Window *window);

    [[nodiscard]] QToolBar *getWidget() const;

private:

    void init();

    Window *parent;

    QAction *start = nullptr;
    QAction *stop = nullptr;

    QToolBar *toolbar = nullptr;
};

