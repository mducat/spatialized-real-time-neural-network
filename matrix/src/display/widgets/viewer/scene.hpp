
#pragma once

#include <QWidget>

class MainScene final : public QWidget
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *);

private:
    void init();
};
