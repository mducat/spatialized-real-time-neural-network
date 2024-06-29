
#pragma once

#include <QPaintEvent>
#include <QWidget>
#include <QSize>

class AnalyzerValue final : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    AnalyzerValue();

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

};
