#ifndef AQITOOLTIP_H
#define AQITOOLTIP_H

#include <QWidget>
#include "Global.h"

class QGraphicsDropShadowEffect;

class AqiToolTip : public QWidget
{
    Q_OBJECT
public:
    explicit AqiToolTip(QWidget *parent = nullptr);
    void setAqiData(CityAqi cityAqiData);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();

    void paintForeground();
    void paintBackground();
    QGraphicsDropShadowEffect *shadowEffect = nullptr;
    CityAqi aqiData;
};

#endif // AQITOOLTIP_H
