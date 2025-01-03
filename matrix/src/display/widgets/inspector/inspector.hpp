
#pragma once

#include <qformlayout.h>
#include <QWidget>

class NetworkObject;
class Object;
class Layer;

class Inspector final : public QWidget
{
    Q_OBJECT

public:
    explicit Inspector(std::shared_ptr<Layer> const & layer);

    void selectObject(std::shared_ptr<Object> const &);

private:

    std::shared_ptr<Layer> _layer;
    QVBoxLayout * _layout = nullptr;
    QFormLayout * _form_layout = nullptr;

    void init();
    void inspectNetworkObject(std::shared_ptr<NetworkObject> const &);
};
