
#include "plot.hpp"

#include <debug.hpp>
#include <qdrag.h>
#include <qmimedata.h>
#include <QPainter>
#include <sstream>

#include "data_inspect/static_data.hpp"

class QDrag;

Plot::Plot(const std::function<double(double)> &getter)
    : _source(std::make_shared<StaticDataSource>(getter)) {

    this->setMouseTracking(this->_should_track_mouse);
    this->setAcceptDrops(true);
}

void Plot::saveAs(const std::string &path) {
    bool const wasTrackingMouse = this->_should_track_mouse;
    this->_should_track_mouse = false;

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    pixmap.save(QString::fromStdString(path));

    this->_should_draw_grid = wasTrackingMouse;
}

void Plot::setDrawGrid(bool const draw) {
    this->_should_draw_grid = draw;
}

void Plot::trackMouse(bool const track) {
    this->_should_track_mouse = track;
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
    stream.precision(_label_precision);

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

    if (!this->_should_track_mouse)
        return;

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    double const maxY = this->_source->getMaxValue();
    double const minY = this->_source->getMinValue();

    double const maxX = this->_source->getMaxRange();
    double const minX = this->_source->getMinRange();

    auto const valXStep = (maxX - minX) / _graph_w;
    auto const valYStep = (maxY - minY) / _graph_h;

    QPoint const mouse = this->mapFromGlobal(QCursor::pos());

    {
        double const x1 = mouse.x();
        double const x2 = x1;

        double const y1 = _graph_offset;
        double const y2 = _graph_h;

        QLineF const cursorX(x1, y1, x2, y2);
        painter->drawLine(cursorX);

        std::string const val = doubleFormat(x1 * valXStep + minX);

        QPointF const point(x2 + _label_x_offset.x(), y2 + _label_x_offset.y());
        painter->drawText(point, val.data());
    }

    {
        double const x1 = _graph_offset;
        double const x2 = _graph_w;

        double const y1 = mouse.y();
        double const y2 = y1;

        QLineF const cursorY(x1, y1, x2, y2);
        painter->drawLine(cursorY);

        std::string const val = doubleFormat((_graph_h - y1) * valYStep + minY);

        QPointF const point(x2 + _label_y_offset.x(), y2 + _label_y_offset.y());
        painter->drawText(point, val.data());
    }
}

void Plot::drawGrid(QPainter *painter) const {

    if (!_should_draw_grid)
        return;

    auto const xStep = _graph_w / static_cast<double>(this->_grid_count);
    auto const yStep = _graph_h / static_cast<double>(this->_grid_count);

    QPen const pen(Qt::white, 0.5, Qt::DotLine);
    painter->setPen(pen);

    double deltaDragModulo = _delta_drag;

    while (deltaDragModulo > xStep)
        deltaDragModulo -= xStep;
    while (deltaDragModulo < 0)
        deltaDragModulo += xStep;

    for (std::size_t i = 1; i < this->_grid_count; i++) {
        double const x1 = static_cast<double>(i) * xStep + _graph_offset - deltaDragModulo;
        double const x2 = x1;

        double const y1 = _graph_offset;
        double const y2 = _graph_h;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }

    // @todo deltaDragModulo Y

    for (std::size_t i = 1; i < this->_grid_count; i++) {
        double const x1 = _graph_offset;
        double const x2 = _graph_w;

        double const y1 = static_cast<double>(i) * yStep + _graph_offset;
        double const y2 = y1;

        QLineF datapoint(x1, y1, x2, y2);
        painter->drawLine(datapoint);
    }
}

void Plot::drawLabels(QPainter *painter) const {

    auto const count = static_cast<double>(this->_grid_count);

    auto const xStep = _graph_w / count;
    auto const yStep = _graph_h / count;

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
    for (std::size_t i = 1; i < this->_grid_count + 1; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = current * xStep + _graph_offset + _label_x_offset.x();

        double const y1 = _graph_h + _label_x_offset.y();

        if ((std::abs(mouse.x() - x1) < 30.0 || std::abs(mouse.x() - x1 - 20.0) < 30.0)
            && this->_should_track_mouse)
            continue;

        std::string const val = doubleFormat(current * valXStep + minX);

        QPointF point(x1, y1);
        painter->drawText(point, val.data());
    }

    for (std::size_t i = 0; i < this->_grid_count; i++) {
        auto const current = static_cast<double>(i);
        double const x1 = _graph_w + _label_y_offset.x();

        double const y1 = (count - current) * yStep + _graph_offset + _label_y_offset.y();

        if (std::abs(mouse.y() - y1) < 20.0 && this->_should_track_mouse)
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

    auto const xStep = _graph_w / pointsCount;
    auto const yStep = _graph_h / (maxY - minY);

    painter->setPen(Qt::white);

    for (std::size_t i = 1; i < values.size(); i++) {
        double const prev = values.at(i - 1);
        double const next = values.at(i);

        auto const step = static_cast<double>(i);

        double const x1 = step * xStep + _graph_offset;
        double const x2 = (step + 1) * xStep + _graph_offset;

        double const y1 = _graph_h - yStep * (prev - minY) + _graph_offset;
        double const y2 = _graph_h - yStep * (next - minY) + _graph_offset;

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

    _widget_h = static_cast<double>(this->height());
    _widget_w = static_cast<double>(this->width());

    _graph_h = _widget_h - _labels_margin - _graph_offset;
    _graph_w = _widget_w - _labels_margin - _graph_offset;

    const QRectF rectangle(0.0, 0.0, _widget_w - 1.0, _widget_h - 1.0);
    const QPen pen(Qt::white, 0.5);

    painter.setPen(pen);

    painter.fillRect(rectangle, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rectangle);

    const QRectF rectangleGraph(_graph_offset, _graph_offset, _graph_w, _graph_h);

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

    double const minX = this->_drag_start_range.x();
    double const maxX = this->_drag_start_range.y();

    double delta = this->_drag_start_pos.x() - cursor.x();
    _delta_drag = delta + this->_delta_drag_save;

    delta /= _graph_w;
    delta *= maxX - minX;

    this->_source->range(minX + delta, maxX + delta);

    this->update();
    event->acceptProposedAction();
}

void Plot::dragEnterEvent(QDragEnterEvent *event) {
    this->_drag_start_pos = this->mapFromGlobal(QCursor::pos());
    this->_drag_start_range = {this->_source->getMinRange(), this->_source->getMaxRange()};
    this->_delta_drag_save = this->_delta_drag;

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
