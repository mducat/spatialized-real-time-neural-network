
#pragma once

#include <project.hpp>
#include <qgraphicsview.h>
#include <qvectornd.h>
#include <QWidget>

constexpr float itemSize = 20.0;

class LayerScene final : public QWidget
{
    Q_OBJECT

    class ConnectionDisplay {
    public:
        explicit ConnectionDisplay(std::shared_ptr<Object> const &object);

        ~ConnectionDisplay();

        std::shared_ptr<Object> getObject();
        [[nodiscard]] QVector2D getPosition() const;

        void update(QVector2D delta);

        [[nodiscard]] QGraphicsEllipseItem *getEllipse() const;

    private:
        std::shared_ptr<Object> _object;

        QGraphicsLineItem *_line;
        QVector2D _position;
    };

    class ObjectDisplay {
    public:
        explicit ObjectDisplay(std::shared_ptr<Object> const &object);

        ~ObjectDisplay();

        std::shared_ptr<Object> getObject();
        [[nodiscard]] QVector2D getPosition() const;

        void update(QVector2D delta);

        [[nodiscard]] QGraphicsEllipseItem *getEllipse() const;

    private:
        std::shared_ptr<Object> _object;
        std::vector<std::shared_ptr<ConnectionDisplay>> _input_connections;
        std::vector<std::shared_ptr<ConnectionDisplay>> _output_connections;

        QGraphicsEllipseItem *_ellipse;
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
    std::unordered_map<std::string, std::unique_ptr<ConnectionDisplay>> _connections;
    std::shared_ptr<Layer> _layer;
    QGraphicsView *_view;
    QGraphicsScene *_scene;

    QTimer *_timer;
};
