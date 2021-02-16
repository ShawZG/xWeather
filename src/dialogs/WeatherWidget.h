#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include "Global.h"
#include <QWidget>

class FutureCityWeatherListView;
class TodayCityWeatherFrame;
class LifeIndexListView;
class WeatherHttpJob;

class WeatherWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WeatherWidget(QString cityId, QString cityName, QWidget *parent = nullptr);

    void updateTodayCityWeather(CityTodayWeather data);
    void updateTodayLifyIndex(QList<CityLifeIndex> list);
    void updateFutureCityWeather(QList<CityFutureWeather> list);
    void updateWarningCityWeather(QList<CityWarningWeather> list);

    void setCityInfo(const QString &cityId, const QString &cityName);
    inline QString getCityId() {return cityId;}

private:
    void initUI();
    void initConnect();
    void initWeatherHttpJob();

    FutureCityWeatherListView   *futureCityListView = nullptr;
    LifeIndexListView           *lifeIndexListView = nullptr;
    TodayCityWeatherFrame       *todayCityFrame = nullptr;

    WeatherHttpJob              *weatherHttpJob = nullptr;

    QString                     cityId;
    QString                     cityName;
};

#endif // WEATHERWIDGET_H
