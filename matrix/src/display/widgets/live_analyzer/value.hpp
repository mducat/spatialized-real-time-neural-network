
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

    void recordValue();

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

private:
    std::function<double()> valueGetter;

    std::size_t maxValueCount = 500;
    std::deque<double> values;

    double minY = -0.5;
    double maxY = 0.5;
};
