#ifndef TODAYCITYWEATHERFRAME_H
#define TODAYCITYWEATHERFRAME_H

#include "Global.h"
#include <QFrame>

class AqiToolTip;

class TodayCityWeatherFrame : public QFrame
{
    Q_OBJECT
public:
    explicit TodayCityWeatherFrame(QWidget *parent = nullptr);
    void     setTodayCiytWeatherData(CityTodayWeather data);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void sigUpdateWeather();
    void sigAddCity();
    void sigAqiToolTipVisible(bool isVisible);

private slots:
    void slotAqiToolTipVisible(bool isVisible);

private:
    void initConnect();

    void paintLocationDate();
    void paintUpdateTime();
    void paintMajorWeather();
    void paintMinorWeather();

    bool                hasWeatherData = false;
    CityTodayWeather    cityTodayWeather;

    QRect               refreshButtonRect;
    bool                isRefreshButtonHover = false;
    bool                isRefreshButtonPressed = false;
    bool                isRefreshButtonReleased = false;

    QRect               locationButtonRect;
    bool                isLocationButtonHover = false;
    bool                isLocationButtonPressed = false;
    bool                isLocationButtonReleased = false;

    QRect               aqiToolTipLabelRect;
    bool                isAqiToolTipLabelHover = false;
    AqiToolTip          *aqiToolTip = nullptr;

};

#endif // TODAYCITYWEATHERFRAME_H
