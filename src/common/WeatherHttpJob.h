#ifndef WEATHERHTTPJOB_H
#define WEATHERHTTPJOB_H

#include <QWidget>

#include "Global.h"

class QTimer;

class WeatherHttpJob : public QWidget
{
    Q_OBJECT
public:
    explicit WeatherHttpJob(QString cityId, QString cityName, QWidget *parent = nullptr);
    ~WeatherHttpJob();

    void   updateWeather();
    void   startJob(bool isSingleShot = true, int interval = 50 * 60 * 1000);
    void   stopJob();

private slots:
    void slotParseCurrentCityLocation();
    void slotParseCurrentCityId();
    void slotParseTodayWeather();
    void slotParseTodayAir();
    void slotParseTodayLife();
    void slotParseFutureWeather();
    void slotParseWarningWeather();

private:
    void getCurrentCityId();
    void getTodayCityWeather();
    void getTodayCityAir();
    void getTodayCityLife();
    void getFutureCityLife();
    void getWarningCityWeather();

//    void parseTodayCityLifeIndex(const QJsonArray &lifeIndexArray);
//    void parseFutureCityWeather(const QJsonArray &futureArray);

    QTimer      *updateTimer = nullptr;
    QString     cityId;
    QString     cityName;
    CityTodayWeather cityTodayWeather;
};

#endif // WEATHERHTTPJOB_H
