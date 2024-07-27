
#ifndef PLOT_HPP
#define PLOT_HPP

#include <functional>
#include <deque>

#include <QPaintEvent>
#include <QWidget>
#include <QSize>

#include "data/data_source.hpp"

class Plot final : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit Plot(QWidget *parent, const std::function<double(double)> &);

    void saveAs(const std::string &);

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

private:

    void drawGraph(QPainter *painter);
    void drawGrid(QPainter *painter) const;
    void drawLabels(QPainter *painter);
    void drawPoints(QPainter *painter) const;

    std::shared_ptr<DataSource> source;

    bool const shouldDrawGrid = true;

    double const labelsMargin = 60.0;
    double const graphOffset = 2.0;

    std::size_t const gridCount = 10.0;

    double widgetH;
    double widgetW;

    double margin = 1.5;
    double graphH;
    double graphW;
};

#endif //PLOT_HPP
