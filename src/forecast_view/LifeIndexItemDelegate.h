#ifndef LIFEINDEXITEMDELEGATE_H
#define LIFEINDEXITEMDELEGATE_H

#include <QStyledItemDelegate>

class LifeIndexItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LifeIndexItemDelegate(QWidget *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    void paintVerticalSegmentationLine(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintLifeIndexTitle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintLifeIndexDesc(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *parentWidget = nullptr;
};

#endif // LIFEINDEXITEMDELEGATE_H
