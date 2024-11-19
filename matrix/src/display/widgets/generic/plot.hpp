
#ifndef PLOT_HPP
#define PLOT_HPP

#include <functional>
#include <deque>

#include <QPaintEvent>
#include <QWidget>
#include <QSize>

#include "data/data_source.hpp"

class StaticDataSource;

class Plot final : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

public:
    explicit Plot(const std::function<double(double)> &);

    void saveAs(const std::string &);
    void setDrawGrid(bool);
    void trackMouse(bool);

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

private:

    [[nodiscard]] std::string doubleFormat(double) const;

    void drawGraph(QPainter *painter) const;
    void drawCursor(QPainter * painter) const;
    void drawGrid(QPainter *painter) const;
    void drawLabels(QPainter *painter) const;
    void drawPoints(QPainter *painter) const;

    std::shared_ptr<StaticDataSource> _source;

    bool _should_draw_grid = true;
    bool _should_track_mouse = true;

    double const _labels_margin = 60.0;
    double const _graph_offset = 2.0;

    std::size_t const _grid_count = 10.0;
    int const _label_precision = 2;

    QPointF _label_x_offset{- 15.0, + 20.0};
    QPointF _label_y_offset{+ 10.0, + 2.0};

    QPoint _drag_start_pos{};
    QPointF _drag_start_range{};
    double _delta_drag = 0;
    double _delta_drag_save = 0;

    double _widget_h = 0;
    double _widget_w = 0;

    double _graph_h = 0;
    double _graph_w = 0;
};

#endif //PLOT_HPP
