
#include "plot.hpp"

#include <debug.hpp>
#include <qdrag.h>
#include <qmimedata.h>
#include <QPainter>
#include <sstream>

#include "data/static_data.hpp"

class QDrag;

Plot::Plot(QWidget *parent, const std::function<double(double)> &getter)
    : QWidget(parent), source(std::make_shared<StaticDataSource>(getter)) {

    this->setMouseTracking(this->shouldTrackMouse);
    this->setAcceptDrops(true);
}

void Plot::saveAs(const std::string &path) {
    bool const wasTrackingMouse = this->shouldTrackMouse;
    this->shouldTrackMouse = false;

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    pixmap.save(QString::fromStdString(path));

    this->shouldDrawGrid = wasTrackingMouse;
}

void Plot::setDrawGrid(bool const draw) {
    this->shouldDrawGrid = draw;
}

void Plot::trackMouse(bool const track) {
    this->shouldTrackMouse = track;
    this->setMouseTracking(track);
}

QSize Plot::minimumSizeHint() const {
    return {600, 600};
}

QSize Plot::sizeHint() const {
    return {600, 600};
}

std::string Plot::doubleFormat(double const value) const {
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(labelPrecision);

    stream << value;

    return stream.str();
}

void Plot::drawGraph(QPainter *painter) const {
    this->drawGrid(painter);
    this->drawPoints(painter);
    this->drawLabels(painter);
    this->drawCursor(painter);
}

void Plot::drawCursor(QPainter *painter) const {

    if (!this->shouldTrackMouse)
        return;

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    double const maxY = this->source->getMaxValue();
    double const minY = this->source->getMinValue();

    double const maxX = this->source->getMaxRange();
    double const minX = this->source->getMinRange();

    auto const valXStep = (maxX - minX) / graphW;
    auto const valYStep = (maxY - minY) / graphH;

    QPoint const mouse = this->mapFromGlobal(QCursor::pos());

    {
        double const x1 = mouse.x();
        double const x2 = x1;

        double const y1 = graphOffset;
        double const y2 = graphH;

        QLineF const cursorX(x1, y1, x2, y2);
        painter->drawLine(cursorX);

        std::string const val = doubleFormat(x1 * valXStep + minX);

        QPointF const point(x2 + labelXOffset.x(), y2 + labelXOffset.y());
        painter->drawText(point, val.data());
    }

    {
        double const x1 = graphOffset;
        double const x2 = graphW;

        double const y1 = mouse.y();
        double const y2 = y1;

        QLineF const cursorY(x1, y1, x2, y2);
        painter->drawLine(cursorY);

        std::string const val = doubleFormat((graphH - y1) * valYStep + minY);

        QPointF const point(x2 + labelYOffset.x(), y2 + labelYOffset.y());
        painter->drawText(point, val.data());
    }
}

void Plot::drawGrid(QPainter *painter) const {

    if (!shouldDrawGrid)
        return;

    auto const xStep = graphW / static_cast<double>(this->gridCount);
    auto const yStep = graphH / static_cast<double>(this->gridCount);

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    double deltaDragModulo = deltaDrag;

    while (deltaDragModulo > xStep)
        deltaDragModulo -= xStep;
    while (deltaDragModulo < 0)
        deltaDragModulo += xStep;

    for (std::size_t i = 1; i < this->gridCount; i++) {
        double const x1 = static_cast<double>(i) * xStep + graphOffset - deltaDragModulo;
        double const x2 = x1;

        double const y1 = graphOffset;
        double const y2 = graphH;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }

    // @todo deltaDragModulo Y

    for (std::size_t i = 1; i < this->gridCount; i++) {
        double const x1 = graphOffset;
        double const x2 = graphW;

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

    QPoint const mouse = this->mapFromGlobal(QCursor::pos());

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    // @todo deltaDragModulo X & Y
    for (std::size_t i = 1; i < this->gridCount + 1; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = current * xStep + graphOffset + labelXOffset.x();

        double const y1 = graphH + labelXOffset.y();

        if ((std::abs(mouse.x() - x1) < 30.0 || std::abs(mouse.x() - x1 - 20.0) < 30.0)
            && this->shouldTrackMouse)
            continue;

        std::string const val = doubleFormat(current * valXStep + minX);

        QPointF point(x1, y1);
        painter->drawText(point, val.data());
    }

    for (std::size_t i = 0; i < this->gridCount; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = graphW + labelYOffset.x();

        double const y1 = (count - current) * yStep + graphOffset + labelYOffset.y();

        if (std::abs(mouse.y() - y1) < 20.0 && this->shouldTrackMouse)
            continue;

        std::string const val = doubleFormat(current * valYStep + minY);

        QPointF point(x1, y1);
        painter->drawText(point, val.data());
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

void Plot::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    this->update();

    if (!(event->buttons() & Qt::LeftButton))
        return;

    auto *drag = new QDrag(this);
    auto *mimeData = new QMimeData;
    mimeData->setText("");
    drag->setMimeData(mimeData);

    drag->exec(Qt::MoveAction);
}

void Plot::dragMoveEvent(QDragMoveEvent *event) {
    QPoint const cursor = this->mapFromGlobal(QCursor::pos());

    double const minX = this->dragStartRange.x();
    double const maxX = this->dragStartRange.y();

    double delta = this->dragStartPos.x() - cursor.x();
    deltaDrag = delta + this->deltaDragSave;

    delta /= graphW;
    delta *= maxX - minX;

    this->source->range(minX + delta, maxX + delta);

    this->update();
    event->acceptProposedAction();
}

void Plot::dragEnterEvent(QDragEnterEvent *event) {
    this->dragStartPos = this->mapFromGlobal(QCursor::pos());
    this->dragStartRange = {this->source->getMinRange(), this->source->getMaxRange()};
    this->deltaDragSave = this->deltaDrag;

    event->acceptProposedAction();
}

void Plot::wheelEvent(QWheelEvent *event) {
    QPoint const delta = event->angleDelta();

    double const minX = this->source->getMinRange();
    double const maxX = this->source->getMaxRange();

    double deltaRange = (maxX - minX) / 10;

    if (delta.y() > 0)
        deltaRange *= - 1.0;

    // @todo scroll depending on cursor X
    this->source->range(minX - deltaRange, maxX + deltaRange);
    this->update();
}
