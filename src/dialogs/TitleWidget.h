#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

class QIcon;
class QLabel;
class QMenu;
class QPushButton;

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    TitleWidget(QString iconPath, QString title, bool showMenuButton, bool showMiniButton, bool showMaxButton, QWidget *parent);

    void setTitleMenu(QMenu *menu);

private:
    void initUI();
    void initConnect();

    QLabel      *titleIconLabel = nullptr;
    QLabel      *titleStrLabel = nullptr;
    QPushButton *titleMenuButton = nullptr;
    QPushButton *titleMiniButton = nullptr;
    QPushButton *titleRestoreButton = nullptr;
    QPushButton *titleMaxButton = nullptr;
    QPushButton *titleCloseButton = nullptr;

    QString     titleIconPath;
    QString     titleStr;
    bool        showMenuButton = false;
    bool        showMiniButton = false;
    bool        showMaxButton = false;
    bool        showCloseButton = true;
};

#endif // TITLEWIDGET_H
