
#pragma once

#include <QWidget>

class Layer;

class Inspector final : public QWidget
{
    Q_OBJECT

public:
    explicit Inspector(std::shared_ptr<Layer> const & layer);

private:

    std::shared_ptr<Layer> _layer;

    void init();
};
