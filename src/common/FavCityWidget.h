#ifndef FAVCITYWIDGET_H
#define FAVCITYWIDGET_H

#include <QWidget>

class QPushButton;

class FavCityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FavCityWidget(QString name, QString id, QWidget *parent = nullptr);
    void setEditable(bool editable);

    inline QString getCityName() {return cityName;}
    inline QString getCityId() {return cityId;}

private:
    void initUI();
    void initConnect();

    QPushButton     *favCityButton = nullptr;
    QPushButton     *delCityButton = nullptr;

    QString     cityName;
    QString     cityId;
};

#endif // FAVCITYWIDGET_H
