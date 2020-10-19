#include <QApplication>
#include <QScreen>
#include <QSettings>
#include "version.h"
#include "AppConfig.h"

QString     AppConfig::appVer = QString("%1.%2.%3").arg(PROJECT_VERSION_MAJOR).arg(PROJECT_VERSION_MINOR).arg(PROJECT_VERSION_PATCH);
QSettings  *AppConfig::setting = nullptr;

QString     AppConfig::apiAppId = "HE2009212252151619";
QString     AppConfig::apiAppKey = "78846d95ed4d4b228fd383582ad5418c";

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
}

QString AppConfig::getAppId()
{
    /* 配置页面删除public id后，直接退出，导致保存了空字符串 */
    QString publicId = AppConfig::getConfig("public_id", QString()).toString();
    return publicId.isEmpty() ? AppConfig::apiAppId : publicId;
}

QString AppConfig::getAppKey()
{
    /* 配置页面删除key后，直接退出，导致保存了空字符串 */
    QString key = AppConfig::getConfig("key", QString()).toString();
    return key.isEmpty() ? AppConfig::apiAppKey : key;
}

QString AppConfig::getUserAppId()
{
    return AppConfig::getConfig("public_id", QString()).toString();
}

void AppConfig::setUserAppId(QString appId)
{
    AppConfig::setConfig("public_id", appId);
}

QString AppConfig::getUserAppKey()
{
    return AppConfig::getConfig("key", QString()).toString();
}

void AppConfig::setUserAppKey(QString appKey)
{
     AppConfig::setConfig("key", appKey);
}

QString AppConfig::getAppVersion()
{
    return AppConfig::appVer;
}

void AppConfig::loadConfig()
{
    /*
    do not need function saveConfig, because QSettings::sync() is called automatically from QSettings's destructor
    and by the event loop at regular intervals, so you normally don't need to call it yourself.
    QSettings::sync() Writes any unsaved changes to permanent storage, and reloads any settings
    that have been changed in the meantime by another application.
    */
    AppConfig::setting = new QSettings();
}

QVariant AppConfig::getConfig(QString key, QVariant defaultValue)
{
    if (nullptr == AppConfig::setting) {
        AppConfig::loadConfig();
    }
    return AppConfig::setting->value(key, defaultValue);
}

void AppConfig::setConfig(QString key, QVariant value)
{
    if (nullptr == AppConfig::setting) {
        AppConfig::loadConfig();
    }
    AppConfig::setting->setValue(key, value);
    AppConfig::setting->sync();
}
