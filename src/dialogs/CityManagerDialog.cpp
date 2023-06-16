#include <QtWidgets>
#include <QNetworkReply>
#include <QSettings>

#include "AppConfig.h"
#include "FramelessWidget.h"
#include "TitleWidget.h"
#include "HttpClient.h"
#include "SearchComBoxWidget.h"
#include "SignalManager.h"
#include "FavCityWidget.h"

#include "CityManagerDialog.h"

QList<TopCity> CityManagerDialog::topCityList = QList<TopCity>();

CityManagerDialog::CityManagerDialog(QWidget *parent) : QDialog(parent)
{
    framelessWidget = new FramelessWidget(this);
    framelessWidget->setFramelessWidgetResizeEnable(false);
    initUI();
    initConnect();
}

CityManagerDialog::~CityManagerDialog()
{
    saveFavCityConfig();
}

void CityManagerDialog::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initTitleWidget();
    initCenterWidget();

    auto *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->addWidget(titleWidget);

    /* 不遮挡当前主窗口的的圆角矩形 */
    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(5, 5, 5, 5);
    hLayout->addWidget(centerWidget);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
}

void CityManagerDialog::initConnect()
{
    connect(SignalManager::instance(), &SignalManager::sigAddCity, this, &CityManagerDialog::slotAddFavCity);
    connect(SignalManager::instance(), &SignalManager::sigDelCity, this, &CityManagerDialog::slotDelFavCity);
    connect(editCityButton, &QPushButton::toggled, this, &CityManagerDialog::slotEditFavCity);
}

void CityManagerDialog::initTitleWidget()
{
    titleWidget = new TitleWidget(QString(), QString::fromLocal8Bit("城市管理"), false, false, false, this);
}

void CityManagerDialog::initCenterWidget()
{
    searchWidget = new SearchComBoxWidget();
    searchWidget->setMinimumWidth(400);

    QFrame *hSeparatorLine1 = new QFrame();
    hSeparatorLine1->setFrameStyle(QFrame::HLine);

    QLabel *topCityLabel = new QLabel(QString::fromLocal8Bit("热门城市:"));
    topCityLayout = new QGridLayout();
    topCityLayout->setHorizontalSpacing(30);
    topCityLayout->setVerticalSpacing(10);
    /* 防止热门城市数量不足一行 */
    QHBoxLayout *hTopLayout = new QHBoxLayout();
    hTopLayout->setContentsMargins(0, 0, 0,0);
    hTopLayout->addLayout(topCityLayout);
    hTopLayout->addStretch(1);

    if (!topCityList.isEmpty()) {
        updateTopCityWidget();
    } else {
        getTopCityList();
    }
    QFrame  *hSeparatorLine2 = new QFrame();
    hSeparatorLine2->setFrameStyle(QFrame::HLine);

    QLabel *favCityLabel = new QLabel(QString::fromLocal8Bit("收藏城市:"));
    editCityButton = new QPushButton(QIcon(":/weather_icon/weather_icon/edit_city.svg"), QString());
    editCityButton->setObjectName("editCityButton");
    editCityButton->setCheckable(true);
    editCityButton->setFixedSize(32, 32);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(favCityLabel);
    hLayout->addStretch();
    hLayout->addWidget(editCityButton);

    favCityLayout = new QGridLayout();
    favCityLayout->setHorizontalSpacing(30);
    favCityLayout->setVerticalSpacing(10);
    /* 防止收藏城市数量不足一行 */
    QHBoxLayout *hFavLayout = new QHBoxLayout();
    hFavLayout->addLayout(favCityLayout);
    hFavLayout->addStretch(1);

    initFavCityWidget();
    updateFavCityWidget();

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(10);
    vLayout->addWidget(searchWidget, 0, Qt::AlignCenter);
    vLayout->addWidget(hSeparatorLine1);
    vLayout->addWidget(topCityLabel, 0, Qt::AlignLeft);
    vLayout->addLayout(hTopLayout);
    vLayout->addWidget(hSeparatorLine2);
    vLayout->addLayout(hLayout);
    vLayout->addLayout(hFavLayout);

    centerWidget = new QWidget();
    centerWidget->setLayout(vLayout);
}

void CityManagerDialog::updateTopCityWidget()
{
    QList<QPushButton*> layoutWidgets = topCityLayout->findChildren<QPushButton*>();
    for (auto widget : layoutWidgets) {
        delete widget;
    }

    for (int i = 0; i< topCityList.size(); i++) {
        TopCity city = topCityList.at(i);
        QString displayCityName = (city.cityName == city.DistrictName) ? city.cityName : city.cityName + city.DistrictName;
        QPushButton *cityButton = new QPushButton(displayCityName);

        cityButton->setFixedHeight(30);
        cityButton->setMinimumWidth(120);
        cityButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        cityButton->setProperty("CityLabel", true);
        cityButton->setProperty("CityId", city.cityId);
        cityButton->setProperty("CityName", city.DistrictName);
        topCityLayout->addWidget(cityButton, i / 5, i % 5, 1, 1);

        connect(cityButton, &QPushButton::clicked, this, &CityManagerDialog::slotTopCityClicked);
    }
}

