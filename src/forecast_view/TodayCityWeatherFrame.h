#ifndef TODAYCITYWEATHERFRAME_H
#define TODAYCITYWEATHERFRAME_H

#include "Global.h"
#include <QFrame>

class AqiToolTip;
class WarningToolTip;

class TodayCityWeatherFrame : public QFrame
{
    Q_OBJECT
public:
    explicit TodayCityWeatherFrame(QWidget *parent = nullptr);
    void setTodayCiytWeatherData(CityTodayWeather data);
    void setWarningCityWeather(const QList<CityWarningWeather> &list);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void sigUpdateWeather();
    void sigAddCity();
    void sigAqiToolTipVisible(bool isVisible);
    void sigWarningToolTipVisible(bool isVisible, int index);

private slots:
    void slotAqiToolTipVisible(bool isVisible);
    void slotWarningToolTipVisible(bool isVisible, int index);


private:
    void initConnect();

    void paintLocationDate();
    void paintUpdateTime();
    void paintMajorWeather();
    void paintMinorWeather();
    void paintWarningInfo();

    bool                        hasWeatherData = false;
    CityTodayWeather            cityTodayWeather;

    QRect       refreshButtonRect;
    bool        isRefreshButtonHover = false;
    bool        isRefreshButtonPressed = false;
    bool        isRefreshButtonReleased = false;

    QRect       locationButtonRect;
    bool        isLocationButtonHover = false;
    bool        isLocationButtonPressed = false;
    bool        isLocationButtonReleased = false;

    QRect       aqiToolTipLabelRect;
    bool        isAqiToolTipLabelHover = false;
    AqiToolTip  *aqiToolTip = nullptr;

    bool        isWarningRectHover = false;
    QList<CityWarningWeather>   cityWarningInfoList;
    QPoint       warningBeginPoint;
    QList<QRect>    cityWarningRectList;
    WarningToolTip  *warningToolTip = nullptr;

};

#endif // TODAYCITYWEATHERFRAME_H
