
#pragma once

#include <network_object.hpp>
#include <QWidget>
#include <QVBoxLayout>

class AnalyzerValue;

class AnalyzerDisplay final : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyzerDisplay(QWidget *);

    AnalyzerValue *addAnalyzer(const std::shared_ptr<NetworkObject> &);

    void record();

private:

    QVBoxLayout *layout = nullptr;
    std::vector<AnalyzerValue *> values;
};
