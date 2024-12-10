
#pragma once

#include <qtreeview.h>
#include <qtreewidget.h>
#include <QWidget>

class Object;
class Layer;

class ObjectList final : public QTreeWidget
{
    Q_OBJECT

public:
    explicit ObjectList(std::shared_ptr<Layer> const & layer);

    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void objectSelected(std::shared_ptr<Object> const &);

private:

    std::shared_ptr<Layer> _layer;

    void init();
};
