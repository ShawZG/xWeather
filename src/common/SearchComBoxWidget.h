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
    void slotSearchCity(QString name);
    void slotParseSearchCity();

private:
    void initUI();
    void initConnect();
    void initTimer();

    QComboBox       *comboBox = nullptr;
    QTimer          *timer;
    QString         currentText;
};

#endif // SEARCHCOMBOXWIDGET_H
