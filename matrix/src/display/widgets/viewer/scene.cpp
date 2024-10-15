
#include "scene.hpp"

#include <object.hpp>
#include <qgraphicsproxywidget.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qpushbutton.h>

LayerScene::LayerScene(QWidget *parent, std::shared_ptr<Layer> const & layer) : QWidget(parent), _layer(layer) {
    this->init();
}

void LayerScene::lookupLayer() {
    this->updateObjectMap();
    this->updateView();
}

void LayerScene::init() {
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
        this->_objects[id_to_remove] = nullptr;
    }

    for (const auto& object : layer_objects) {
        if (this->_objects.find(object->getObjectId()) != this->_objects.end())
            continue;

        this->_objects[object->getObjectId()] = std::make_unique<ObjectDisplay>(object);
    }
}

void LayerScene::updateView() {
    std::unordered_map<int, QVector2D> objects_movement;
    // @todo loop in timer ?

    float const mag = 2.0;
    float temp_ = 1.0;

    for (auto &[id, obj] : this->_objects) {
        objects_movement[id] = QVector2D(0, 0);
    }

    for (auto &[id_a, obj_a] : this->_objects) {
        for (auto &[id_b, obj_b] : this->_objects) {

            if (id_a == id_b)
                continue;

            QVector2D delta = obj_a->getPosition() - obj_b->getPosition();
            float const dist = delta.length();

            if (dist > 1000)
                continue;

            float const repulsion = (mag * mag) / dist;

            objects_movement[id_a] += delta / dist * repulsion;
            objects_movement[id_b] -= delta / dist * repulsion;
        }

        for (auto object : obj_a->getObject()->getInputs()) {
            int id_b = object->getObjectId();
            std::unique_ptr<ObjectDisplay> &obj_b = this->_objects[id_b];

            QVector2D delta = obj_a->getPosition() - obj_b->getPosition();
            float const dist = delta.length();

            if (dist < 1.0)
                continue;

            float const attraction = (dist * dist) / mag;

            objects_movement[id_a] -= delta / dist * attraction;
            objects_movement[id_b] += delta / dist * attraction;
        }
    }

    for (auto [id, movement] : objects_movement) {
        float const dist = movement.length();

        float const capped_mvmt = std::min(dist, temp_);
        QVector2D mov = movement / dist * capped_mvmt;

        this->_objects[id]->update(mov.x(), mov.y());
    }
}

LayerScene::ObjectDisplay::ObjectDisplay(std::shared_ptr<Object> const &object) : _object(object) {}

std::shared_ptr<Object> LayerScene::ObjectDisplay::getObject() {
    return this->_object;
}

QVector2D LayerScene::ObjectDisplay::getPosition() const {
    return this->_position;
}

void LayerScene::ObjectDisplay::update(const float x, const float y) {
    this->_position.setX(x);
    this->_position.setY(y);
};
