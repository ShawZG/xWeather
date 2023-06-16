#include <QPainter>
#include <QVariant>
#include <QModelIndex>
#include <QDebug>
#include <QListView>
#include <QColor>

#include "Global.h"
#include "CommonHelper.h"
#include "LifeIndexItemDelegate.h"

LifeIndexItemDelegate::LifeIndexItemDelegate(QWidget *parent) : parentWidget(parent)
{
}

void LifeIndexItemDelegate::paintVerticalSegmentationLine(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    if (0 == index.row() || 4 == index.row()) {
        /* do nothing */
    } else {
        painter->drawLine(option.rect.topLeft() + QPoint(0, 10),
                          option.rect.bottomLeft() - QPoint(0, 10));
    }

    if ( index.row() >= 0 && index.row() <= 3){
        painter->drawLine(option.rect.bottomLeft() + QPoint(10, 0),
                          option.rect.bottomRight() - QPoint(10, 0));
    }
    painter->restore();
}

void LifeIndexItemDelegate::paintLifeIndexTitle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    CityLifeIndex lifeIndex = index.data(Qt::DisplayRole).value<CityLifeIndex>();
    /* 绘制图标 */
    int iconPadding = 20;
    int iconSize = option.rect.height() - iconPadding * 2;
    QRect iconRect = QRect(option.rect.topLeft() + QPoint(25, iconPadding), QSize(iconSize, iconSize));
    painter->drawImage(iconRect, QImage(lifeIndex.icon));
    /* 绘制标题*/
    QPoint titlePoint(iconRect.topRight().x() + 20, iconRect.topRight().y());
    CommonHelper::initPainter(*painter, "white", 13);
    QRect titleRect = CommonHelper::textPaintRect(painter->font(), titlePoint, lifeIndex.title);
    painter->drawText(titleRect, Qt::AlignCenter, lifeIndex.title);
    /* 绘制等级 */
    QPoint levelPoint(titleRect.bottomLeft().x(), titleRect.bottomLeft().y() + 20);
    CommonHelper::initPainter(*painter, "white", 18);
    QRect levelRect = CommonHelper::textPaintRect(painter->font(), levelPoint, lifeIndex.level);
    painter->drawText(levelRect, Qt::AlignCenter, lifeIndex.level);
    painter->restore();
}

void LifeIndexItemDelegate::paintLifeIndexDesc(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    CityLifeIndex lifeIndex = index.data(Qt::DisplayRole).value<CityLifeIndex>();
    /* 绘制描述 */
    QRect descRect(option.rect.topLeft() + QPoint(10, 10), option.rect.bottomRight() - QPoint(10, 10));
    CommonHelper::initPainter(*painter, "white", 12);
    painter->drawText(descRect, Qt::AlignCenter | Qt::TextWordWrap, lifeIndex.desc);
    painter->restore();
}

void LifeIndexItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid() && index.data(Qt::DisplayRole).canConvert<CityLifeIndex>()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

#ifdef QT_DEBUG
//    painter->save();
//    painter->setPen(Qt::white);
//    painter->drawRect(option.rect);
//    painter->restore();
#endif
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            paintLifeIndexDesc(painter, option, index);
        } else {
            paintLifeIndexTitle(painter, option, index);
        }
        paintVerticalSegmentationLine(painter, option, index);
        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize LifeIndexItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid() && index.data(Qt::DisplayRole).canConvert<CityLifeIndex>()) {
        return QSize(250, 100);
    } else {
        return option.rect.size();
    }
}
