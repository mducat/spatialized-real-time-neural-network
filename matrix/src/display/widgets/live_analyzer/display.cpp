
#include <QVBoxLayout>

#include "display.hpp"
#include "value.hpp"

AnalyzerDisplay::AnalyzerDisplay() {
    const auto layout = new QVBoxLayout;

    const auto val = new AnalyzerValue;
    layout->addWidget(val);

    this->setLayout(layout);
}
