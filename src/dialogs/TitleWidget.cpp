#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMenu>

#include "TitleWidget.h"

TitleWidget::TitleWidget(QString iconPath, QString title, bool showMenuButton, bool showMiniButton, bool showMaxButton, QWidget *parent)
    : QWidget(parent),
      titleIconPath(iconPath), titleStr(title), showMenuButton(showMenuButton), showMiniButton(showMiniButton), showMaxButton(showMaxButton)
{
    initUI();
    initConnect();
}

void TitleWidget::initUI()
{
    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("titleWidget");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    titleIconLabel = new QLabel();
    titleIconLabel->setObjectName("titleIconLabel");
    titleIconLabel->setFixedSize(QSize(32, 32));
    titleIconLabel->hide();

    titleStrLabel = new QLabel();
    titleStrLabel->setObjectName("titleStrLabel");
    titleStrLabel->hide();

    titleMenuButton = new QPushButton("");
    titleMenuButton->setObjectName("titleMenuButton");
    titleMenuButton->hide();

    titleMiniButton = new QPushButton();
    titleMiniButton->setObjectName("titleMiniButton");
    titleMiniButton->hide();

    titleRestoreButton = new QPushButton();
    titleRestoreButton->setObjectName("titleRestoreButton");
    titleRestoreButton->hide();

    titleMaxButton = new QPushButton();
    titleMaxButton->setObjectName("titleMaxButton");
    titleMaxButton->hide();

    titleCloseButton = new QPushButton();
    titleCloseButton->setObjectName("titleCloseButton");

    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    hLayout->addSpacing(4);
    if (!titleIconPath.isEmpty()) {
        titleIconLabel->setPixmap(QPixmap(titleIconPath).scaled(titleIconLabel->rect().size()));
        titleIconLabel->show();
        hLayout->addWidget(titleIconLabel);

    }
    if (!titleStr.isEmpty()) {
        hLayout->addSpacing(20);
        titleStrLabel->setText(titleStr);
        titleStrLabel->show();
        hLayout->addWidget(titleStrLabel);
    }
    hLayout->addStretch();
    if (showMenuButton) {
        hLayout->addWidget(titleMenuButton);
        titleMenuButton->show();
    }
    if (showMiniButton) {
        hLayout->addWidget(titleMiniButton);
        titleMiniButton->show();
    }
    if (showMaxButton) {
        hLayout->addWidget(titleRestoreButton);
        hLayout->addWidget(titleMaxButton);
        titleMaxButton->show();
    }
    hLayout->addWidget(titleCloseButton);

    setLayout(hLayout);
}

void TitleWidget::initConnect()
{
    connect(titleMiniButton, &QPushButton::clicked, this, [this]() {
        this->parentWidget()->showMinimized();
    });
    connect(titleMaxButton, &QPushButton::clicked, this, [this]() {
        this->parentWidget()->showMaximized();
        this->titleMaxButton->hide();
        this->titleRestoreButton->show();
    });
    connect(titleRestoreButton, &QPushButton::clicked, this, [this]() {
        this->parentWidget()->showNormal();
        this->titleRestoreButton->hide();
        this->titleMaxButton->show();
    });
    connect(titleCloseButton, &QPushButton::clicked, this, [this]() {
        this->parentWidget()->close();
    });
}

void TitleWidget::setTitleMenu(QMenu *menu)
{
    if (nullptr != titleMenuButton) {
        titleMenuButton->setMenu(menu);
    }
}
