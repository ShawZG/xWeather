#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QMap>

class FramelessWidget;
class TitleWidget;
class WeatherWidget;
class QMenu;
class QStackedWidget;
class QPushButton;

class MainDialog : public QDialog
{
    Q_OBJECT
public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void slotShowCityManagerDialog();

    void slotShowAboutDialog();
    void slotShowConfigDialog();

    void slotAddWeatherWidget(const QString &cityName, const QString &cityId);
    void slotShowWeatherWidget(const QString &cityName, const QString &cityId);
    void slotDelWeatherWidget(const QString &cityName, const QString &cityId);

    void slotShowPrevCityWeather();
    void slotShowNextCityWeather();

private:
    void initUI();
    void initConnect();
    void initTitleWidget();
    void initTitleMenu();
    void initMainWidget();

    FramelessWidget     *framelessWidget = nullptr;
    TitleWidget         *titleWidget = nullptr;
    QMenu               *titleMenu = nullptr;
    QWidget             *centerWidget = nullptr;
    QPushButton         *pageUpButton = nullptr;
    QPushButton         *pageDownButton = nullptr;
    QStackedWidget      *weatherStackWidget = nullptr;

    QList<WeatherWidget*>   weatherWidgetList;
};

#endif // MAINDIALOG_H
