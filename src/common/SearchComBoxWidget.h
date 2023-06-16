#ifndef SEARCHCOMBOXWIDGET_H
#define SEARCHCOMBOXWIDGET_H

#include <QWidget>

class QComboBox;
class QTimer;

class SearchComBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchComBoxWidget(QWidget *parent = nullptr);

private slots:
    void slotSearchCity(const QString& name);
    void slotParseSearchCity();
    void slotAddCity(int index);

private:
    void initUI();
    void initConnect();
    void initTimer();

    QComboBox       *comboBox = nullptr;
    QTimer          *timer;
    QString         currentText;
};

#endif // SEARCHCOMBOXWIDGET_H
