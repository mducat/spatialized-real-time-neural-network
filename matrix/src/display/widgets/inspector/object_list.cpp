
#include <ranges>

#include "object_list.hpp"

#include <layer.hpp>
#include <object.hpp>
#include <qtreewidget.h>

ObjectList::ObjectList(std::shared_ptr<Layer> const & layer) : _layer(layer) {
    init();
}


void ObjectList::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QTreeWidget::selectionChanged(selected, deselected);

    QModelIndexList selectedIndexes = selected.indexes();
    if (selectedIndexes.isEmpty() || selectedIndexes.size() > 1) {
        return;
    }

    int const rowIndex = selectedIndexes.first().row();
    QTreeWidgetItem const *item = this->topLevelItem(rowIndex);

    if (!item) {
        qFatal("ObjectList::selectionChanged: item is null");
    }

    auto const objectId = item->data(0, Qt::UserRole).value<std::size_t>();
    std::shared_ptr<Object> const &object = this->_layer->getObjectById(objectId);

    emit objectSelected(object);
}

void ObjectList::init() {
    this->setColumnCount(1);

    QStringList header;
    header << "Layer objects" ;

    this->setHeaderLabels(header);

    for (const auto& object : this->_layer->getObjects() | std::views::values) {
        QStringList item;
        item << QString::fromStdString(object->getObjectName());

        // ReSharper disable once CppDFAMemoryLeak
        const auto top = new QTreeWidgetItem(item);
        QVariant value;
        value.setValue(object->getObjectId());
        top->setData(0, Qt::UserRole, value);

        this->addTopLevelItem(top);
    }

    this->show();
}
