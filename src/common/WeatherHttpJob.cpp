#include <QTimer>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "WeatherHttpJob.h"
#include "HttpClient.h"
#include "CommonHelper.h"

#include "WeatherWidget.h"
#include "WeatherHttpJob.h"

WeatherHttpJob::WeatherHttpJob(QString cityId,QString cityName, QWidget *parent)
    : QWidget(parent), cityId(cityId), cityName(cityName)
{
    updateTimer = new QTimer();
    if(true == cityId.isEmpty()) {
        getCurrentCityId();
    } else {
        updateWeather();
    }

}

WeatherHttpJob::~WeatherHttpJob()
{
    stopJob();
}

void WeatherHttpJob::updateWeather()
{
    getTodayCityWeather();
    getTodayCityAir();
    getTodayCityLife();
    getFutureCityLife();
}

void WeatherHttpJob::startJob(bool isSingleShot, int interval)
{
    Q_UNUSED(isSingleShot)
    Q_UNUSED(interval)
    updateTimer->stop();
//    if (true == cityId.isEmpty()){

//    }
//    slotGetTodayCityWeather();
//    slotGetFutureCityWeather();
//    if (false == isSingleShot) {
//        connect(updateTimer, &QTimer::timeout, this, &WeatherHttpJob::slotGetTodayCityWeather, Qt::UniqueConnection);
//        updateTimer->setInterval(interval);
//        updateTimer->setSingleShot(isSingleShot);
//        updateTimer->start();
//    }
}

void WeatherHttpJob::stopJob()
{
    updateTimer->stop();
}

void WeatherHttpJob::getCurrentCityId()
{
    QNetworkReply *reply = HttpClient::instance()->getCurrentCityLocationRequest();;
    connect(reply, &QNetworkReply::finished, this, &WeatherHttpJob::slotParseCurrentCityLocation);
}

