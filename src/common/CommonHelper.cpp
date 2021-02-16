#include <QDateTime>
#include <QMap>
#include <QApplication>
#include <QFile>
#include <QCryptographicHash>
#include <QImage>
#include <QFontMetrics>
#include <QDateTime>
#include <QFontDatabase>
#include "CommonHelper.h"

CommonHelper::CommonHelper(QObject *parent) : QObject(parent)
{
}

bool CommonHelper::setApplicationFont(QString fontPath)
{
    bool result = false;
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    if (false == fontFamily.isEmpty()) {
        QApplication::setFont(QFont(fontFamily));
        result = true;
    }
    return result;
}

void CommonHelper::setStyle(const QString &stylePath)
{
    QFile qss(stylePath);
    if (true == qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
}

QString CommonHelper::getLocalTimestamp()
{
    qint64 timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    return QString::number(timestamp);
}

/*
* 数字签名（sign）方式如下：
* 将请求参数格式化为“key=value”格式，如“k1=v1”、“k2=v2”、“k3=v3”；
* 去除请求参数中值为空的参数
* 去除请求参数中参数为sign 的参数 ( 签名参数不参与签名算法 )
* 将第3步剩余的键值对以字典序升序排列，并用 & 符号连接，如 a=1&b=2&m=3&w=4
* 将第4步得到的字符串后拼接上API密钥, 假设密钥为 abc , 则 结果为: a=1&b=2&m=3&w=4abc
* 将第5步得到的字符串进行MD5加密 ( 注 ：字符集为 UTF-8 )，得到签名结果
* 将第6步得到的签名结果 作为参数添加至请求中，参数名为 sign, 即 sign=xxxxxxx
*/
QString CommonHelper::getSign(QMap<QString, QString> map, QString key)
{
    QStringList mapStrList;
    QMap<QString, QString>::iterator i = map.begin();
    while (i != map.constEnd()) {
        if ( i.value() != " ") {
            mapStrList << i.key() + "=" + i.value();
        }
        ++i;
    }

    QString mapStr = mapStrList.join("&");
    mapStr.append(key);

    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(mapStr.toUtf8());
    return QString(md5.result().toHex());
}

void CommonHelper::ConvertImageColor(QImage &image, const QRgb &rgb)
{
    QVector<QRgb> rgbVector = image.colorTable();
    for (int i = 0; i < rgbVector.size(); ++i) {
        image.setColor(i, rgb);
    }
}

void CommonHelper::initPainter(QPainter &painter, QColor penColor, int fontSize)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(penColor);
    QFont font = painter.font();
    font.setPointSize(fontSize);
    painter.setFont(font);
}

QColor CommonHelper::getAirQualityIndexColor(int aqi)
{
    if (aqi > 301) {
        /* 严重污染 褐红色 */
        return QColor("#880B20");
    } else if (aqi > 201) {
        /* 重度污染 紫色 */
        return QColor("#BA3779");
    } else if (aqi > 151) {
        /* 中度污染 红色 */
        return QColor("#DB555E");
    } else if (aqi > 101) {
        /* 轻度污染 橙色 */
        return QColor("#F29F39");
    } else if (aqi > 51) {
        /* 良 黄色 */
        return QColor("#F9DA65");
    } else if (aqi > 0) {
        /* 优 绿色 */
        return QColor("#9CCA7F");
    } else {
        return QColor(Qt::transparent);
    }
}

QRect CommonHelper::textPaintRect(QFont font, QPoint leftTopPoint, QString text)
{
    QFontMetrics fontMetrics(font);
    QRect textRect = fontMetrics.boundingRect(QRect(), Qt::AlignLeft, text);
    return QRect(leftTopPoint, textRect.size());
}

QString CommonHelper::calLastUpdateTime(QDateTime lastUpdate)
{
    qint64 secs = lastUpdate.secsTo(QDateTime::currentDateTime());
    return QString::number(secs / 60);
}

QRect CommonHelper::adjustTextRect(const QRect &textRect, const QRect &optionRect)
{
    QRect newRect = textRect;
    if (true == optionRect.contains(textRect.bottomRight())) {
        QPoint bottomRight(optionRect.bottomRight().x(), textRect.bottomRight().y());
        newRect = QRect(textRect.topLeft(), bottomRight);
    }
    return newRect;
}

QColor CommonHelper::getWarningLevelColor(const QString &warningLevel)
{
    if (warningLevel == QString::fromLocal8Bit("白色")){
        return QColor("#FFFFFF");
    } else if (warningLevel == QString::fromLocal8Bit("蓝色")){
        return QColor("#00A7FE");
    } else if (warningLevel == QString::fromLocal8Bit("黄色")){
        return QColor("#F6D707");
    } else if (warningLevel == QString::fromLocal8Bit("橙色")){
        return QColor("#EE6B14");
    } else if (warningLevel == QString::fromLocal8Bit("红色")){
        return QColor("#EC311E");
    } else {
        return QColor();
    }
}