void CityManagerDialog::initFavCityWidget()
{
    QSettings settings;
    int size = settings.beginReadArray("FavCity");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString cityName = settings.value("city_name").toString();
        QString cityId = settings.value("city_id").toString();
        FavCityWidget *favCityWidget = new FavCityWidget(cityName, cityId);
        favCityList << favCityWidget;
    }
    settings.endArray();
}

void CityManagerDialog::updateFavCityWidget()
{
    for (auto favCityWidget : favCityList) {
        favCityLayout->removeWidget(favCityWidget);
    }
    for (int i = 0; i < favCityList.size(); i++) {
        FavCityWidget *favCityWidget = favCityList.at(i);
        favCityWidget->setFixedHeight(30);
        favCityWidget->setMinimumWidth(120);
        favCityWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        favCityLayout->addWidget(favCityWidget, i /5, i % 5, 1, 1);
    }
}

void CityManagerDialog::saveFavCityConfig()
{
    QSettings settings;
    settings.beginWriteArray("FavCity");
    for (int i = 0; i < favCityList.size(); ++i) {
        FavCityWidget *favCityWidget = favCityList.at(i);
        settings.setArrayIndex(i);
        settings.setValue("city_name", favCityWidget->getCityName());
        settings.setValue("city_id", favCityWidget->getCityId());
    }
    settings.endArray();
}

bool CityManagerDialog::isFavCityExits(const QString &cityName, const QString &cityId)
{
    Q_UNUSED(cityName)

    for (auto favCity : favCityList){
        if (favCity->getCityId() == cityId){
            return true;
        }
    }
    return false;
}

int CityManagerDialog::getFavCityWidget(const QString &cityName, const QString &cityId)
{
    Q_UNUSED(cityName)

    for (int i = 0; i< favCityList.size(); i++) {
        if (favCityList.at(i)->getCityId() == cityId){
            return i;
        }
    }
    return -1;
}

void CityManagerDialog::paintEvent(QPaintEvent *event)
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

void CityManagerDialog::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Enter == event->key()){
        event->ignore();
    }
}

void CityManagerDialog::getTopCityList()
{
     QNetworkReply *reply = HttpClient::instance()->getTopCityRequest();
     connect(reply, &QNetworkReply::finished, this, &CityManagerDialog::slotParseTopCity);
}

void CityManagerDialog::slotParseTopCity()
{
    QNetworkReply *reply =  qobject_cast<QNetworkReply *>(QObject::sender());
    reply->deleteLater();

    QJsonDocument doc;
    if ( false == HttpClient::validateReplay(reply, doc)) {
        return;
    }

    QJsonObject obj = doc.object();
    if ( 200 != obj.value("code").toString().toInt()) {
        return;
    }

    QJsonArray cityArray = obj.value("topCityList").toArray();
    for (int i = 0; i< cityArray.size(); i++) {
        TopCity topCity;
        QJsonObject cityJson = cityArray.at(i).toObject();
        topCity.DistrictName = cityJson.value("name").toString();
        topCity.cityName = cityJson.value("adm2").toString();
        topCity.cityId = cityJson.value("id").toString();
        topCityList << topCity;
    }

    updateTopCityWidget();
}

void CityManagerDialog::slotTopCityClicked()
{
    QPushButton *cityButton = qobject_cast<QPushButton*>(QObject::sender());

    QString id = cityButton->property("CityId").toString();
    QString name = cityButton->property("CityName").toString();

    emit SignalManager::instance()->sigAddCity(name, id);
}

void CityManagerDialog::slotEditFavCity(bool checked)
{
    QList<FavCityWidget*> favCities = findChildren<FavCityWidget*>();
    for( auto favCity : favCities) {
        favCity->setEditable(checked);
    }
}

void CityManagerDialog::slotAddFavCity(const QString &cityName, const QString &cityId)
{
    if (!isFavCityExits(cityName, cityId)){
        auto *favCity = new FavCityWidget(cityName, cityId);
        favCityList << favCity;
        updateFavCityWidget();
    }
}

void CityManagerDialog::slotDelFavCity(const QString &cityName, const QString &cityId)
{
    int i = getFavCityWidget(cityName, cityId);
    if (i >= 0){
        FavCityWidget *favCity = favCityList.takeAt(i);
        favCityLayout->removeWidget(favCity);
        updateFavCityWidget();
        delete favCity;
    }
}
