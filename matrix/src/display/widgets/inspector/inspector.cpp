
#include "inspector.hpp"

#include <layer.hpp>
#include <network_object.hpp>
#include <object.hpp>
#include <qformlayout.h>
#include <QLabel>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <value.hpp>

Inspector::Inspector(std::shared_ptr<Layer> const & layer) : _layer(layer) {
    init();
}

void Inspector::selectObject(std::shared_ptr<Object> const &obj) {
    switch (obj->getDestinationLayer()) {
        case LayerType::ANY:
            break;
        case LayerType::NETWORK:
            this->inspectNetworkObject(std::static_pointer_cast<NetworkObject>(obj));
            break;
        case LayerType::CNS:
            break;
        case LayerType::CELL:
            break;
        case LayerType::DATA:
            break;
        case LayerType::UNDEF:
            break;
    }
}

void Inspector::init() {
    this->_layout = new QVBoxLayout;
    this->setLayout(this->_layout);
    this->_layout->setContentsMargins(0,0,0,0);

    auto pal = QPalette();

    pal.setColor(QPalette::Window, QColor(35, 35, 35, 255));

    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void static clearLayout(QLayout* layout) // NOLINT(*-no-recursion)
{
    while (true)
    {
        if (!layout->count())
            return;

        QLayoutItem* item = layout->takeAt(0);

        if (!item)
            return;

        if (QWidget* widget = item->widget())
            widget->deleteLater();

        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout);

        delete item;
    }
}

void Inspector::inspectNetworkObject(std::shared_ptr<NetworkObject> const &obj) {
    clearLayout(_layout);
    this->_form_layout = new QFormLayout;
    this->_form_layout->setContentsMargins(10,5,10,10);

    QString const name = QString::fromStdString("OBJ: " + obj->getObjectName());

    auto *test = new QLineEdit(name);
    auto *label = new QLabel(name);

    QPalette pal = QPalette();

    pal.setColor(QPalette::Window, QColor(60, 60, 60, 255));
    label->setAutoFillBackground(true);
    label->setPalette(pal);

    auto *live = new QPushButton("Inspect live");
    connect(live, &QPushButton::clicked, [&] { // NOLINT(*-unused-return-value)
        std::function const rec = [obj] () -> double { return obj->value(); };
        auto *val = new AnalyzerValue(rec);

        val->setDisplayMode(AnalyzerValue::lines);

        this->_layer->addCallback([val] {
            val->recordValue();
        });

        val->show();
    });

    this->_layout->addWidget(label);
    this->_layout->addLayout(this->_form_layout);
    this->_form_layout->addRow(tr("Test"), test);
    this->_form_layout->addWidget(live);

    this->_layout->addStretch();
}
