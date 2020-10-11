#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QSize>

class QSettings;

class AppConfig : public QObject
{
    Q_OBJECT
public:
    explicit AppConfig(QObject *parent = nullptr);

    static QString  getAppVersion();
    static QString  getAppId();
    static QString  getAppKey();
    static QString  getUserAppId();
    static void     setUserAppId(QString appId);
    static QString  getUserAppKey();
    static void     setUserAppKey(QString appKey);

private:
    static void        loadConfig();
    static QVariant    getConfig(QString key, QVariant defaultValue);
    static void        setConfig(QString key, QVariant value);

    static QSettings    *setting;
    static QString      appVer;

    static QString      apiAppId;
    static QString      apiAppKey;
};

#endif // APPCONFIG_H
