
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
    explicit Plot(QWidget *parent, const std::function<double(double)> &);

    void saveAs(const std::string &);
    void setDrawGrid(bool);
    void trackMouse(bool);

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

private:

    std::string doubleFormat(double) const;

    void drawGraph(QPainter *painter) const;
    void drawCursor(QPainter * painter) const;
    void drawGrid(QPainter *painter) const;
    void drawLabels(QPainter *painter) const;
    void drawPoints(QPainter *painter) const;

    std::shared_ptr<StaticDataSource> _source;

    bool _shouldDrawGrid = true;
    bool _shouldTrackMouse = true;

    double const _labelsMargin = 60.0;
    double const _graphOffset = 2.0;

    std::size_t const _gridCount = 10.0;
    int const _labelPrecision = 2;

    QPointF _labelXOffset{- 15.0, + 20.0};
    QPointF _labelYOffset{+ 10.0, + 2.0};

    QPoint _dragStartPos{};
    QPointF _dragStartRange{};
    double _deltaDrag = 0;
    double _deltaDragSave = 0;

    double _widgetH = 0;
    double _widgetW = 0;

    double _graphH = 0;
    double _graphW = 0;
};

#endif //PLOT_HPP
