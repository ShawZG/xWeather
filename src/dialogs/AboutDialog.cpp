#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QPainter>
#include <QLabel>
#include <QApplication>

#include "AppConfig.h"
#include "FramelessWidget.h"
#include "TitleWidget.h"

#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    framelessWidget = new FramelessWidget(this);
    framelessWidget->setFramelessWidgetResizeEnable(false);
    initUI();
}

void AboutDialog::initUI()
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

void AboutDialog::initTitleWidget()
{
    titleWidget = new TitleWidget(QString(), QString::fromLocal8Bit("关于"), false, false, false, this);
}

void AboutDialog::initCenterWidget()
{
    centerWidget = new QWidget();

    auto *iconLabel = new QLabel();
    iconLabel->setFixedSize(96, 96);
    QPixmap icon(":/app_icon/app_icon/app_logo.svg");
    iconLabel->setPixmap(icon.scaled(iconLabel->size()));

    QFont font = QApplication::font();
    font.setPointSize(14);

    auto *verLabel = new QLabel();
    verLabel->setFont(font);
    verLabel->setText("版    本: " + AppConfig::getAppVersion());

    auto *fallbackLabel = new QLabel();
    fallbackLabel->setFont(font);
    fallbackLabel->setText("作    者: blueshaw@163.com");

    auto *apiLabel = new QLabel();
    apiLabel->setFont(font);
    apiLabel->setText(QString::fromLocal8Bit("数据来源: 和风天气"));

    auto *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(10);
    vLayout->addStretch();
    vLayout->addWidget(verLabel);
    vLayout->addWidget(fallbackLabel);
    vLayout->addWidget(apiLabel);
    vLayout->addStretch();

    auto *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(10, 10, 10, 10);
    hLayout->addWidget(iconLabel);
    hLayout->addSpacing(20);
    hLayout->addLayout(vLayout);

    centerWidget->setLayout(hLayout);
}

void AboutDialog::paintEvent(QPaintEvent *event)
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
