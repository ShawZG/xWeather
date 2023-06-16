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
    static void     setUserAppId(const QString& appId);
    static QString  getUserAppKey();
    static void     setUserAppKey(const QString& appKey);

private:
    static void        loadConfig();
    static QVariant    getConfig(const QString& key, const QVariant& defaultValue);
    static void        setConfig(const QString& key, const QVariant& value);

    static QSettings    *setting;
    static QString      appVer;

    static QString      apiAppId;
    static QString      apiAppKey;
};

#endif // APPCONFIG_H
