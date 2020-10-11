#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

class QNetworkAccessManager;
class QNetworkReply;

#include <QObject>

const int HTTP_RESPONSE_OK = 200;

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient();

    static HttpClient *instance();
    static bool validateReplay(QNetworkReply *reply, QJsonDocument &doc);

    QNetworkReply *getCurrentCityLocationRequest();

    QNetworkReply *getCurrentCityIdRequest(QString location);
    /* 获取当日天气信息 */
    QNetworkReply *getTodayCityWeatherRequest(QString cityId);
    /* 获取当日空气质量 */
    QNetworkReply *getTodayCityAirRequest(QString cityId);
    /* 获取当然生活指数信息 */
    QNetworkReply *getTodayCityLifeRequest(QString cityId);
    /* 获取7天预报 */
    QNetworkReply *getFutureCityWeatherRequest(QString cityId);
    /* 获取热门城市信息 */
    QNetworkReply *getTopCityRequest(QString cityNum = "20", QString range = "cn");
    /* 搜索城市 */
    QNetworkReply *getSearchCityRequest(QString cityName, int cityNum = 20, QString range = "cn");

private:
    QString getUrlWithData(QMap<QString, QString> data, QString url);

    Q_DISABLE_COPY(HttpClient)
    QNetworkAccessManager *manager = nullptr;
};

#endif // HTTPCLIENT_H
