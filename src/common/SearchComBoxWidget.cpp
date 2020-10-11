#include <QtWidgets>
#include <QNetworkReply>
#include <QSignalBlocker>

#include "AppConfig.h"
#include "HttpClient.h"
#include "SignalManager.h"

#include "SearchComBoxWidget.h"

SearchComBoxWidget::SearchComBoxWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initTimer();
    initConnect();
}

void SearchComBoxWidget::initUI()
{
    comboBox = new QComboBox();
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    comboBox->setEditable(true);
    comboBox->lineEdit()->setPlaceholderText(QString::fromLocal8Bit("请输入城市名称,支持中文或拼音,不包含空格"));

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(comboBox);

    setLayout(vLayout);
}

void SearchComBoxWidget::initTimer()
{
    timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(500);
}

void SearchComBoxWidget::initConnect()
{
    connect(timer, &QTimer::timeout, [=](){slotSearchCity(comboBox->currentText());});
    connect(comboBox, &QComboBox::currentTextChanged, [=](){timer->start();});
    connect(comboBox, QOverload<int>::of(&QComboBox::activated),
            [=](int index){ QVariantMap map = comboBox->itemData(index).toMap();
                            emit SignalManager::instance()->sigAddCity(map["name"].toString(), map["id"].toString());});
}

void SearchComBoxWidget::slotSearchCity(QString name)
{
    currentText =  comboBox->currentText();
    if ( false == name.isEmpty() && false == name.contains(" ", Qt::CaseInsensitive)) {
        QSignalBlocker blocker(comboBox->lineEdit());
        comboBox->clear();
        /* clear之后lineedit的内容会被清除，再次设置，避免lineedit内容闪烁。 */
        comboBox->lineEdit()->setText(currentText);
        QNetworkReply *reply = HttpClient::instance()->getSearchCityRequest(name);
        connect(reply, &QNetworkReply::finished, this, &SearchComBoxWidget::slotParseSearchCity);
    }
}

void SearchComBoxWidget::slotParseSearchCity()
{       
    QSignalBlocker blocker(comboBox->lineEdit());

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

    QJsonArray locationArray = obj.value("location").toArray();
    for (int i = 0; i< locationArray.size(); i++) {
        QJsonObject location = locationArray.at(i).toObject();
        QString locationDetailName = QString("%1 %2 %3").arg(location.value("adm1").toString())
                                                        .arg(location.value("adm2").toString())
                                                        .arg(location.value("name").toString());
        QVariantMap  data;
        data["id"] = location.value("id").toString();
        data["name"] = location.value("name").toString();
        comboBox->addItem(locationDetailName, data);
    }

    comboBox->setCurrentIndex(-1);
    comboBox->showPopup();
    /* 下拉框显示后，lineedit的内容会被自动填充为下拉框的内容。 */
    comboBox->lineEdit()->setText(currentText);
}
