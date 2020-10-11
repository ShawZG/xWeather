#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QScopedPointer>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QTextCodec>

#include "CommonHelper.h"
#include "AppConfig.h"
#include "HttpClient.h"

Q_GLOBAL_STATIC(HttpClient, httpClient)

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{
    if (nullptr != manager) {
        delete manager;
    }
}

HttpClient *HttpClient::instance()
{
    return httpClient;
}

bool HttpClient::validateReplay(QNetworkReply *reply, QJsonDocument &doc)
{
    if (QNetworkReply::NoError != reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->request().url().url();
        qDebug() << reply->errorString();
#endif
        return false;
    }
    QByteArray jsonByte = reply->readAll();
#ifdef QT_DEBUG
    qDebug() << reply->request().url().url();
    qDebug() << QString(jsonByte);
#endif
    QJsonParseError jsonError;
    doc = QJsonDocument::fromJson(jsonByte, &jsonError);
    if ( true == doc.isNull()) {
#ifdef QT_DEBUG
        qDebug() << reply->request().url().url();
        qDebug() << jsonError.errorString();
#endif
        return false;
    } else {
        return true;
    }
}

QNetworkReply *HttpClient::getCurrentCityLocationRequest()
{
    QString url = QString("http://ip-api.com/json/");
    return manager->get(QNetworkRequest(url));
}

QString HttpClient::getUrlWithData(QMap<QString, QString> data, QString url)
{
#ifdef QT_DEBUG
    QMap<QString, QString> params = data;
    params["key"] = AppConfig::getAppKey();
    QStringList paramsStrList;
    for(auto key : params.keys()) {
        paramsStrList << key + "=" + params.value(key);
    }
    QString paramsStr = paramsStrList.join("&");
    return (url + "?" + paramsStr);
#else
    QMap<QString, QString> params = data;
    params["publicid"] = AppConfig::getAppId();
    params["t"] = CommonHelper::getLocalTimestamp();
    QStringList paramsStrList;
    for(auto key : params.keys()) {
        paramsStrList << key + "=" + params.value(key);
    }
    paramsStrList << QString("sign=%1").arg(CommonHelper::getSign(params, AppConfig::getAppKey()));
    QString paramsStr = paramsStrList.join("&");
    return (url + "?" + paramsStr);
#endif
}

QNetworkReply *HttpClient::getCurrentCityIdRequest(QString location)
{
    QMap<QString, QString> params;
    params["location"] = location;
    params["number"] = QString("1");
    QString url = getUrlWithData(params, "https://geoapi.heweather.net/v2/city/lookup");
    return manager->get(QNetworkRequest(url));

//    QString url = QString("https://geoapi.heweather.net/v2/city/lookup?location=%1&number=1&key=%2")
//            .arg(location).arg(AppConfig::getAppKey());
//    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTodayCityWeatherRequest(QString cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.heweather.net/v7/weather/now");
    return manager->get(QNetworkRequest(url));

//    QString url = QString("https://devapi.heweather.net/v7/weather/now?location=%1&key=%2")
//                  .arg(cityId).arg(AppConfig::getAppKey());
//    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTodayCityAirRequest(QString cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.heweather.net/v7/air/now");
    return manager->get(QNetworkRequest(url));

//    QString url = QString("https://devapi.heweather.net/v7/air/now?location=%1&key=%2")
//            .arg(cityId).arg(AppConfig::getAppKey());
//    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTodayCityLifeRequest(QString cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    params["type"] = "0";
    QString url = getUrlWithData(params, "https://devapi.heweather.net/v7/indices/1d");
    return manager->get(QNetworkRequest(url));

//    QString url = QString("https://devapi.heweather.net/v7/indices/1d?location=%1&type=0&key=%2")
//            .arg(cityId).arg(AppConfig::getAppKey());
//    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getFutureCityWeatherRequest(QString cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.heweather.net/v7/weather/7d");
    return manager->get(QNetworkRequest(url));

//    QString url = QString("https://devapi.heweather.net/v7/weather/3d?location=%1&key=%2")
//                  .arg(cityId).arg(AppConfig::getAppKey());
//    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTopCityRequest(QString cityNum, QString range)
{
    QMap<QString, QString> params;
    params["range"] = range;
    params["number"] = cityNum;
    QString url = getUrlWithData(params, "https://geoapi.heweather.net/v2/city/top");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getSearchCityRequest(QString cityName, int cityNum, QString range)
{
    QMap<QString, QString> params;
    params["location"] = cityName;
    params["range"] = range;
    params["number"] = QString::number(cityNum);
    QString url = getUrlWithData(params, "https://geoapi.heweather.net/v2/city/lookup");
    return manager->get(QNetworkRequest(url));
}
