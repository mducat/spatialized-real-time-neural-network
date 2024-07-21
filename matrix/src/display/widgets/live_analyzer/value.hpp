
#pragma once


#include <functional>
#include <deque>

#include <QPaintEvent>
#include <QWidget>
#include <QSize>

#include <network_object.hpp>

class AnalyzerValue final : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit AnalyzerValue(const std::function<double()> &);

    enum DisplayMode { points, lines };

    void setDisplayMode(DisplayMode);
    void setMargin(double);
    void recordValue();

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

private:
    std::function<double()> valueGetter;

    DisplayMode mode;

    std::size_t maxValueCount = 500;
    std::deque<double> values;

    double minY = -1.0;
    double maxY = 1.0;

    double margin = 1.5;
};
