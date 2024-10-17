
#include "scene.hpp"

#include <object.hpp>
#include <qgraphicsproxywidget.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qpushbutton.h>
#include <qrandom.h>
#include <qtimer.h>
#include <QVBoxLayout>

#include "view.hpp"

LayerScene::LayerScene(QWidget *parent, std::shared_ptr<Layer> const & layer) : QWidget(parent), _layer(layer) {
    this->init();
}

void LayerScene::lookupLayer() {
    this->updateObjectMap();
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
    std::vector<int> object_ids_to_remove;

    for (auto &[id, obj] : this->_objects) {
        if (std::find(layer_objects.begin(), layer_objects.end(), obj->getObject()) == layer_objects.end())
            object_ids_to_remove.push_back(id);
    }

    for (int id_to_remove : object_ids_to_remove) {
        this->_scene->removeItem(this->_objects[id_to_remove]->getEllipse());
        this->_objects[id_to_remove] = nullptr;
    }

    for (const auto& object : layer_objects) {
        int const objectId = object->getObjectId();
        if (this->_objects.find(objectId) != this->_objects.end())
            continue;

        this->_objects[objectId] = std::make_unique<ObjectDisplay>(object);
        this->_scene->addItem(this->_objects[objectId]->getEllipse());
    }
}

void LayerScene::updateView() {
    std::unordered_map<int, QVector2D> objects_movement;
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
            int id_b = object->getObjectId();
            const std::unique_ptr<ObjectDisplay> &obj_b = this->_objects[id_b];

            QVector2D delta = obj_a->getPosition() - obj_b->getPosition();
            float dist = delta.length();

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
        // QVector2D pos = this->_objects[id]->getPosition() + mov;

        this->_objects[id]->update(mov);
    }
    this->_view->centerOn(0,0);
}

void LayerScene::drawScene() {
    this->_scene->update();
    this->_view->repaint();
}

LayerScene::ObjectDisplay::ObjectDisplay(std::shared_ptr<Object> const &object)
    : _object(object),
      _ellipse(new QGraphicsEllipseItem(this->_position.x(), this->_position.y(), itemSize, itemSize)) {
    _ellipse->setBrush(QBrush(Qt::white));

    _position.setX(static_cast<float>(QRandomGenerator::global()->bounded(10.0)));
    _position.setY(static_cast<float>(QRandomGenerator::global()->bounded(10.0)));

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

    _ellipse->setPos(_ellipse->pos() + delta.toPoint());
}

QGraphicsEllipseItem *LayerScene::ObjectDisplay::getEllipse() const {
    return this->_ellipse;
};
