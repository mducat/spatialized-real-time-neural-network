
#include "plot.hpp"

#include <debug.hpp>
#include <qdrag.h>
#include <qmimedata.h>
#include <QPainter>
#include <sstream>

#include "data/static_data.hpp"

class QDrag;

Plot::Plot(QWidget *parent, const std::function<double(double)> &getter)
    : QWidget(parent), _source(std::make_shared<StaticDataSource>(getter)) {

    this->setMouseTracking(this->_shouldTrackMouse);
    this->setAcceptDrops(true);
}

void Plot::saveAs(const std::string &path) {
    bool const wasTrackingMouse = this->_shouldTrackMouse;
    this->_shouldTrackMouse = false;

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    pixmap.save(QString::fromStdString(path));

    this->_shouldDrawGrid = wasTrackingMouse;
}

void Plot::setDrawGrid(bool const draw) {
    this->_shouldDrawGrid = draw;
}

void Plot::trackMouse(bool const track) {
    this->_shouldTrackMouse = track;
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
    stream.precision(_labelPrecision);

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

    if (!this->_shouldTrackMouse)
        return;

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    double const maxY = this->_source->getMaxValue();
    double const minY = this->_source->getMinValue();

    double const maxX = this->_source->getMaxRange();
    double const minX = this->_source->getMinRange();

    auto const valXStep = (maxX - minX) / _graphW;
    auto const valYStep = (maxY - minY) / _graphH;

    QPoint const mouse = this->mapFromGlobal(QCursor::pos());

    {
        double const x1 = mouse.x();
        double const x2 = x1;

        double const y1 = _graphOffset;
        double const y2 = _graphH;

        QLineF const cursorX(x1, y1, x2, y2);
        painter->drawLine(cursorX);

        std::string const val = doubleFormat(x1 * valXStep + minX);

        QPointF const point(x2 + _labelXOffset.x(), y2 + _labelXOffset.y());
        painter->drawText(point, val.data());
    }

    {
        double const x1 = _graphOffset;
        double const x2 = _graphW;

        double const y1 = mouse.y();
        double const y2 = y1;

        QLineF const cursorY(x1, y1, x2, y2);
        painter->drawLine(cursorY);

        std::string const val = doubleFormat((_graphH - y1) * valYStep + minY);

        QPointF const point(x2 + _labelYOffset.x(), y2 + _labelYOffset.y());
        painter->drawText(point, val.data());
    }
}

void Plot::drawGrid(QPainter *painter) const {

    if (!_shouldDrawGrid)
        return;

    auto const xStep = _graphW / static_cast<double>(this->_gridCount);
    auto const yStep = _graphH / static_cast<double>(this->_gridCount);

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    double deltaDragModulo = _deltaDrag;

    while (deltaDragModulo > xStep)
        deltaDragModulo -= xStep;
    while (deltaDragModulo < 0)
        deltaDragModulo += xStep;

    for (std::size_t i = 1; i < this->_gridCount; i++) {
        double const x1 = static_cast<double>(i) * xStep + _graphOffset - deltaDragModulo;
        double const x2 = x1;

        double const y1 = _graphOffset;
        double const y2 = _graphH;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }

    // @todo deltaDragModulo Y

    for (std::size_t i = 1; i < this->_gridCount; i++) {
        double const x1 = _graphOffset;
        double const x2 = _graphW;

        double const y1 = static_cast<double>(i) * yStep + _graphOffset;
        double const y2 = y1;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }
}

void Plot::drawLabels(QPainter *painter) const {

    auto const count = static_cast<double>(this->_gridCount);

    auto const xStep = _graphW / count;
    auto const yStep = _graphH / count;

    double const maxY = this->_source->getMaxValue();
    double const minY = this->_source->getMinValue();

    double const maxX = this->_source->getMaxRange();
    double const minX = this->_source->getMinRange();

    auto const valXStep = (maxX - minX) / count;
    auto const valYStep = (maxY - minY) / count;

    QPoint const mouse = this->mapFromGlobal(QCursor::pos());

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    // @todo deltaDragModulo X & Y
    for (std::size_t i = 1; i < this->_gridCount + 1; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = current * xStep + _graphOffset + _labelXOffset.x();

        double const y1 = _graphH + _labelXOffset.y();

        if ((std::abs(mouse.x() - x1) < 30.0 || std::abs(mouse.x() - x1 - 20.0) < 30.0)
            && this->_shouldTrackMouse)
            continue;

        std::string const val = doubleFormat(current * valXStep + minX);

        QPointF point(x1, y1);
        painter->drawText(point, val.data());
    }

    for (std::size_t i = 0; i < this->_gridCount; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = _graphW + _labelYOffset.x();

        double const y1 = (count - current) * yStep + _graphOffset + _labelYOffset.y();

        if (std::abs(mouse.y() - y1) < 20.0 && this->_shouldTrackMouse)
            continue;

        std::string const val = doubleFormat(current * valYStep + minY);

        QPointF point(x1, y1);
        painter->drawText(point, val.data());
    }
}

void Plot::drawPoints(QPainter *painter) const {
    auto const values = this->_source->getValues();
    auto const pointsCount = static_cast<double>(this->_source->getSize());
    double const maxY = this->_source->getMaxValue();
    double const minY = this->_source->getMinValue();

    auto const xStep = _graphW / pointsCount;
    auto const yStep = _graphH / (maxY - minY);

    painter->setPen(Qt::white);

    for (std::size_t i = 1; i < values.size(); i++) {
        double const prev = values.at(i - 1);
        double const next = values.at(i);

        auto const step = static_cast<double>(i);

        double const x1 = step * xStep + _graphOffset;
        double const x2 = (step + 1) * xStep + _graphOffset;

        double const y1 = _graphH - yStep * (prev - minY) + _graphOffset;
        double const y2 = _graphH - yStep * (next - minY) + _graphOffset;

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

    _widgetH = static_cast<double>(this->height());
    _widgetW = static_cast<double>(this->width());

    _graphH = _widgetH - _labelsMargin - _graphOffset;
    _graphW = _widgetW - _labelsMargin - _graphOffset;

    const QRectF rectangle(0.0, 0.0, _widgetW - 1.0, _widgetH - 1.0);
    const QPen pen(Qt::white, 0.5);

    painter.setPen(pen);

    painter.fillRect(rectangle, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rectangle);

    const QRectF rectangleGraph(_graphOffset, _graphOffset, _graphW, _graphH);

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

    double const minX = this->_dragStartRange.x();
    double const maxX = this->_dragStartRange.y();

    double delta = this->_dragStartPos.x() - cursor.x();
    _deltaDrag = delta + this->_deltaDragSave;

    delta /= _graphW;
    delta *= maxX - minX;

    this->_source->range(minX + delta, maxX + delta);

    this->update();
    event->acceptProposedAction();
}

void Plot::dragEnterEvent(QDragEnterEvent *event) {
    this->_dragStartPos = this->mapFromGlobal(QCursor::pos());
    this->_dragStartRange = {this->_source->getMinRange(), this->_source->getMaxRange()};
    this->_deltaDragSave = this->_deltaDrag;

    event->acceptProposedAction();
}

void Plot::wheelEvent(QWheelEvent *event) {
    QPoint const delta = event->angleDelta();

    double const minX = this->_source->getMinRange();
    double const maxX = this->_source->getMaxRange();

    double deltaRange = (maxX - minX) / 10;

    if (delta.y() > 0)
        deltaRange *= - 1.0;

    // @todo scroll depending on cursor X
    this->_source->range(minX - deltaRange, maxX + deltaRange);
    this->update();
}
