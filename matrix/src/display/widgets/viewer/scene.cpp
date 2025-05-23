
#include <ranges>

#include "scene.hpp"

#include <debug.hpp>
#include <object.hpp>
#include <qgraphicsproxywidget.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qrandom.h>
#include <qtimer.h>
#include <QVBoxLayout>

#include "view.hpp"

LayerScene::LayerScene(std::shared_ptr<Layer> const & layer) : _layer(layer) {
    this->init();
}

void LayerScene::lookupLayer() {
    this->updateObjectMap();
    this->drawScene();
}

void LayerScene::setObjectColor(std::shared_ptr<Object> const &obj, Qt::GlobalColor const color) {
    std::size_t const objectId = obj->getObjectId();
    QGraphicsEllipseItem *ellipse = this->_objects[objectId]->getEllipse();

    ellipse->setBrush(QBrush(color));
}

void LayerScene::selectObject(std::shared_ptr<Object> const &obj) {
    if (this->_selected_object) {
        this->setObjectColor(this->_selected_object, Qt::white);
    }

    /*std::size_t const objectId = obj->getObjectId();
    QGraphicsEllipseItem const *ellipse = this->_objects[objectId]->getEllipse();

    this->_view->moveTo(QPointF(ellipse->x(),ellipse->y()));*/

    this->_selected_object = obj;
    this->setObjectColor(obj, Qt::red);
    this->drawScene();
}

void LayerScene::init() {
    this->setLayout(new QHBoxLayout);

    this->layout()->setContentsMargins(0,0,0,0);
    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    this->_scene = new QGraphicsScene;
    this->_view = new GraphicsView(this->_scene, this);

    this->layout()->addWidget(this->_view);

    this->_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->_view->setMouseTracking(true);

    this->_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // this->_view->setTransformationAnchor(QGraphicsView::NoAnchor);

    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, QOverload<>::of(&LayerScene::updateView));

    _timer->start(10);

    this->lookupLayer();
}

void LayerScene::updateObjectMap() {
    auto layer_objects = this->_layer->getObjects();
    std::vector<std::size_t> object_ids_to_remove;
    std::vector<std::string> connections_to_remove;

    // remove deleted connections

    for (auto const &[key, connection] : this->_connections) {

        std::size_t output_id = connection->getOutputId();
        std::size_t input_id = connection->getInputId();

        auto id_match_input = [&input_id](std::shared_ptr<Object> const &cmp) -> bool {
            return input_id == cmp->getObjectId();
        };

        if (layer_objects.contains(output_id)) {
            connections_to_remove.push_back(key);
            continue;
        }

        auto const inputs = this->_objects[output_id]->getObject()->getInputs();

        if (std::ranges::find_if(inputs, id_match_input) == inputs.end())
            connections_to_remove.push_back(key);
    }

    for (const auto& to_remove : connections_to_remove) {
        this->_scene->removeItem(this->_connections[to_remove]->getLine());
        this->_connections[to_remove] = nullptr;
    }

    // remove deleted objects

    for (auto &[id, obj] : this->_objects) {
        std::size_t const objectId = obj->getObject()->getObjectId();

        if (!layer_objects.contains(objectId))
            object_ids_to_remove.push_back(id);
    }

    for (std::size_t id_to_remove : object_ids_to_remove) {
        this->_scene->removeItem(this->_objects[id_to_remove]->getEllipse());
        this->_objects[id_to_remove] = nullptr;
    }

    // add points

    for (const auto& object : layer_objects | std::views::values) {
        std::size_t const objectId = object->getObjectId();
        if (this->_objects.contains(objectId))
            continue;

        this->_objects[objectId] = std::make_unique<ObjectDisplay>(object);
        this->_scene->addItem(this->_objects[objectId]->getEllipse());
    }

    // add connections

    for (const auto &output : layer_objects | std::views::values) {
        for (const auto &input : output->getInputs()) {
            const std::size_t input_id = input->getObjectId();
            const std::size_t output_id = output->getObjectId();

            std::string const key = std::to_string(input_id) + "_" + std::to_string(output_id);

            const auto &input_display = this->_objects[input_id];
            const auto &output_display = this->_objects[output_id];

            if (this->_connections.find(key) != this->_connections.end())
                continue;

            auto connection_display = std::make_shared<ConnectionDisplay>(input_display, output_display);
            this->_connections[key] = connection_display;
            this->_scene->addItem(connection_display->getLine());

            input_display->addInputConnection(connection_display);
            output_display->addOutputConnection(connection_display);
        }
    }
}

