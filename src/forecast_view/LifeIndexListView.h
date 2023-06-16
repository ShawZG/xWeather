#ifndef HOURFORECASTLISTVIEW_H
#define HOURFORECASTLISTVIEW_H

#include "Global.h"
#include <QListView>

class QStandardItemModel;

class LifeIndexListView : public QListView
{
    Q_OBJECT
public:
    explicit LifeIndexListView(QWidget *parent = nullptr);
    void setTodayLifeIndexData(const QList<CityLifeIndex>& list);

private:
    void initUI();

    QStandardItemModel      *itemModel = nullptr;
//    bool                    hasWeatherData = false;
};

#endif // HOURFORECASTLISTVIEW_H
