//
// Created by thornhill on 19/11/24.
//

#pragma once



#include <QWidget>

class ObjectList;
class Inspector;
class Layer;
class SceneTabs;
class LayerScene;
class Window;
class QGridLayout;
class Object;

class LayerDisplay final : public QWidget
{
    Q_OBJECT

public:
    explicit LayerDisplay(std::shared_ptr<Layer> const & layer);

private:

    LayerScene * _scene = nullptr;
    Inspector * _inspector = nullptr;
    ObjectList * _objects = nullptr;


    QGridLayout * _layout = nullptr;

    std::shared_ptr<Layer> _layer;

    void init();
    void selectObject(std::shared_ptr<Object> const &) const;
};
