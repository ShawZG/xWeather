#include <QDateTime>
#include <QMouseEvent>
#include <QDebug>
#include "AqiToolTip.h"
#include "WarningToolTip.h"
#include "CommonHelper.h"
#include "SignalManager.h"
#include "TodayCityWeatherFrame.h"

TodayCityWeatherFrame::TodayCityWeatherFrame(QWidget *parent) : QFrame(parent)
{
    setMouseTracking(true);
    aqiToolTip = new AqiToolTip(this);
    aqiToolTip->hide();

    warningToolTip = new WarningToolTip(this);
    warningToolTip->hide();

    initConnect();
}

void TodayCityWeatherFrame::initConnect()
{
    connect(this, &TodayCityWeatherFrame::sigAqiToolTipVisible, this, &TodayCityWeatherFrame::slotAqiToolTipVisible);
    connect(this, &TodayCityWeatherFrame::sigWarningToolTipVisible, this, &TodayCityWeatherFrame::slotWarningToolTipVisible);
}

void TodayCityWeatherFrame::setTodayCityWeatherData(CityTodayWeather data)
{
    hasWeatherData = true;
    cityTodayWeather = data;
    update();
}

void TodayCityWeatherFrame::setWarningCityWeather(const QList<CityWarningWeather> &list)
{
    cityWarningInfoList.clear();
    cityWarningRectList.clear();
    cityWarningInfoList = list;
}

void TodayCityWeatherFrame::mousePressEvent(QMouseEvent *event)
{
    isRefreshButtonPressed = refreshButtonRect.contains(event->pos());
    isLocationButtonPressed = locationButtonRect.contains(event->pos());
}

void TodayCityWeatherFrame::mouseReleaseEvent(QMouseEvent *event)
{
    isRefreshButtonReleased = refreshButtonRect.contains(event->pos());
    if (isRefreshButtonPressed && isRefreshButtonReleased){
        emit sigUpdateWeather();
    }
    isRefreshButtonPressed = false;
    isRefreshButtonReleased = false;

    isLocationButtonReleased = locationButtonRect.contains(event->pos());
    if (isLocationButtonPressed && isLocationButtonReleased){
        emit SignalManager::instance()->sigShowAddCityDialog();
    }
    isLocationButtonPressed = false;
    isLocationButtonReleased = false;
}

void TodayCityWeatherFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (isRefreshButtonHover != refreshButtonRect.contains(event->pos())){
        isRefreshButtonHover = !isRefreshButtonHover;
        update();
    }
    if (isLocationButtonHover != locationButtonRect.contains(event->pos())){
        isLocationButtonHover = !isLocationButtonHover;
        update();
    }
    if (isAqiToolTipLabelHover != aqiToolTipLabelRect.contains(event->pos())){
        isAqiToolTipLabelHover = !isAqiToolTipLabelHover;
        emit sigAqiToolTipVisible(isAqiToolTipLabelHover);
    }

    bool isOneRectHover = false;
    int  hoverRectIndex = -1;
    for (int i = 0; i < cityWarningRectList.size(); i++) {
        if (cityWarningRectList.at(i).contains(event->pos())) {
            isOneRectHover = true;
            hoverRectIndex = i;
            break;
        }
    }
    if (isWarningRectHover != isOneRectHover) {
        isWarningRectHover = !isWarningRectHover;
        emit sigWarningToolTipVisible(isWarningRectHover, hoverRectIndex);
    }
}

void TodayCityWeatherFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if (hasWeatherData) {
        paintLocationDate();
        paintUpdateTime();
        paintWarningInfo();
        paintMajorWeather();
        paintMinorWeather();
    }
}

void TodayCityWeatherFrame::paintLocationDate()
{
    QPainter painter(this);
    CommonHelper::initPainter(painter, QColor("white"), 20);
    /* 绘制坐标点 */
    QString imagePath = isLocationButtonHover ? ":/weather_icon/weather_icon/add_city.svg" : ":/weather_icon/weather_icon/location.svg";
    locationButtonRect = QRect(16, 16, 28, 28);
    painter.drawImage(locationButtonRect, QImage(imagePath));
    /* 绘制城市，日期 */
    QString str = QString("%1 %2 %3").arg(cityTodayWeather.cityName, cityTodayWeather.date, cityTodayWeather.week);
    QPoint textPoint(locationButtonRect.topRight().x() + 10, locationButtonRect.topRight().y());
    QRect textRect = CommonHelper::textPaintRect(painter.font(), textPoint, str);
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignCenter, str);
}