void LayerScene::updateView() {
    // qDebug() << this->size();

    std::unordered_map<std::size_t, QVector2D> objects_movement;
    // @todo stop timer when nothing to move

    float const mag = 200.0;
    float temp_ = 1.0;

    for (auto &[id, obj] : this->_objects) {
        objects_movement[id] = QVector2D(0, 0);
    }

    for (auto &[id_a, obj_a] : this->_objects) {
        for (auto &[id_b, obj_b] : this->_objects) {

            if (id_a == id_b)
                continue;

            QVector2D delta = obj_a->getPosition() - obj_b->getPosition();
            float dist = delta.length();

            if (dist > mag * mag)
                continue;

            if (dist == 0)
                dist = 1.0;

            float const repulsion = (mag * mag) / dist;

            objects_movement[id_a] += delta / dist * repulsion;
            objects_movement[id_b] -= delta / dist * repulsion;
        }

        for (const auto &object : obj_a->getObject()->getInputs()) {
            std::size_t id_b = object->getObjectId();
            const std::unique_ptr<ObjectDisplay> &obj_b = this->_objects[id_b];

            QVector2D delta = obj_a->getPosition() - obj_b->getPosition();
            const float dist = delta.length();

            if (dist < 30.0)
                continue;

            float const attraction = (dist * dist) / mag;

            objects_movement[id_a] -= delta / dist * attraction;
            objects_movement[id_b] += delta / dist * attraction;
        }
    }

    for (auto [id, movement] : objects_movement) {
        float dist = movement.length();

        if (dist == 0)
            dist = 1.0;

        float const capped_mvmt = std::min(dist, temp_);
        QVector2D mov = movement / dist * capped_mvmt;

        this->_objects[id]->update(mov);
    }
    this->_view->centerOn(0,0);
}

void LayerScene::drawScene() {
    this->_scene->update();
    this->_view->repaint();
}

LayerScene::ConnectionDisplay::ConnectionDisplay(std::unique_ptr<ObjectDisplay> const &in,
    std::unique_ptr<ObjectDisplay> const &out)
        : _position_in(in->getPosition()),
          _position_out(out->getPosition()),
          _input_id(in->getObject()->getObjectId()),
          _output_id(out->getObject()->getObjectId()),
          _line(new QGraphicsLineItem(QLineF(_position_in.toPoint(), _position_out.toPoint())))
{
    _position_in += QVector2D(item_size / 2, item_size / 2);
    _position_out += QVector2D(item_size / 2, item_size / 2);

    _line->setLine(QLineF(_position_in.toPoint(), _position_out.toPoint()));

    _line->setPen(QPen(Qt::white));
}

LayerScene::ConnectionDisplay::~ConnectionDisplay() {
    delete _line;
}

std::size_t LayerScene::ConnectionDisplay::getInputId() const {
    return _input_id;
}

std::size_t LayerScene::ConnectionDisplay::getOutputId() const {
    return _output_id;
}

void LayerScene::ConnectionDisplay::updateIn(const QVector2D delta) {
    this->_position_in = this->_position_in + delta;
    auto line = _line->line();

    line.setP1(line.p1() + delta.toPoint());
    _line->setLine(line);
    _line->update();
}

void LayerScene::ConnectionDisplay::updateOut(QVector2D delta) {
    this->_position_out = this->_position_out + delta;
    auto line = _line->line();

    line.setP2(line.p2() + delta.toPoint());
    _line->setLine(line);
    _line->update();
}

QGraphicsLineItem * LayerScene::ConnectionDisplay::getLine() const {
    return this->_line;
}

LayerScene::ObjectDisplay::ObjectDisplay(std::shared_ptr<Object> const &object)
    : _object(object),
      _ellipse(new QGraphicsEllipseItem(0, 0, item_size, item_size)) {
    _ellipse->setBrush(QBrush(Qt::white));

    _position.setX(static_cast<float>(QRandomGenerator::global()->bounded(1000.0)));
    _position.setY(static_cast<float>(QRandomGenerator::global()->bounded(1000.0)));

    _ellipse->setPos(_position.toPoint());
}

LayerScene::ObjectDisplay::~ObjectDisplay() {
    delete this->_ellipse;
}

std::shared_ptr<Object> LayerScene::ObjectDisplay::getObject() {
    return this->_object;
}

QVector2D LayerScene::ObjectDisplay::getPosition() const {
    return this->_position;
}

void LayerScene::ObjectDisplay::update(const QVector2D delta) {
    this->_position = this->_position + delta;

    for (const auto &input_connection : this->_input_connections)
        input_connection->updateIn(delta);

    for (const auto &output_connection : this->_output_connections)
        output_connection->updateOut(delta);

    _ellipse->setPos(_ellipse->pos() + delta.toPoint());
}

void LayerScene::ObjectDisplay::addInputConnection(std::shared_ptr<ConnectionDisplay> const &display) {
    this->_input_connections.push_back(display);
}

void LayerScene::ObjectDisplay::addOutputConnection(std::shared_ptr<ConnectionDisplay> const &display) {
    this->_output_connections.push_back(display);
}

void LayerScene::ObjectDisplay::removeInputConnection(std::shared_ptr<ConnectionDisplay> const &display) {
    auto const it = std::ranges::remove(this->_input_connections, display);
    this->_input_connections.erase(it.begin(), it.end());
}

void LayerScene::ObjectDisplay::removeOutputConnection(std::shared_ptr<ConnectionDisplay> const &display) {
    auto const it = std::ranges::remove(this->_output_connections, display);
    this->_output_connections.erase(it.begin(), it.end());
}

QGraphicsEllipseItem *LayerScene::ObjectDisplay::getEllipse() const {
    return this->_ellipse;
}
