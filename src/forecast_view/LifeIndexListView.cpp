#include <QStandardItem>
#include <QStandardItemModel>
#include <QVariant>
#include <QScrollBar>
#include <QStandardItem>

#include "LifeIndexItemDelegate.h"
#include "LifeIndexListView.h"

LifeIndexListView::LifeIndexListView(QWidget *parent) : QListView(parent)
{
    initUI();
}

void LifeIndexListView::initUI()
{
    setFixedSize(1006, 210);
    setFrameShape(QFrame::NoFrame);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Fixed);
    setMovement(QListView::Static);
    setFlow(QListView::LeftToRight);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSpacing(0);
    setWrapping(true);
    itemModel = new QStandardItemModel();
    setModel(itemModel);
    setItemDelegate(new LifeIndexItemDelegate(this));
    setMouseTracking(true);
}

void LifeIndexListView::setTodayLifeIndexData(const QList<CityLifeIndex>& list)
{
//    int rowCount = itemModel->rowCount();
//    if (rowCount > 0) {
//        itemModel->removeRows(0, rowCount - 1);
//    }
    itemModel->clear();

    for (const auto& lifeIndex : list) {
        auto *item = new QStandardItem();
        item->setData(QVariant::fromValue(lifeIndex), Qt::DisplayRole);
        itemModel->appendRow(item);
    }
    update();
}
