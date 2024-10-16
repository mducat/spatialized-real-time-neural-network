
#pragma once

#include <project.hpp>
#include <qgraphicsview.h>
#include <qvectornd.h>
#include <QWidget>

class LayerScene final : public QWidget
{
    Q_OBJECT

    class ObjectDisplay {
    public:
        explicit ObjectDisplay(std::shared_ptr<Object> const &object);

        std::shared_ptr<Object> getObject();
        [[nodiscard]] QVector2D getPosition() const;

        void update(float x, float y);

    private:
        std::shared_ptr<Object> _object;

        QVector2D _position;
    };

public:
    LayerScene(QWidget *parent, std::shared_ptr<Layer> const & layer);

    void lookupLayer();

private:
    void init();

    void updateObjectMap();
    void updateView();
    void drawScene();

    std::unordered_map<int, std::unique_ptr<ObjectDisplay>> _objects;
    std::shared_ptr<Layer> _layer;
    QGraphicsView *_view;
    QGraphicsScene *_scene;
};
