#include <QPainter>
#include <QVariant>
#include <QModelIndex>
#include <QDebug>
#include <QListView>
#include <QColor>

#include "Global.h"
#include "CommonHelper.h"
#include "FutureWeatherItemDelegate.h"
#include "FutureWeatherItemDelegate.h"

FutureWeatherItemDelegate::FutureWeatherItemDelegate(QWidget *parent) : parentWidget(parent)
{
}

void FutureWeatherItemDelegate::paintItemBackground(QPainter *painter, const QStyleOptionViewItem &option) const
{
    if (true == option.state.testFlag(QStyle::State_MouseOver)) {
        QRect rect(option.rect.topLeft() + QPoint(0, 5), option.rect.bottomRight() - QPoint(0, 5));
        painter->save();
        painter->setPen(Qt::transparent);
        painter->setBrush(QColor(66, 66, 66));
        painter->drawRoundedRect(rect, 10, 10);
        painter->restore();
    }
/* 下面的代码会导致鼠标的hover状态与pagedown按钮共存 */
//    QPoint mousePoint = parentWidget->mapFromGlobal(QCursor::pos());
//    if (true == option.rect.contains(mousePoint)) {
//        QRect rect(option.rect.topLeft() + QPoint(0, 5), option.rect.bottomRight() - QPoint(0, 5));
//        painter->save();
//        painter->setPen(Qt::transparent);
//        painter->setBrush(QColor(66, 66, 66));
//        painter->drawRoundedRect(rect, 10, 10);
//        painter->restore();
//    }
}

void FutureWeatherItemDelegate::paintDayWeather(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CityFutureWeather futureWeather = index.data(Qt::DisplayRole).value<CityFutureWeather>();
    QRect rect(option.rect.topLeft() + QPoint(0, 30), option.rect.bottomRight() - QPoint(0, 5));

    CommonHelper::initPainter(*painter, "white", 13);
    /* 绘制日期 */
    QRect dayRect = CommonHelper::textPaintRect(painter->font(), rect.topLeft(), futureWeather.day);
    QRect newDayRect = CommonHelper::adjustTextRect(dayRect, option.rect);
    painter->drawText(newDayRect, Qt::AlignCenter | Qt::TextWordWrap, futureWeather.day);
    /* 绘制星期 */
    QRect weekRect = CommonHelper::textPaintRect(painter->font(), newDayRect.bottomLeft() + QPoint(0, 20), futureWeather.week);
    QRect newWeekRect = CommonHelper::adjustTextRect(weekRect, option.rect);
    painter->drawText(newWeekRect, Qt::AlignCenter | Qt::TextWordWrap, futureWeather.week);
    /* 绘制天气图标 */
    int padding = 10;
    QPoint iconTopLeft = newWeekRect.bottomLeft() + QPoint(padding, 20);
    int iconSize = option.rect.width() - padding * 2;
    QRect iconRect(iconTopLeft, QSize(iconSize, iconSize));
    QString iconPath = QString(":/weather_icon/weather_icon/%1.png").arg(futureWeather.weatherId);
    painter->drawImage(iconRect, QImage(iconPath));
    /* 绘制天气说明 */
    QPoint weatherPoint(iconRect.bottomLeft() - QPoint(padding, 0) + QPoint(0, 20));
    QRect weatherRect = CommonHelper::textPaintRect(painter->font(), weatherPoint, futureWeather.weatherDesc);
    QRect newWeatherRect = CommonHelper::adjustTextRect(weatherRect, option.rect);
    painter->drawText(newWeatherRect, Qt::AlignCenter | Qt::TextWordWrap, futureWeather.weatherDesc);
    /* 绘制温度 */
    QString tempStr = QString("%1/%2℃").arg(futureWeather.temperatureLow).arg(futureWeather.temperatureHigh);
    QPoint tempPoint(newWeatherRect.bottomLeft() + QPoint(0, 20));
    QRect tempRect = CommonHelper::textPaintRect(painter->font(), tempPoint, tempStr);
    QRect newTempRect = CommonHelper::adjustTextRect(tempRect, option.rect);
    painter->drawText(newTempRect, Qt::AlignCenter | Qt::TextWordWrap, tempStr);
    /* 绘制风力 */
    QString windStr = QString::fromLocal8Bit("风力") + QString("%1").arg(futureWeather.windSpeed) + QString::fromLocal8Bit("级");
    QPoint windPoint(newTempRect.bottomLeft() + QPoint(0, 15));
    QRect windRect = CommonHelper::textPaintRect(painter->font(), windPoint, windStr);
    QRect newWindRect = CommonHelper::adjustTextRect(windRect, option.rect);
    painter->drawText(newWindRect, Qt::AlignCenter | Qt::TextWordWrap, windStr);
}

void FutureWeatherItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid() && index.data(Qt::DisplayRole).canConvert<CityFutureWeather>()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

#ifdef QT_DEBUG
//    painter->save();
//    painter->setPen(Qt::white);
//    painter->drawRect(option.rect);
//    painter->restore();
#endif
        paintItemBackground(painter, option);
        paintDayWeather(painter, option, index);
        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize FutureWeatherItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid() && index.data(Qt::DisplayRole).canConvert<CityFutureWeather>()) {
        return QSize(100, parentWidget->height());
    } else {
        return option.rect.size();
    }
}