void TodayCityWeatherFrame::paintUpdateTime()
{
    QDateTime update = QDateTime::fromString(cityTodayWeather.updateTime, "yyyy-MM-dd HH:mm:ss");
    QString minutes = CommonHelper::calLastUpdateTime(update);
    QString updateStr = QString("%1").arg(minutes) + QString::fromLocal8Bit("分钟前发布");

    QPainter painter(this);
    CommonHelper::initPainter(painter, QColor("white"), 13);
    QPoint textPoint(16 + 32 + 4, 20 + 32 + 6);
    QRect textRect = CommonHelper::textPaintRect(painter.font(), textPoint, updateStr);
    painter.drawText(textRect, updateStr);

    /* 绘制刷新按钮 */
    int buttonSize = isRefreshButtonHover ? 32 : 24;
    int xMargin = 20 - (buttonSize - 20) / 2;
    QPoint buttonPoint = textRect.topRight() + QPoint(xMargin, (textRect.height() - buttonSize) / 2);
    refreshButtonRect = QRect(buttonPoint, QSize(buttonSize, buttonSize));
    painter.drawImage(refreshButtonRect, QImage(":/weather_icon/weather_icon/refresh.svg"));

    //根据天气更新时间的坐标，得到预警天气信息的绘制开始的坐标
    warningBeginPoint = QPoint(textRect.bottomRight().x() + 64, textRect.bottomRight().y() - 4);
}

void TodayCityWeatherFrame::paintWarningInfo()
{
    if (cityWarningInfoList.empty()) {
            return;
    }
    cityWarningRectList.clear();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width(), warningBeginPoint.y());
    QFontMetrics fontMetrics = painter.fontMetrics();
    int height = fontMetrics.height();
    int xMargin = 16;
    QRect preWarningRect(this->width(), 0, 0 ,0);
    for (int i = 0; i < cityWarningInfoList.size(); i++) {
        QString warningTypeName = cityWarningInfoList.at(i).typeName;
        int width = fontMetrics.horizontalAdvance(warningTypeName);
        QRect warningBackgroundRect = QRect(0, 0,  width + 8, height + 8);
        // 调整预警信息绘制的坐标
        painter.translate(-(xMargin + warningBackgroundRect.width()), 0);

        QColor backgroundColor = CommonHelper::getWarningLevelColor( cityWarningInfoList.at(i).level);
        painter.setBrush(backgroundColor);
        painter.setPen(QColor(Qt::transparent));
        painter.drawRoundedRect(warningBackgroundRect, 5, 5);
        CommonHelper::initPainter(painter, QColor("white"), painter.font().pixelSize());
        painter.drawText(QRect(4, 3, width, height),warningTypeName);
        //添加实际预警信息的坐标位置到warningRectList
        QPoint warningLeftTopPoint(preWarningRect.topLeft().x() - xMargin - warningBackgroundRect.width(), warningBeginPoint.y());
        preWarningRect = QRect(warningLeftTopPoint, QSize(warningBackgroundRect.width(), height));
        cityWarningRectList << preWarningRect;
    }
}

void TodayCityWeatherFrame::paintMajorWeather()
{
    QPainter painter(this);
    CommonHelper::initPainter(painter, QColor("white"), 100);
    /* 绘制天气图标 */
    QRect weatherIconRect = QRect(20, 120, 128, 128);
    QString iconPath = QString(":/weather_icon/weather_icon/%1.png").arg(cityTodayWeather.weatherId);
    painter.drawImage(weatherIconRect, QImage(iconPath));
    /* 绘制当前温度数字 */
    QString tempStr = QString("%1").arg(cityTodayWeather.temperature);
    QPoint tempPoint(weatherIconRect.topRight().x() + 20, weatherIconRect.topRight().y());
    QRect tempRect = CommonHelper::textPaintRect(painter.font(), tempPoint, tempStr);
    painter.drawText(tempRect, tempStr);
    /* 绘制摄氏度图标 */
    QPoint degreeCelsiusPoint(tempRect.topRight().x() + 6, tempRect.topRight().y());
    QRect degreeCelsiusRect(degreeCelsiusPoint, QSize(36, 36));
    painter.drawImage(degreeCelsiusRect, QImage(":/weather_icon/weather_icon/degree_celsius.svg"));
    /* 绘制天气描述 */
    QPoint weatherDescPoint(degreeCelsiusRect.topRight().x() + 10, degreeCelsiusRect.topRight().y() + 10);
    CommonHelper::initPainter(painter, QColor("white"), 32);
    QRect weatherDescRect = CommonHelper::textPaintRect(painter.font(), weatherDescPoint, cityTodayWeather.weatherDesc);
    painter.drawText(weatherDescRect, cityTodayWeather.weatherDesc);
    /* 绘制最低和最高温度数字 */
    QString tempLimitStr = QString("%1/%2").arg(cityTodayWeather.temperatureLow, cityTodayWeather.temperatureHigh);
    QPoint tempLimitPoint(weatherDescRect.bottomLeft().x(), weatherDescRect.bottomLeft().y() + 20);
    CommonHelper::initPainter(painter, QColor("white"), 28);
    QRect tempLimitRect = CommonHelper::textPaintRect(painter.font(), tempLimitPoint, tempLimitStr);
    painter.drawText(tempLimitRect, tempLimitStr);
    /* 绘制摄氏度图标 */
    QPoint degreeCelsiusPoint2(tempLimitRect.topRight().x() + 5, tempLimitRect.topRight().y());
    QRect degreeCelsiusRect2(degreeCelsiusPoint2, QSize(32, 32));
    painter.drawImage(degreeCelsiusRect2, QImage(":/weather_icon/weather_icon/degree_celsius.svg"));
}

