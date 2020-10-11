#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class FramelessWidget;
class TitleWidget;

class AboutDialog: public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initUI();
    void initTitleWidget();
    void initCenterWidget();

    FramelessWidget *framelessWidget = nullptr;
    TitleWidget     *titleWidget = nullptr;
    QWidget         *centerWidget = nullptr;
};

#endif // ABOUTDIALOG_H
