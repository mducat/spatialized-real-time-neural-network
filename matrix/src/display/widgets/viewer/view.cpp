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

    const QPointF delta = (this->_start_point - event->pos()) * 1;

    Q_FOREACH(QGraphicsItem *item, this->items()) {
        item->setPos(item->pos() - delta);
    }

    // this->rotate(0.4);

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

    if (event->angleDelta().y() > 0)
        scale(fac, fac);
    else
        scale(1.0/fac, 1.0/fac);
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent) {
    // this->setMouseTracking(true);
    // this->setTransformationAnchor(NoAnchor);
    // this->setAlignment(Qt::AlignCenter);
    this->setTransformationAnchor(NoAnchor);
    this->setResizeAnchor(NoAnchor);
    setDragMode(ScrollHandDrag);
    this->setSceneRect(-500, -500, 500, 500);
}


