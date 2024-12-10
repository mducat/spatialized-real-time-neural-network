
#pragma once

#include <project.hpp>
#include <qgraphicsview.h>
#include <qvectornd.h>
#include <QWidget>

class GraphicsView;
constexpr float item_size = 20.0;

class LayerScene final : public QWidget
{
    Q_OBJECT

    class ObjectDisplay;

    class ConnectionDisplay {
    public:
        explicit ConnectionDisplay(std::unique_ptr<ObjectDisplay> const &in, std::unique_ptr<ObjectDisplay> const &out);

        ~ConnectionDisplay();

        [[nodiscard]] std::size_t getInputId() const;
        [[nodiscard]] std::size_t getOutputId() const;

        void updateIn(QVector2D delta);
        void updateOut(QVector2D delta);

        [[nodiscard]] QGraphicsLineItem *getLine() const;

    private:
        QVector2D _position_in;
        QVector2D _position_out;

        std::size_t _input_id;
        std::size_t _output_id;

        QGraphicsLineItem *_line;
    };

    class ObjectDisplay {
    public:
        explicit ObjectDisplay(std::shared_ptr<Object> const &object);

        ~ObjectDisplay();

        std::shared_ptr<Object> getObject();
        [[nodiscard]] QVector2D getPosition() const;

        void update(QVector2D delta);

        void addInputConnection(std::shared_ptr<ConnectionDisplay> const &display);
        void addOutputConnection(std::shared_ptr<ConnectionDisplay> const &display);

        void removeInputConnection(std::shared_ptr<ConnectionDisplay> const &display);
        void removeOutputConnection(std::shared_ptr<ConnectionDisplay> const &display);

        [[nodiscard]] QGraphicsEllipseItem *getEllipse() const;

    private:
        std::shared_ptr<Object> _object;
        std::vector<std::shared_ptr<ConnectionDisplay>> _input_connections;
        std::vector<std::shared_ptr<ConnectionDisplay>> _output_connections;

        QGraphicsEllipseItem *_ellipse;
        QVector2D _position;
    };

public:
    LayerScene(std::shared_ptr<Layer> const & layer);

    void lookupLayer();

    void selectObject(std::shared_ptr<Object> const &);

private:
    void init();

    void updateObjectMap();
    void updateView();
    void drawScene();

    void setObjectColor(std::shared_ptr<Object> const &obj, Qt::GlobalColor color);

    std::unordered_map<std::size_t, std::unique_ptr<ObjectDisplay>> _objects;
    std::unordered_map<std::string, std::shared_ptr<ConnectionDisplay>> _connections;
    std::shared_ptr<Object> _selected_object;
    std::shared_ptr<Layer> _layer;
    GraphicsView *_view = nullptr;
    QGraphicsScene *_scene = nullptr;

    QTimer *_timer = nullptr;
};
