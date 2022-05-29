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

    QNetworkReply *getCurrentCityIdRequest(const QString &location);
    /* 获取当日天气信息 */
    QNetworkReply *getTodayCityWeatherRequest(const QString &cityId);
    /* 获取当日空气质量 */
    QNetworkReply *getTodayCityAirRequest(const QString &cityId);
    /* 获取当然生活指数信息 */
    QNetworkReply *getTodayCityLifeRequest(const QString &cityId);
    /* 获取7天预报 */
    QNetworkReply *getFutureCityWeatherRequest(const QString &cityId);
    /* 获取热门城市信息 */
    QNetworkReply *getTopCityRequest(const QString &cityNum = "20", const QString &range = "cn");
    /* 搜索城市 */
    QNetworkReply *getSearchCityRequest(const QString &cityName, int cityNum = 20, const QString &range = "cn");
    /* 获取极端天气信息 */
    QNetworkReply *getWarningCityWeatherRequest(const QString &cityId);
private:
    QString getUrlWithData(QMap<QString, QString> data, const QString &url);

    Q_DISABLE_COPY(HttpClient)
    QNetworkAccessManager *manager = nullptr;
};

#endif // HTTPCLIENT_H
