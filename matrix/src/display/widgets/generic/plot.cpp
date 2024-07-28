
#include "plot.hpp"

#include <debug.hpp>
#include <QPainter>
#include <sstream>

#include "data/static_data.hpp"

Plot::Plot(QWidget *parent, const std::function<double(double)> &getter)
    : QWidget(parent), source(std::make_shared<StaticDataSource>(getter)) {}

void Plot::saveAs(const std::string &path) {
    QPixmap pixmap(this->size());
    this->render(&pixmap);
    pixmap.save(QString::fromStdString(path));
}

void Plot::setDrawGrid(bool const draw) {
    this->shouldDrawGrid = draw;
}

QSize Plot::minimumSizeHint() const {
    return {600, 600};
}

QSize Plot::sizeHint() const {
    return {600, 600};
}

void Plot::drawGraph(QPainter *painter) const {
    this->drawGrid(painter);
    this->drawPoints(painter);
    this->drawLabels(painter);
}

void Plot::drawGrid(QPainter *painter) const {

    if (!shouldDrawGrid)
        return;

    auto const xStep = graphW / static_cast<double>(this->gridCount);
    auto const yStep = graphH / static_cast<double>(this->gridCount);

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    for (std::size_t i = 1; i < this->gridCount; i++) {
        double const x1 = static_cast<double>(i) * xStep + graphOffset;
        double const x2 = x1;

        double const y1 = graphOffset;
        double const y2 = graphH;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }

    for (std::size_t i = 1; i < this->gridCount; i++) {
        double const x1 = graphOffset;
        double const x2 = graphH;

        double const y1 = static_cast<double>(i) * yStep + graphOffset;
        double const y2 = y1;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }
}

void Plot::drawLabels(QPainter *painter) const {

    auto const count = static_cast<double>(this->gridCount);

    auto const xStep = graphW / count;
    auto const yStep = graphH / count;

    double const maxY = this->source->getMaxValue();
    double const minY = this->source->getMinValue();

    double const maxX = this->source->getMaxRange();
    double const minX = this->source->getMinRange();

    auto const valXStep = (maxX - minX) / count;
    auto const valYStep = (maxY - minY) / count;

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    qDebug() << DISP(minY) << DISP(maxY);

    for (std::size_t i = 1; i < this->gridCount + 1; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = current * xStep + graphOffset - 15.0;

        double const y1 = graphH + 20.0;

        std::stringstream stream;
        stream.setf(std::ios::fixed);
        stream.precision(labelPrecision);

        double value = current * valXStep + minX;
        stream << value;

        QPointF point(x1, y1);
        painter->drawText(point, stream.str().data());
    }

    for (std::size_t i = 0; i < this->gridCount; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = graphW + 10.0;

        double const y1 = (count - current) * yStep + graphOffset + 2.0;

        std::stringstream stream;
        stream.setf(std::ios::fixed);
        stream.precision(labelPrecision);

        double value = current * valYStep + minY;
        stream << value;

        QPointF point(x1, y1);
        painter->drawText(point, stream.str().data());
    }
}

void Plot::drawPoints(QPainter *painter) const {
    auto const values = this->source->getValues();
    auto const pointsCount = static_cast<double>(this->source->getSize());
    double const maxY = this->source->getMaxValue();
    double const minY = this->source->getMinValue();

    auto const xStep = graphW / pointsCount;
    auto const yStep = graphH / (maxY - minY);

    painter->setPen(Qt::white);

    for (std::size_t i = 1; i < values.size(); i++) {
        double const prev = values.at(i - 1);
        double const next = values.at(i);

        auto const step = static_cast<double>(i);

        double const x1 = step * xStep + graphOffset;
        double const x2 = (step + 1) * xStep + graphOffset;

        double const y1 = graphH - yStep * (prev - minY) + graphOffset;
        double const y2 = graphH - yStep * (next - minY) + graphOffset;

        QPointF point(x1, y1);
        QLineF datapoint(x1, y1, x2, y2);

        // painter->drawPoint(point);
        painter->drawLine(datapoint);
        /*switch (mode) {
            case points:
            break;
            case lines:
            break;
        }*/
    }
}

void Plot::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    widgetH = static_cast<double>(this->height());
    widgetW = static_cast<double>(this->width());

    graphH = widgetH - labelsMargin - graphOffset;
    graphW = widgetW - labelsMargin - graphOffset;

    const QRectF rectangle(0.0, 0.0, widgetW - 1.0, widgetH - 1.0);
    const QPen pen(Qt::white, 0.5);

    painter.setPen(pen);

    painter.fillRect(rectangle, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rectangle);

    const QRectF rectangleGraph(graphOffset, graphOffset, graphW, graphH);

    painter.fillRect(rectangleGraph, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rectangleGraph);

    this->drawGraph(&painter);
}
