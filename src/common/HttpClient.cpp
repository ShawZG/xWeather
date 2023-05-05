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
    delete manager;
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
    QJsonParseError jsonError{};
    doc = QJsonDocument::fromJson(jsonByte, &jsonError);
    if (doc.isNull()) {
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
    QString url = QString("https://api.ipbase.com/v2/info");
    return manager->get(QNetworkRequest(url));
}

QString HttpClient::getUrlWithData(QMap<QString, QString> data, const QString &url)
{
#ifdef QT_DEBUG
    QMap<QString, QString> params = data;
    params["key"] = AppConfig::getAppKey();
    QStringList paramsStrList;
    for(const auto& key : params.keys()) {
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

QNetworkReply *HttpClient::getCurrentCityIdRequest(const QString &location)
{
    QMap<QString, QString> params;
    params["location"] = location;
    params["number"] = QString("1");
    QString url = getUrlWithData(params, "https://geoapi.qweather.com/v2/city/lookup");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTodayCityWeatherRequest(const QString &cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.qweather.com/v7/weather/now");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTodayCityAirRequest(const QString &cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.qweather.com/v7/air/now");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTodayCityLifeRequest(const QString &cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    params["type"] = "0";
    QString url = getUrlWithData(params, "https://devapi.qweather.com/v7/indices/1d");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getFutureCityWeatherRequest(const QString &cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.qweather.com/v7/weather/7d");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getTopCityRequest(const QString &cityNum, const QString &range)
{
    QMap<QString, QString> params;
    params["range"] = range;
    params["number"] = cityNum;
    QString url = getUrlWithData(params, "https://geoapi.qweather.com/v2/city/top");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getSearchCityRequest(const QString &cityName, int cityNum, const QString &range)
{
    QMap<QString, QString> params;
    params["location"] = cityName;
    params["range"] = range;
    params["number"] = QString::number(cityNum);
    QString url = getUrlWithData(params, "https://geoapi.qweather.com/v2/city/lookup");
    return manager->get(QNetworkRequest(url));
}

QNetworkReply *HttpClient::getWarningCityWeatherRequest(const QString &cityId)
{
    QMap<QString, QString> params;
    params["location"] = cityId;
    QString url = getUrlWithData(params, "https://devapi.qweather.com/v7/warning/now");
    return manager->get(QNetworkRequest(url));
}
