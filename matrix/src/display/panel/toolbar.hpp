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

    explicit Toolbar();

    [[nodiscard]] QToolBar *getWidget() const;

private:

    void init();

    QAction *_start = nullptr;
    QAction *_stop = nullptr;

    QToolBar *_toolbar = nullptr;
};

