#ifndef FUTUREWEATHERITEMDELEGATE_H
#define FUTUREWEATHERITEMDELEGATE_H

#include <QStyledItemDelegate>

class FutureWeatherItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FutureWeatherItemDelegate(QWidget *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    void paintItemBackground(QPainter *painter, const QStyleOptionViewItem &option) const;
    void paintDayWeather(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *parentWidget = nullptr;
};

#endif // FUTUREWEATHERITEMDELEGATE_H
