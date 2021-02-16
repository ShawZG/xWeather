#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QObject>
#include <QPainter>
#include <QRgb>

class CommonHelper : public QObject
{
    Q_OBJECT
public:
    explicit CommonHelper(QObject *parent = nullptr);

    static bool setApplicationFont(QString fontPath);
    static QString getLocalTimestamp();
    static QString getSign(QMap<QString, QString> map, QString key);
    static void setStyle(const QString &stylePath);
    static void ConvertImageColor(QImage &image, const QRgb &rgb);
    static QRect textPaintRect(QFont font, QPoint leftTopPoint, QString text);
    static QString calLastUpdateTime(QDateTime lastUpdate);
    static void initPainter(QPainter &painter, QColor penColor, int fontSize);
    static QColor getAirQualityIndexColor(int aqi);
    static QRect adjustTextRect(const QRect &textRect, const QRect &optionRect);
    static QColor getWarningLevelColor(const QString &warningLevel);
};


#endif // COMMONHELPER_H