void WeatherHttpJob::slotParseCurrentCityLocation()
{
    QNetworkReply *reply =  qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if ( false == HttpClient::validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if ( "success" == obj.value("status").toString()) {
        QString latitude = QString::number(obj.value("lat").toDouble());
        QString longitude = QString::number(obj.value("lon").toDouble());
        QString location = longitude + "," + latitude;
        QNetworkReply *reply = HttpClient::instance()->getCurrentCityIdRequest(location);
        connect(reply, &QNetworkReply::finished, this, &WeatherHttpJob::slotParseCurrentCityId);
    }
}

void WeatherHttpJob::slotParseCurrentCityId()
{
    QNetworkReply *reply =  qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if ( false == HttpClient::validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if( 200 != obj.value("code").toString().toInt()){
        return;
    }

   QJsonObject cityObj = obj.value("location").toArray().at(0).toObject();
   cityId = cityObj.value("id").toString();
   cityName = cityObj.value("name").toString();

   WeatherWidget *weatherWidget = qobject_cast<WeatherWidget *>(parent());
   weatherWidget->setCityInfo(cityId, cityName);

   updateWeather();
}

void WeatherHttpJob::getTodayCityWeather()
{
    QNetworkReply *reply = HttpClient::instance()->getTodayCityWeatherRequest(cityId);
    connect(reply, &QNetworkReply::finished, this, &WeatherHttpJob::slotParseTodayWeather);
}

void WeatherHttpJob::slotParseTodayWeather()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if (false == HttpClient::instance()->validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if( 200 != obj.value("code").toString().toInt()){
        return;
    }

    QJsonObject todayOjb = obj.value("now").toObject();

    cityTodayWeather.cityName = cityName;
    cityTodayWeather.cityId = cityId;
    QString updateTimeStr = obj.value("updateTime").toString();
    QDateTime updateDataTime = QDateTime::fromString(updateTimeStr, Qt::ISODate);
    cityTodayWeather.date = updateDataTime.date().toString("yyyy-MM-dd");
    cityTodayWeather.week =  updateDataTime.date().toString("dddd"); //todo QLocale::system()
    cityTodayWeather.updateTime = updateDataTime.toString("yyyy-MM-dd HH:mm:ss");
    cityTodayWeather.temperature = todayOjb.value("temp").toString();
    /* 由7天天气预报接口获取 */
//    cityTodayWeather.temperatureLow = todayOjb.value("tem2").toString();
//    cityTodayWeather.temperatureHigh = todayOjb.value("tem1").toString();
    cityTodayWeather.weatherDesc = todayOjb.value("text").toString();
    cityTodayWeather.weatherId = todayOjb.value("icon").toString();
    cityTodayWeather.windDirection = todayOjb.value("windDir").toString();
    cityTodayWeather.windScale = todayOjb.value("windScale").toString() + QString::fromLocal8Bit("级");
    cityTodayWeather.humidity = todayOjb.value("humidity").toString();
    /* 由空气质量接口获取 */
//    cityTodayWeather.aqi = todayOjb.value("air").toString();
//    cityTodayWeather.aqiLevel = todayOjb.value("air_level").toString();
//    cityTodayWeather.aqiTips = todayOjb.value("air_tips").toString();


//    QJsonObject alarmObj = todayOjb.value("alarm").toObject();
//    cityTodayWeather.alarmType = todayOjb.value("alarm_type").toString();
//    cityTodayWeather.alarmLevel = todayOjb.value("alarm_level").toString();
//    cityTodayWeather.alarmContent = todayOjb.value("alarm_content").toString();

    WeatherWidget *weatherWidget = qobject_cast<WeatherWidget *>(parent());
    weatherWidget->updateTodayCityWeather(cityTodayWeather);
}

void WeatherHttpJob::getTodayCityAir()
{
    QNetworkReply *reply = HttpClient::instance()->getTodayCityAirRequest(cityId);
    connect(reply, &QNetworkReply::finished, this, &WeatherHttpJob::slotParseTodayAir);
}

void WeatherHttpJob::slotParseTodayAir()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if (false == HttpClient::instance()->validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if( 200 != obj.value("code").toString().toInt()){
        return;
    }

    QJsonObject nowOjb = obj.value("now").toObject();
    cityTodayWeather.aqi = nowOjb.value("aqi").toString();
    cityTodayWeather.aqiLevel = nowOjb.value("category").toString();
    cityTodayWeather.primary = nowOjb.value("primary").toString();
    cityTodayWeather.primary = nowOjb.value("primary").toString();
    cityTodayWeather.pm10 = nowOjb.value("pm10").toString();
    cityTodayWeather.pm2p5 = nowOjb.value("pm2p5").toString();
    cityTodayWeather.no2 = nowOjb.value("no2").toString();
    cityTodayWeather.so2 = nowOjb.value("so2").toString();
    cityTodayWeather.co = nowOjb.value("co").toString();
    cityTodayWeather.o3 = nowOjb.value("o3").toString();

    WeatherWidget *weatherWidget = qobject_cast<WeatherWidget *>(parent());
    weatherWidget->updateTodayCityWeather(cityTodayWeather);
}

void WeatherHttpJob::getTodayCityLife()
{
    QNetworkReply *reply = HttpClient::instance()->getTodayCityLifeRequest(cityId);
    connect(reply, &QNetworkReply::finished, this, &WeatherHttpJob::slotParseTodayLife);
}

void WeatherHttpJob::slotParseTodayLife()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if (false == HttpClient::instance()->validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if( 200 != obj.value("code").toString().toInt()){
        return;
    }

    QList<CityLifeIndex> list;
    QJsonArray lifeArray = obj.value("daily").toArray();
    for (int i = 0; i< lifeArray.size(); i++) {
        QJsonObject life = lifeArray.at(i).toObject();
        CityLifeIndex cityTodayLife;
        if (LifeIndex::fitness ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::fitness;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/fitness.svg");
            list << cityTodayLife;
        } else if (LifeIndex::car_washing ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::car_washing;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/car_washing.svg");
            list << cityTodayLife;
        } else if (LifeIndex::clothe ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::clothe;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/clothe.svg");
            list << cityTodayLife;
        } else if (LifeIndex::uv ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::uv;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/uv.svg");
            list << cityTodayLife;
        } else if (LifeIndex::dust ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::dust;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/dust.svg");
            list << cityTodayLife;
        } else if (LifeIndex::comfort ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::comfort;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/comfort.svg");
            list << cityTodayLife;
        } else if (LifeIndex::sick ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::sick;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/sick.svg");
            list << cityTodayLife;
        } else if (LifeIndex::hanger ==  life.value("type").toString().toInt()){
            cityTodayLife.type = LifeIndex::hanger;
            cityTodayLife.title = life.value("name").toString();
            cityTodayLife.level = life.value("category").toString();
            cityTodayLife.desc = life.value("text").toString();
            cityTodayLife.icon = QString(":/weather_icon/weather_icon/hanger.svg");
            list << cityTodayLife;
        }
    }
    WeatherWidget *weatherWidget = qobject_cast<WeatherWidget *>(parent());
    weatherWidget->updateTodayLifyIndex(list);
}

void WeatherHttpJob::getFutureCityLife()
{
    QNetworkReply *reply = HttpClient::instance()->getFutureCityWeatherRequest(cityId);
    connect(reply, &QNetworkReply::finished, this, &WeatherHttpJob::slotParseFutureWeather);
}

void WeatherHttpJob::slotParseFutureWeather()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if (false == HttpClient::instance()->validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if( 200 != obj.value("code").toString().toInt()){
        return;
    }

    QJsonArray dailyArray = obj.value("daily").toArray();
    QJsonObject today = dailyArray.at(0).toObject();
    /* 获取当天的温度范围 */
    cityTodayWeather.temperatureLow = today.value("tempMin").toString();
    cityTodayWeather.temperatureHigh = today.value("tempMax").toString();
    WeatherWidget *weatherWidget = qobject_cast<WeatherWidget *>(parent());
    weatherWidget->updateTodayCityWeather(cityTodayWeather);

    QList<CityFutureWeather> futrueList;
    /* 今天的天气不作为未来天气的预报, 只预报5天 */
    int days = (dailyArray.size() <= 5) ? dailyArray.size() : 5;

    for (int i = 1; i <= days; i++) {
        CityFutureWeather futureWeather;
        QJsonObject json = dailyArray.at(i).toObject();
//        futureWeather.date = json.value("fxDate").toString();
        futureWeather.day = json.value("fxDate").toString();
        QDate data = QDate::fromString(futureWeather.day, Qt::ISODate);
        futureWeather.week =  data.toString("dddd");
        futureWeather.weatherDesc = json.value("textDay").toString();
        futureWeather.weatherId = json.value("iconDay").toString();
        futureWeather.temperatureHigh = json.value("tempMax").toString();
        futureWeather.temperatureLow = json.value("tempMin").toString();
        futureWeather.windSpeed = json.value("windScaleDay").toString();
        futrueList << futureWeather;
    }
    weatherWidget->updateFutureCityWeather(futrueList);
}
