#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QApplication>
#include <QPalette>
#include "AqiToolTip.h"
#include "CommonHelper.h"

AqiToolTip::AqiToolTip(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AqiToolTip::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // 设置阴影边框;
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(10);
    setGraphicsEffect(shadowEffect);

    setFixedSize(200, 200);
}

void AqiToolTip::setAqiData(CityAqi cityAqiData)
{
    aqiData = cityAqiData;
}

void AqiToolTip::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintBackground();
    paintForeground();
}

void AqiToolTip::paintBackground()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QApplication::palette(this).alternateBase());
//    painter.setBrush(QColor(66, 66, 66));

    /* 三角形坐标 */
    int TRIANGLE_WIDTH = 40;
    int TRIANGLE_HEIGHT = 20;
    QPoint trianglePoint1(width() / 2, height());
    QPoint trianglePoint2((width() - TRIANGLE_WIDTH) / 2, height() - TRIANGLE_HEIGHT);
    QPoint trianglePoint3((width() + TRIANGLE_WIDTH) / 2, height() - TRIANGLE_HEIGHT);
    QPolygon trianglePolygon;
    trianglePolygon << trianglePoint1 << trianglePoint2 << trianglePoint3;
    painter.drawPolygon(trianglePolygon);

    /* 圆角矩形 */
    int PADDING = 2;
    painter.drawRoundedRect(rect().x() + PADDING, rect().y() + PADDING,
                            rect().width() - PADDING * 2, rect().height() - TRIANGLE_HEIGHT,
                            8, 8);
}

void AqiToolTip::paintForeground()
{
    int LINE_MARGIN = 10;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    CommonHelper::initPainter(painter, QColor("white"), 15);
    /* 主要污染物 */
    QString primaryStr = QString("主要污染物: %1").arg(aqiData.primary);
    QPoint primaryPoint(15, LINE_MARGIN);
    QRect primaryRect = CommonHelper::textPaintRect(painter.font(), primaryPoint, primaryStr);
    painter.drawText(primaryRect, primaryStr);
    /* PM10 */
    QString pm10Str = QString("PM10: %1").arg(aqiData.pm10);
    QPoint pm10Point(primaryRect.bottomLeft().x(), primaryRect.bottomLeft().y() + LINE_MARGIN);
    QRect pm10Rect = CommonHelper::textPaintRect(painter.font(), pm10Point, pm10Str);
    painter.drawText(pm10Rect, pm10Str);
    /* PM2.5 */
    QString pm2p5Str = QString("PM2.5: %1").arg(aqiData.pm2p5);
    QPoint pm2p5Point(pm10Rect.bottomLeft().x(), pm10Rect.bottomLeft().y() + LINE_MARGIN);
    QRect pm2p5Rect = CommonHelper::textPaintRect(painter.font(), pm2p5Point, pm2p5Str);
    painter.drawText(pm2p5Rect, pm2p5Str);
    /* 二氧化氮 */
    QString no2Str = QString("二氧化氮(NO2): %1").arg(aqiData.no2);
    QPoint no2Point(pm2p5Rect.bottomLeft().x(), pm2p5Rect.bottomLeft().y() + LINE_MARGIN);
    QRect no2Rect = CommonHelper::textPaintRect(painter.font(), no2Point, no2Str);
    painter.drawText(no2Rect, no2Str);
    /* 二氧化硫 */
    QString so2Str = QString("二氧化硫(SO2): %1").arg(aqiData.so2);
    QPoint so2Point(no2Rect.bottomLeft().x(), no2Rect.bottomLeft().y() + LINE_MARGIN);
    QRect so2Rect = CommonHelper::textPaintRect(painter.font(), so2Point, so2Str);
    painter.drawText(so2Rect, so2Str);
    /* 一氧化碳 */
    QString coStr = QString("一氧化碳(CO): %1").arg(aqiData.co);
    QPoint coPoint(so2Rect.bottomLeft().x(), so2Rect.bottomLeft().y() + LINE_MARGIN);
    QRect coRect = CommonHelper::textPaintRect(painter.font(), coPoint, coStr);
    painter.drawText(coRect, coStr);
}

