
#include <QVBoxLayout>

#include "display.hpp"
#include "value.hpp"

AnalyzerDisplay::AnalyzerDisplay() {
    _layout = new QVBoxLayout;

    this->setLayout(_layout);
}

AnalyzerValue *AnalyzerDisplay::addAnalyzer(const std::shared_ptr<NetworkObject> &obj) {
    const std::function ptr = [obj]() -> double { return obj->value(); };

    const auto val = new AnalyzerValue(ptr);
    _layout->addWidget(val);

    _values.push_back(val);

    return val;
}

void AnalyzerDisplay::record() {
    for (const auto value : _values) {
        value->recordValue();
    }
}

