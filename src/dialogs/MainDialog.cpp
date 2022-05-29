#include <QStackedWidget>
#include <QtWidgets>

#include "FramelessWidget.h"
#include "TitleWidget.h"
#include "WeatherWidget.h"
#include "AboutDialog.h"
#include "ConfigDialog.h"
#include "SignalManager.h"
#include "CityManagerDialog.h"

#include "MainDialog.h"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    framelessWidget = new FramelessWidget(this);
    framelessWidget->setFramelessWidgetResizeEnable(false);
    initUI();
    initConnect();
}

MainDialog::~MainDialog()
{
}

void MainDialog::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initTitleWidget();
    initMainWidget();

    auto *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->addWidget(titleWidget);

    /* 不遮挡MainDialog的圆角矩形 */
    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(5, 0, 5, 5);
    hLayout->addWidget(centerWidget);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
}

void MainDialog::initConnect()
{
    connect(SignalManager::instance(), &SignalManager::sigShowAddCityDialog, this, &MainDialog::slotShowCityManagerDialog);
    connect(SignalManager::instance(), &SignalManager::sigAddCity, this, &MainDialog::slotAddWeatherWidget);
    connect(SignalManager::instance(), &SignalManager::sigDelCity, this, &MainDialog::slotDelWeatherWidget);
    connect(SignalManager::instance(), &SignalManager::sigShowCity, this, &MainDialog::slotShowWeatherWidget);

    connect(pageUpButton, &QPushButton::clicked, this, &MainDialog::slotShowPrevCityWeather);
    connect(pageDownButton, &QPushButton::clicked, this, &MainDialog::slotShowNextCityWeather);
}

void MainDialog::initTitleWidget()
{
    titleWidget = new TitleWidget(QString(":/app_icon/app_icon/app_logo.svg"), QString::fromLocal8Bit("简单天气"), true, true, false, this);
    initTitleMenu();
}

void MainDialog::initTitleMenu()
{
    titleMenu = new QMenu();
    titleMenu->addAction(QString::fromLocal8Bit("城市管理"), this, &MainDialog::slotShowCityManagerDialog);
    titleMenu->addSeparator();
    titleMenu->addAction(QString::fromLocal8Bit("设置"), this, &MainDialog::slotShowConfigDialog);
    titleMenu->addAction(QString::fromLocal8Bit("关于"), this, &MainDialog::slotShowAboutDialog);
    titleWidget->setTitleMenu(titleMenu);
}

void MainDialog::initMainWidget()
{
    /* 默认显示当前所在城市天气 */
    auto weatherWidget = new WeatherWidget(QString(), QString());
    weatherStackWidget = new QStackedWidget();
    weatherStackWidget->addWidget(weatherWidget);
    weatherWidgetList.append(weatherWidget);

    pageUpButton = new QPushButton();
    pageUpButton->setFixedSize(64, 400);
    pageUpButton->setObjectName("pageUpButton");

    pageDownButton = new QPushButton();
    pageDownButton->setFixedSize(64, 400);
    pageDownButton->setObjectName("pageDownButton");

    auto gLayout = new QGridLayout();
    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->addWidget(weatherStackWidget, 0, 0, 1, 1);
    gLayout->addWidget(pageUpButton, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignHCenter);
    gLayout->addWidget(pageDownButton, 0, 0, 1, 1, Qt::AlignRight | Qt::AlignHCenter);

    centerWidget = new QWidget();
    centerWidget->setAttribute(Qt::WA_Hover, false);
    centerWidget->setLayout(gLayout);
}

void MainDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPalette pa = QGuiApplication::palette();
    painter.setBrush(pa.window());
    painter.setPen(Qt::transparent);
    QRect dialogRect;
    dialogRect.setWidth(rect().width());
    dialogRect.setHeight(rect().height());
    painter.drawRoundedRect(dialogRect, 10, 10);
    QDialog::paintEvent(event);
}

void MainDialog::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);
}

void MainDialog::slotShowCityManagerDialog()
{
    CityManagerDialog dlg;
    dlg.exec();
}

void MainDialog::slotShowConfigDialog()
{
    ConfigDialog dialog;
    dialog.exec();
}

void MainDialog::slotShowAboutDialog()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainDialog::slotAddWeatherWidget(const QString &cityName, const QString &cityId)
{
    for (auto weatherWidget : weatherWidgetList){
        if (weatherWidget->getCityId() == cityId){
            weatherStackWidget->setCurrentWidget(weatherWidget);
            return;
        }
    }

    auto weatherWidget = new WeatherWidget(cityId, cityName);
    weatherStackWidget->addWidget(weatherWidget);
    weatherWidgetList.append(weatherWidget);
    weatherStackWidget->setCurrentWidget(weatherWidget);
}

void MainDialog::slotShowWeatherWidget(const QString &cityName, const QString &cityId)
{
    Q_UNUSED(cityName)

    for (auto weatherWidget : weatherWidgetList){
        if (weatherWidget->getCityId() == cityId){
            weatherStackWidget->setCurrentWidget(weatherWidget);
            return;
        }
    }

    slotAddWeatherWidget(cityName, cityId);
}

void MainDialog::slotDelWeatherWidget(const QString &cityName, const QString &cityId)
{
    Q_UNUSED(cityName)

    int index;
    for (index = 0; index < weatherWidgetList.size(); index++) {
        if ( weatherWidgetList.at(index)->getCityId() == cityId){
            break;
        }
    }
    if (index < weatherWidgetList.size()) {
        WeatherWidget* delWidget = weatherWidgetList.takeAt(index);
         weatherStackWidget->removeWidget(delWidget);
        delWidget->deleteLater();
    }
}

void MainDialog::slotShowPrevCityWeather()
{
    auto curWidget = qobject_cast<WeatherWidget*>(weatherStackWidget->currentWidget());

    int index;
    for (index = 0; index < weatherWidgetList.size(); index++) {
        WeatherWidget*  weatherWidget = weatherWidgetList.at(index);
        if ( weatherWidget->getCityId() == curWidget->getCityId()){
            break;
        }
    }

    if (index - 1 >= 0) {
        weatherStackWidget->setCurrentWidget(weatherWidgetList.at(index - 1));
    }
}

void MainDialog::slotShowNextCityWeather()
{
    auto curWidget = qobject_cast<WeatherWidget*>(weatherStackWidget->currentWidget());

    int index;
    for (index = 0; index < weatherWidgetList.size(); index++) {
        WeatherWidget*  weatherWidget = weatherWidgetList.at(index);
        if ( weatherWidget->getCityId() == curWidget->getCityId()){
            break;
        }
    }

    if (index + 1 < weatherWidgetList.size()) {
        weatherStackWidget->setCurrentWidget(weatherWidgetList.at(index + 1));
    }
}
