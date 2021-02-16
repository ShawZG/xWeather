#ifndef WARNINGTOOLTIP_H
#define WARNINGTOOLTIP_H

#include <QWidget>
#include "Global.h"

class QLabel;
class QGraphicsDropShadowEffect;

class WarningToolTip : public QWidget
{
    Q_OBJECT
public:
    explicit WarningToolTip(QWidget *parent = nullptr);
    void setWarningData(const CityWarningWeather &cityWarningData);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();
    void updateWidgetSize();

    void paintForeground();
    void paintBackground();
    QGraphicsDropShadowEffect *shadowEffect = nullptr;
    CityWarningWeather warningData;

    QRect   textRect;
    QLabel *textLabel = nullptr;
};

#endif // WARNINGTOOLTIP_H
