
#pragma once


#include <functional>

#include <QPaintEvent>
#include <QWidget>
#include <QSize>

#include "data/data_source.hpp"
#include "data/dynamic_data.hpp"

class AnalyzerValue final : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    AnalyzerValue(const std::function<double()> &);

    enum DisplayMode { points, lines };

    void setDisplayMode(DisplayMode);
    void setMargin(double);
    void recordValue() const;

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

private:

    std::shared_ptr<DynamicDataSource> _source;
    DisplayMode _mode;

    double _margin = 1.5;
};
