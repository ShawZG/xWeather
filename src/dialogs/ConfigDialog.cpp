#include <QtWidgets>
#include <QDesktopServices>

#include "AppConfig.h"
#include "FramelessWidget.h"
#include "TitleWidget.h"

#include "ConfigDialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent)
{
    framelessWidget = new FramelessWidget(this);
//    framelessWidget->setFramelessWidgetResizeEnable(false);
    initUI();
    initConnect();
}

ConfigDialog::~ConfigDialog()
{
    AppConfig::setUserAppId(apiPublicIdEdit->toPlainText().trimmed());
    AppConfig::setUserAppKey(apiKeyEdit->toPlainText().trimmed());
}

void ConfigDialog::initUI()
{
    setMinimumWidth(490);
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
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(centerWidget);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
}

void ConfigDialog::initConnect()
{
    connect(visitWebButton, &QPushButton::clicked, this, &ConfigDialog::slotOpenWeb);
}

void ConfigDialog::initTitleWidget()
{
    titleWidget = new TitleWidget(QString(), QString::fromLocal8Bit("设置"), false, false, false, this);
}

void ConfigDialog::initCenterWidget()
{
    leftListNavigation = new QListWidget();
    leftListNavigation->setMaximumWidth(130);

//   initBaseConfigWidget();
    initApiConfigWidget();

    if (leftListNavigation->count() > 0) {
        leftListNavigation->setCurrentItem(leftListNavigation->item(0));
    }

    QVBoxLayout *vRightLayout = new QVBoxLayout();
    vRightLayout->setContentsMargins(5, 5, 5, 5);
    vRightLayout->setSpacing(5);
//    vRightLayout->addWidget(baseConfigWidget);
    vRightLayout->addWidget(apiConfigWidget);
    vRightLayout->addStretch();

    QWidget *rightWidget = new QWidget();
    rightWidget->setLayout(vRightLayout);

    QScrollArea *rightScrollConfig = new QScrollArea();
    rightScrollConfig->setFrameShape(QFrame::NoFrame);
    rightScrollConfig->setWidget(rightWidget);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 5, 0);
    hLayout->setSpacing(0);
    hLayout->addWidget(leftListNavigation);
    hLayout->addSpacing(4);
    hLayout->addWidget(rightScrollConfig);

    centerWidget = new QWidget();
    centerWidget->setLayout(hLayout);
}

void ConfigDialog::initBaseConfigWidget()
{
    QListWidgetItem *baseItem = new QListWidgetItem(QString::fromLocal8Bit("基本设置"));
    baseItem->setSizeHint(QSize(120, 40));
    leftListNavigation->addItem(baseItem);

    baseConfigWidget = new QWidget();
    baseConfigWidget->setObjectName("baseConfigWidget");
    QLabel *baseLabel = new QLabel(QString::fromLocal8Bit("基本设置"));
    QCheckBox *pushExtremeWeather = new QCheckBox(QString::fromLocal8Bit("推送极端天气通知"));
    QCheckBox *autoRefreshWeather = new QCheckBox(QString::fromLocal8Bit("后台自动刷新天气"));
    QComboBox *timeComboBox = new QComboBox();
    timeComboBox->addItem(QString::fromLocal8Bit("5分钟"), QVariant(5));
    timeComboBox->addItem(QString::fromLocal8Bit("10分钟"), QVariant(10));
    timeComboBox->addItem(QString::fromLocal8Bit("15分钟"), QVariant(15));
    timeComboBox->addItem(QString::fromLocal8Bit("30分钟"), QVariant(30));
    timeComboBox->addItem(QString::fromLocal8Bit("60分钟"), QVariant(60));
    QVBoxLayout *vBaseLayout = new QVBoxLayout();
    vBaseLayout->setContentsMargins(5, 20, 5, 20);
    vBaseLayout->setSpacing(10);
    vBaseLayout->addWidget(baseLabel);
    vBaseLayout->addWidget(pushExtremeWeather);
    vBaseLayout->addWidget(autoRefreshWeather);
    vBaseLayout->addWidget(timeComboBox);
    baseConfigWidget->setLayout(vBaseLayout);
}

void ConfigDialog::initApiConfigWidget()
{
    QListWidgetItem *apiItem = new QListWidgetItem(QString::fromLocal8Bit("天气接口"));
    apiItem->setSizeHint(QSize(120, 40));
    leftListNavigation->addItem(apiItem);

    apiConfigWidget = new QWidget();
    apiConfigWidget->setObjectName("apiConfigWidget");
    QLabel *apiLabel = new QLabel(QString::fromLocal8Bit("天气接口"));
    QLabel *apiComment = new QLabel();
    apiComment->setText("免费版本的天气数据接口对每天的访问量和访问频率都做了限制。"
                        "为了您更好的使用本软件，建议您去和风天气官网(https://dev.heweather.com)注册成为个人开发者，"
                        "使用自己的认证信息请求天气数据。您的个人信息不会被收集，请放心使用。");
    apiComment->setWordWrap(true);
    visitWebButton = new QPushButton(QString::fromLocal8Bit("前往注册"));
    visitWebButton->setObjectName("visitWebButton");
    visitWebButton->setFixedHeight(32);

    apiPublicIdEdit = new QTextEdit();
    apiPublicIdEdit->setObjectName("apiPublicIdEdit");
    apiPublicIdEdit->setPlaceholderText(QString::fromLocal8Bit("请在此处填入从和风天气官网申请的Public ID值。"));
    apiPublicIdEdit->setFixedHeight(64);
    apiPublicIdEdit->setText(AppConfig::getUserAppId());

    apiKeyEdit = new QTextEdit();
    apiKeyEdit->setObjectName("apiKeyEdit");
    apiKeyEdit->setPlaceholderText(QString::fromLocal8Bit("请在此处填入从和风天气官网申请的KEY值。"));
    apiKeyEdit->setFixedHeight(96);
    apiKeyEdit->setText(AppConfig::getUserAppKey());

    QVBoxLayout *vApiLayout = new QVBoxLayout();
    vApiLayout->setContentsMargins(5, 20, 5, 20);
    vApiLayout->setSpacing(10);
    vApiLayout->addWidget(apiLabel);
    vApiLayout->addWidget(apiComment);
    vApiLayout->addWidget(visitWebButton);
    vApiLayout->addWidget(apiPublicIdEdit);
    vApiLayout->addWidget(apiKeyEdit);
    apiConfigWidget->setLayout(vApiLayout);
}

void ConfigDialog::paintEvent(QPaintEvent *event)
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

void ConfigDialog::slotOpenWeb()
{
    QString url = "https://dev.heweather.com/docs/faq/user";
    QDesktopServices::openUrl(QUrl(url));
}
