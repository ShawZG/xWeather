#include <QDateTime>
#include <QMouseEvent>

#include "AqiToolTip.h"
#include "CommonHelper.h"
#include "SignalManager.h"
#include "TodayCityWeatherFrame.h"

TodayCityWeatherFrame::TodayCityWeatherFrame(QWidget *parent) : QFrame(parent)
{
    setMouseTracking(true);
    aqiToolTip = new AqiToolTip(this);
    aqiToolTip->hide();

    initConnect();
}

void TodayCityWeatherFrame::initConnect()
{
    connect(this, &TodayCityWeatherFrame::sigAqiToolTipVisible, this, &TodayCityWeatherFrame::slotAqiToolTipVisible);
}

void TodayCityWeatherFrame::setTodayCiytWeatherData(CityTodayWeather data)
{
    hasWeatherData = true;
    cityTodayWeather = data;
    update();
}

void TodayCityWeatherFrame::mousePressEvent(QMouseEvent *event)
{
    isRefreshButtonPressed = refreshButtonRect.contains(event->pos());
    isLocationButtonPressed = locationButtonRect.contains(event->pos());
}

void TodayCityWeatherFrame::mouseReleaseEvent(QMouseEvent *event)
{
    isRefreshButtonReleased = refreshButtonRect.contains(event->pos());
    if (true == isRefreshButtonPressed && true == isRefreshButtonReleased){
        emit sigUpdateWeather();
    }
    isRefreshButtonPressed = false;
    isRefreshButtonReleased = false;

    isLocationButtonReleased = locationButtonRect.contains(event->pos());
    if (true == isLocationButtonPressed && true == isLocationButtonReleased){
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
}

void TodayCityWeatherFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (true == hasWeatherData) {
        paintLocationDate();
        paintUpdateTime();
        paintMajorWeather();
        paintMinorWeather();
    }
}

void TodayCityWeatherFrame::paintLocationDate()
{
    QPainter painter(this);
    CommonHelper::initPainter(painter, QColor("white"), 20);
    /* 绘制坐标点 */
    QString imagePath = (true == isLocationButtonHover)
                        ? ":/weather_icon/weather_icon/add_city.svg"
                        : ":/weather_icon/weather_icon/location.svg";
    locationButtonRect = QRect(16, 16, 28, 28);
    painter.drawImage(locationButtonRect, QImage(imagePath));
    /* 绘制城市，日期 */
    QString str = QString("%1 %2 %3").arg(cityTodayWeather.cityName)
                                     .arg(cityTodayWeather.date)
                                     .arg(cityTodayWeather.week);
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
    int buttonSize = (true == isRefreshButtonHover) ? 32 : 24;
    int xMargin = 20 - (buttonSize - 20) / 2;
    QPoint buttonPoint = textRect.topRight() + QPoint(xMargin, (textRect.height() - buttonSize) / 2);
    refreshButtonRect = QRect(buttonPoint, QSize(buttonSize, buttonSize));
    painter.drawImage(refreshButtonRect, QImage(":/weather_icon/weather_icon/refresh.svg"));
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
    QString tempLimitStr = QString("%1/%2").arg(cityTodayWeather.temperatureLow).arg(cityTodayWeather.temperatureHigh);
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
    if (true == isVisible) {
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

