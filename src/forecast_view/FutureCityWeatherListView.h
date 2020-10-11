#ifndef FUTURECITYWEATHERLISTVIEW_H
#define FUTURECITYWEATHERLISTVIEW_H

#include "Global.h"
#include <QListView>

class QStandardItemModel;

class FutureCityWeatherListView: public QListView
{
    Q_OBJECT
public:
    explicit FutureCityWeatherListView(QWidget *parent = nullptr);
    void seFutureCityWeatherData(QList<CityFutureWeather> list);

private:
    void initUI();

    QStandardItemModel      *itemModel = nullptr;
    bool                    hasWeatherData = false;
};

#endif // FUTURECITYWEATHERLISTVIEW_H
