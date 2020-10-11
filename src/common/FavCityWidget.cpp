#include <QtWidgets>

#include "SignalManager.h"

#include "FavCityWidget.h"

FavCityWidget::FavCityWidget(QString name, QString id, QWidget *parent)
    : QWidget(parent), cityName(name), cityId(id)
{
    initUI();
    initConnect();
}

void FavCityWidget::initUI()
{
    favCityButton = new QPushButton(cityName);
    favCityButton->setObjectName("favCityButton");
    favCityButton->setMinimumSize(120, 30);
    favCityButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    delCityButton = new QPushButton();
    delCityButton->setObjectName("delCityButton");
    delCityButton->setFixedSize(16, 16);
    delCityButton->setFlat(true);

    QGridLayout *gLayout = new QGridLayout();
    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->addWidget(favCityButton, 0, 0, 1, 1, Qt::AlignCenter);
    gLayout->addWidget(delCityButton, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignRight);
    setLayout(gLayout);

    delCityButton->hide();
}

void FavCityWidget::initConnect()
{
    connect(favCityButton, &QPushButton::clicked, [=]{SignalManager::instance()->sigShowCity(cityName, cityId);});
    connect(delCityButton, &QPushButton::clicked, [=]{SignalManager::instance()->sigDelCity(cityName, cityId);});
}

void FavCityWidget::setEditable(bool editable)
{
    delCityButton->setVisible(editable);
}
