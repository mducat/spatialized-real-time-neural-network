//
// Created by thornhill on 16/10/24.
//

#include "view.hpp"

#include <qevent.h>
#include <qgraphicsitem.h>

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);

    if (!_dragging)
        return;

    event->accept();

    const QPointF delta = (this->_start_point - event->pos()) / _scale;

    Q_FOREACH(QGraphicsItem *item, this->items()) {
        item->setPos(item->pos() - delta);
    }

    this->_start_point = event->position();
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);

    event->accept();

    if (!(event->buttons() & Qt::LeftButton))
        return;

    _dragging = true;
    this->_start_point = event->position();

    this->setCursor(Qt::ClosedHandCursor);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);

    event->accept();
    _dragging = false;

    this->setCursor(Qt::ArrowCursor);
}

void GraphicsView::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);

    constexpr float fac = 1.05;
    double scaling;

    if (event->angleDelta().y() > 0)
        scaling = fac;
    else
        scaling = 1.0 / fac;

    _scale *= scaling;
    scale(scaling, scaling);
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent) {
    this->setTransformationAnchor(NoAnchor);
    this->setResizeAnchor(NoAnchor);
    setDragMode(ScrollHandDrag);
    this->setSceneRect(0, 0, 500, 500);
}


