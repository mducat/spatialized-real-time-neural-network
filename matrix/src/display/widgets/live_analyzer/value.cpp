
#include <QPainter>
#include <QRectF>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QLineF>

#include <debug.hpp>

#include "value.hpp"

AnalyzerValue::AnalyzerValue(const std::function<double()> &value)
    : valueGetter(value),
      mode(points) {}

void AnalyzerValue::setDisplayMode(const DisplayMode m) {
    mode = m;
}

void AnalyzerValue::recordValue() {
    const double val = this->valueGetter();

    if (val < minY)
        minY = val;

    if (val > maxY)
        maxY = val;

    this->values.push_back(val);

    while (this->values.size() > this->maxValueCount)
        this->values.pop_front();
}

QSize AnalyzerValue::sizeHint() const {
    return {400, 200};
}

QSize AnalyzerValue::minimumSizeHint() const {
    return {400, 100};
}

void AnalyzerValue::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    const auto height = static_cast<double>(this->height());
    const auto width = static_cast<double>(this->width());

    const QRectF rectangle(0.0, 0.0, width - 1.0, height - 1.0);
    const QPen pen(Qt::white, 0.5);

    painter.setPen(pen);

    painter.fillRect(rectangle, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rectangle);

    auto const xStep = width / static_cast<double>(this->maxValueCount);
    auto const yStep = height / (minY - maxY);
    auto const valuesCount = static_cast<double>(this->values.size());
    auto const yOffset = yStep * minY;

    painter.setPen(Qt::white);

    for (std::size_t i = 1; i < this->values.size(); i++) {
        double const prev = this->values.at(i - 1);
        double const next = this->values.at(i);

        auto const step = static_cast<double>(i);

        double const x1 = width - valuesCount * xStep + step * xStep;
        double const x2 = width - valuesCount * xStep + (step + 1) * xStep;

        double const y1 = yStep * prev + yOffset;
        double const y2 = yStep * next + yOffset;

        QPointF point(x1, y1);
        QLineF datapoint(x1, y1, x2, y2);

        switch (mode) {
            case points:
                painter.drawPoint(point);
            break;
            case lines:
                painter.drawLine(datapoint);
            break;
        }
    }
}
