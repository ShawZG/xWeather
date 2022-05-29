#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QApplication>
#include <QPalette>
#include <QFontMetrics>
#include <QDebug>
#include <QtGlobal>
#include <QLabel>
#include <QVBoxLayout>

#include "AqiToolTip.h"
#include "CommonHelper.h"
#include "WarningToolTip.h"

/* 三角形大小 */
const int TRIANGLE_WIDTH = 40;
const int TRIANGLE_HEIGHT = 20;
const int TEXT_MARGIN = 8;
const int WIDGET_WIDTH = 260;

WarningToolTip::WarningToolTip(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void WarningToolTip::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    // 设置阴影边框;
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(10);
    setGraphicsEffect(shadowEffect);

//  QFontMetrics::boundingRect返回的绘制文字的QRect信息不准确，
//  故不使用此方法，直接把文字信息放在QLabel中。
    QFont font;
    font.setPointSizeF(12);
    textLabel = new QLabel();
    textLabel->setFont(font);
    textLabel->setWordWrap(true);
    textLabel->setFrameShape(QFrame::NoFrame);
    textLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(TEXT_MARGIN, TRIANGLE_HEIGHT + TEXT_MARGIN, TEXT_MARGIN, TEXT_MARGIN);
    vLayout->addWidget(textLabel);
    setLayout(vLayout);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void WarningToolTip::updateWidgetSize()
{
//    QRect originRect(QPoint(TEXT_MARGIN, TRIANGLE_HEIGHT + TEXT_MARGIN),
//                     QSize(WIDGET_WIDTH - TEXT_MARGIN * 2, WIDGET_HEIGHT));
//
//    QFont f = font();
//    f.setPixelSize(FONT_SIZE);
//    setFont(f);
//
//    QFontMetrics metrics(f);
//    qDebug() << "----" << metrics.boundingRect(originRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, warningData.text);
//
//    textRect = fontMetrics().boundingRect(originRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, warningData.text);
//    int strWidth = fontMetrics().horizontalAdvance(warningData.text);
//    int strHeight = fontMetrics().lineSpacing();
//
//    int drawTextHeight = ( strWidth / (WIDGET_WIDTH - TEXT_MARGIN * 2) + 2) * strHeight;
//    QRect newtextRect = QRect(originRect.topLeft(), QSize(originRect.width(), drawTextHeight));
//    qDebug() << strWidth << strHeight;
//    qDebug() << "origin rect" << originRect;
//    qDebug() << "new rect" << textRect;
//    qDebug() << "newtextRect" << newtextRect;
//
//    setFixedSize(WIDGET_WIDTH, textRect.height() + TRIANGLE_HEIGHT + TEXT_MARGIN * 2);
}

void WarningToolTip::setWarningData(const CityWarningWeather &cityWarningData)
{
    warningData = cityWarningData;
    textLabel->setText(warningData.text);
//    setFixedSize(WIDGET_WIDTH, textLabel->sizeHint().height() + 20);
}

void WarningToolTip::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintBackground();
    paintForeground();
}

void WarningToolTip::paintBackground()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QApplication::palette(this).alternateBase());
//    painter.setBrush(QColor(66, 66, 66));

    QPoint trianglePoint1(width() / 2, 0);
    QPoint trianglePoint2((width() - TRIANGLE_WIDTH) / 2, TRIANGLE_HEIGHT);
    QPoint trianglePoint3((width() + TRIANGLE_WIDTH) / 2, TRIANGLE_HEIGHT);
    QPolygon trianglePolygon;
    trianglePolygon << trianglePoint1 << trianglePoint2 << trianglePoint3;
    painter.drawPolygon(trianglePolygon);

    /* 圆角矩形 */
    int PADDING = 2;
    painter.drawRoundedRect(rect().x() + PADDING, rect().y() + TRIANGLE_HEIGHT,
                            rect().width() - PADDING * 2, rect().height() - TRIANGLE_HEIGHT - PADDING,
                            8, 8);
}

void WarningToolTip::paintForeground()
{
}
