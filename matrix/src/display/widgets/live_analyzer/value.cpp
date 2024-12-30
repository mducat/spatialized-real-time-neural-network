
#include <QPainter>
#include <QRectF>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QLineF>

#include "value.hpp"

#include "data/dynamic_data.hpp"

AnalyzerValue::AnalyzerValue(const std::function<double()> &value)
    : _source(std::make_shared<DynamicDataSource>(value)),
      _mode(points) {}

void AnalyzerValue::setDisplayMode(const DisplayMode m) {
    _mode = m;
}

void AnalyzerValue::setMargin(double const m) {
    _margin = m;
}

void AnalyzerValue::recordValue() {
    this->_source->recordValue();
    this->repaint();
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

    auto const values = this->_source->getValues();
    double const maxY = this->_source->getMaxValue();
    double const minY = this->_source->getMinValue();

    auto const xStep = width / static_cast<double>(this->_source->getSize());
    auto const yStep = height / ((maxY - minY) * _margin);
    auto const valuesCount = static_cast<double>(values.size());
    // auto const yOffset = yStep * std::abs(minY) * margin;

    painter.setPen(Qt::white);

    for (std::size_t i = 1; i < values.size(); i++) {
        double const prev = values.at(i - 1);
        double const next = values.at(i);

        auto const step = static_cast<double>(i);

        double const x1 = width - valuesCount * xStep + step * xStep;
        double const x2 = width - valuesCount * xStep + (step + 1) * xStep;

        double const y1 = height - yStep * (prev - minY);
        double const y2 = height - yStep * (next - minY);

        QPointF point(x1, y1);
        QLineF datapoint(x1, y1, x2, y2);

        switch (_mode) {
            case points:
                painter.drawPoint(point);
            break;
            case lines:
                painter.drawLine(datapoint);
            break;
        }
    }
}
