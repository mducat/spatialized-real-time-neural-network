
#pragma once

#include <QWidget>

class Layer;

class ObjectList final : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectList(std::shared_ptr<Layer> const & layer);

private:

    std::shared_ptr<Layer> _layer;

    void init();
};
