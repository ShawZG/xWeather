#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QDebug>

#include "WeatherHttpJob.h"
#include "FutureCityWeatherListView.h"
#include "TodayCityWeatherFrame.h"
#include "LifeIndexListView.h"
#include "HttpClient.h"
#include "CommonHelper.h"

#include "WeatherWidget.h"

WeatherWidget::WeatherWidget(QString cityId, QString cityName, QWidget *parent)
    : QWidget(parent), cityId(cityId), cityName(cityName)
{
    initUI();
    initWeatherHttpJob();
    initConnect();
}

void WeatherWidget::initUI()
{
    todayCityFrame = new TodayCityWeatherFrame(this);
    todayCityFrame->setFixedSize(520, 320);

    futureCityListView = new FutureCityWeatherListView(this);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 5, 0, 0);
    hLayout->setSpacing(0);
    hLayout->addWidget(todayCityFrame, 0, Qt::AlignCenter);
    hLayout->addWidget(futureCityListView, 0, Qt::AlignCenter);

    lifeIndexListView = new LifeIndexListView();
    lifeIndexListView->setObjectName("lifeIndexListView");

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(10, 10, 10, 10);
    vLayout->setSpacing(20);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(lifeIndexListView, 0, Qt::AlignCenter);

    setLayout(vLayout);
}

void WeatherWidget::initConnect()
{
    connect(todayCityFrame, &TodayCityWeatherFrame::sigUpdateWeather,
            this, [this](){this->weatherHttpJob->updateWeather();});
}

void WeatherWidget::initWeatherHttpJob()
{
    weatherHttpJob = new WeatherHttpJob(cityId, cityName, this);
    weatherHttpJob->startJob();
}

void WeatherWidget::updateTodayCityWeather(CityTodayWeather data)
{
    todayCityFrame->setTodayCiytWeatherData(data);
}

void WeatherWidget::updateTodayLifyIndex(QList<CityLifeIndex> list)
{
    lifeIndexListView->setTodayLifeIndexData(list);
}

void WeatherWidget::updateFutureCityWeather(QList<CityFutureWeather> list)
{
    futureCityListView->seFutureCityWeatherData(list);
}

void WeatherWidget::updateWarningCityWeather(QList<CityWarningWeather> list)
{
    todayCityFrame->setWarningCityWeather(list);
}

void WeatherWidget::setCityInfo(const QString &cityId, const QString &cityName)
{
    this->cityId = cityId;
    this->cityName = cityName;
}
