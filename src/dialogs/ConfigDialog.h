#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

class FramelessWidget;
class TitleWidget;
class QPushButton;
class QTextEdit;
class QListWidget;

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void slotOpenWeb();

private:
    void initUI();
    void initConnect();
    void initBaseConfigWidget();
    void initApiConfigWidget();
    void initTitleWidget();
    void initCenterWidget();

    FramelessWidget *framelessWidget = nullptr;
    TitleWidget     *titleWidget = nullptr;
    QWidget         *centerWidget = nullptr;
    QListWidget     *leftListNavigation = nullptr;
    QWidget         *baseConfigWidget = nullptr;

    QWidget         *apiConfigWidget = nullptr;
    QTextEdit       *apiKeyEdit = nullptr;
    QTextEdit       *apiPublicIdEdit = nullptr;
    QPushButton     *visitWebButton = nullptr;
};

#endif // CONFIGDIALOG_H