void TodayCityWeatherFrame::paintMinorWeather()
{
    QPainter painter(this);
    CommonHelper::initPainter(painter, QColor("white"), 18);
    /* 绘制湿度图标 */
    QRect humidityIconRect = QRect(24, 270, 24, 24);
    painter.drawImage(humidityIconRect, QImage(":/weather_icon/weather_icon/humidity.svg"));
    /* 绘制湿度信息 */
    QPoint humidityPoint(humidityIconRect.topRight().x() + 10, humidityIconRect.topRight().y());
    QString humidityStr = QString::fromLocal8Bit("湿度") + QString("%1\%").arg(cityTodayWeather.humidity);
    QRect humidityStrRect = CommonHelper::textPaintRect(painter.font(), humidityPoint, humidityStr);
    painter.drawText(humidityStrRect, humidityStr);
    /* 绘制风向图标 */
    QPoint windPoint(humidityStrRect.topRight().x() + 20, humidityStrRect.topRight().y());
    QRect windIconRect = QRect(windPoint, humidityIconRect.size());
    painter.drawImage(windIconRect, QImage(":/weather_icon/weather_icon/wind_direction.svg"));
    /* 绘制风向信息 */
    QString windStr = /*QString::fromLocal8Bit("风向：") + */cityTodayWeather.windDirection + cityTodayWeather.windScale;
    QPoint windStrPoint(windIconRect.topRight().x() + 10, windIconRect.topRight().y());
    QRect windStrRect = CommonHelper::textPaintRect(painter.font(), windStrPoint, windStr);
    painter.drawText(windStrRect, windStr);
    /* 绘制空气质量图标 */
    QPoint aqiPoint(windStrRect.topRight().x() + 23, windStrRect.topRight().y());
    QRect aqiIconRect = QRect(aqiPoint, humidityIconRect.size());
    /* 绘制空气质量信息 */
    QString aqiStr = /*QString::fromLocal8Bit("空气质量：") + */cityTodayWeather.aqiLevel + QString("(%1)").arg(cityTodayWeather.aqi);
    QPoint aqiStrPoint(aqiIconRect.topRight().x() + 10, aqiIconRect.topRight().y());
    QRect aqiStrRect = CommonHelper::textPaintRect(painter.font(), aqiStrPoint, aqiStr);
    /* 绘制空气质量背景 */
    QRect aqiBackgroundRect = QRect(aqiIconRect.topLeft() - QPoint(6, 6), aqiStrRect.bottomRight() + QPoint(6, 6));
    aqiToolTipLabelRect = aqiBackgroundRect;
    QColor color = CommonHelper::getAirQualityIndexColor(cityTodayWeather.aqi.toInt());
    painter.setBrush(color);
    painter.setPen(QColor(Qt::transparent));
    painter.drawRoundedRect(aqiBackgroundRect, 5, 5);
    /* 先绘制背景，后绘制前景信息 */
    painter.setPen(QColor(Qt::white));
    painter.drawImage(aqiIconRect, QImage(":/weather_icon/weather_icon/aqi.svg"));
    painter.drawText(aqiStrRect, aqiStr);
}

void TodayCityWeatherFrame::slotAqiToolTipVisible(bool isVisible)
{
    if (isVisible) {
        CityAqi aqiData;
        aqiData.primary = cityTodayWeather.primary;
        aqiData.pm10 = cityTodayWeather.pm10;
        aqiData.pm2p5 = cityTodayWeather.pm2p5;
        aqiData.no2 = cityTodayWeather.no2;
        aqiData.so2 = cityTodayWeather.so2;
        aqiData.co = cityTodayWeather.co;

        aqiToolTip->setAqiData(aqiData);

        int x = aqiToolTipLabelRect.topLeft().x() + aqiToolTipLabelRect.width() / 2 - aqiToolTip->width() / 2;
        int y = aqiToolTipLabelRect.y() - aqiToolTip->height() - 5;
        aqiToolTip->move(x, y);
        aqiToolTip->show();
    }
    else {
        aqiToolTip->hide();
    }
}

void TodayCityWeatherFrame::slotWarningToolTipVisible(bool isVisible, int index)
{
    if ( index >= cityWarningInfoList.size() || index < 0){
        warningToolTip->hide();
        return;
    }

    if (isVisible) {
       warningToolTip->setWarningData(cityWarningInfoList.at(index));
       QRect warningRect = cityWarningRectList.at(index);
       int x = warningRect.bottomLeft().x() + warningRect.width() / 2 - warningToolTip->width() / 2;
       int y = warningRect.bottomLeft().y() + 16;
//       warningToolTip->move(x, y);
       warningToolTip->move(mapToGlobal(QPoint(x, y)));
       warningToolTip->show();
    }
    else {
        warningToolTip->hide();
    }
}
