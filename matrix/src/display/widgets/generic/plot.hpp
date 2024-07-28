
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

    std::shared_ptr<StaticDataSource> source;

    bool shouldDrawGrid = true;
    bool shouldTrackMouse = true;

    double const labelsMargin = 60.0;
    double const graphOffset = 2.0;

    std::size_t const gridCount = 10.0;
    int const labelPrecision = 2;

    QPointF labelXOffset{- 15.0, + 20.0};
    QPointF labelYOffset{+ 10.0, + 2.0};

    QPoint dragStartPos{};
    QPointF dragStartRange{};
    double deltaDrag = 0;
    double deltaDragSave = 0;

    double widgetH = 0;
    double widgetW = 0;

    double graphH = 0;
    double graphW = 0;
};

#endif //PLOT_HPP
