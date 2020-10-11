#include <QStandardItem>
#include <QStandardItemModel>
#include <QVariant>
#include <QScrollBar>
#include <QStandardItem>

#include "FutureWeatherItemDelegate.h"
#include "FutureCityWeatherListView.h"

FutureCityWeatherListView::FutureCityWeatherListView(QWidget *parent) : QListView(parent)
{
    initUI();
}


void FutureCityWeatherListView::initUI()
{
    setFixedSize(506, 320);
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
    setItemDelegate(new FutureWeatherItemDelegate(this));
    setMouseTracking(true);
}

void FutureCityWeatherListView::seFutureCityWeatherData(QList<CityFutureWeather> list)
{
    itemModel->clear();
    for (auto lifeIndex : list) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(lifeIndex), Qt::DisplayRole);
        itemModel->appendRow(item);
    }
    update();
}
