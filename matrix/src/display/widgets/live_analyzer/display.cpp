
#include <QVBoxLayout>

#include "display.hpp"
#include "value.hpp"

AnalyzerDisplay::AnalyzerDisplay() {
    layout = new QVBoxLayout;

    this->setLayout(layout);
}

AnalyzerValue *AnalyzerDisplay::addAnalyzer(const std::shared_ptr<NetworkObject> &obj) {
    const std::function ptr = [obj]() -> double { return obj->value(); };

    const auto val = new AnalyzerValue(ptr);
    layout->addWidget(val);

    values.push_back(val);

    return val;
}

void AnalyzerDisplay::record() {
    for (const auto value : values) {
        value->recordValue();
    }
}

