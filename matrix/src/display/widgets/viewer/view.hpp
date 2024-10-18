//
// Created by thornhill on 16/10/24.
//

#pragma once

#include <qgraphicsview.h>

class GraphicsView final : public QGraphicsView
{
    Q_OBJECT

protected:

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    explicit GraphicsView(QGraphicsScene *, QWidget *);

private:
    QPointF _start_point;

    double _scale = 1.0;

    bool _dragging = false;
};
