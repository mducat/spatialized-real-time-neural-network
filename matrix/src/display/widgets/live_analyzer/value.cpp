
#include <QPainter>
#include <QRectF>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QLineF>

#include "value.hpp"

AnalyzerValue::AnalyzerValue() = default;

QSize AnalyzerValue::sizeHint() const {
    return {400, 200};
}

QSize AnalyzerValue::minimumSizeHint() const {
    return {100, 100};
}

void AnalyzerValue::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    const int height = this->height();
    const int width = this->width();

    painter.setPen(palette().light().color());

    qDebug("test");

    QRectF rectangle(0.0, 0.0, width - 1.0, height - 1.0);
    //painter.fillRect(rectangle, QBrush(QColor(0.0, 0.0, 0.0, 0.0)));
    QPen pen(Qt::white, 0.5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));

    painter.fillRect(rectangle, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rectangle);

    //painter.drawRect(rectangle);

    painter.setPen(Qt::red);

    QLineF line(40.0, 40.0, 62.0, 62.0);
    painter.drawLine(line);

    QPointF point(20.0, 20.0);
    painter.drawPoint(point);

    QPointF point2(21.0, 22.0);
    painter.drawPoint(point2);
}
