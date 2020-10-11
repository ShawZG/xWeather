#ifndef CITYMANAGERDIALOG_H
#define CITYMANAGERDIALOG_H

#include <Global.h>
#include <QDialog>

class FramelessWidget;
class TitleWidget;
class QGridLayout;
class SearchComBoxWidget;
class QPushButton;
class FavCityWidget;

class CityManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CityManagerDialog(QWidget *parent = nullptr);
    ~CityManagerDialog();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void slotParseTopCity();
    void slotTopCityClicked();

    void slotEditFavCity(bool checked);
    void slotAddFavCity(const QString &cityName, const QString &cityId);
    void slotDelFavCity(const QString &cityName, const QString &cityId);

private:
    void initUI();
    void initConnect();
    void initTitleWidget();
    void initCenterWidget();
    void updateTopCityWidget();
    void initFavCityWidget();
    void updateFavCityWidget();

    void saveFavCityConfig();

    bool isFavCityExits(const QString &cityName, const QString &cityId);
    int getFavCityWidget(const QString &cityName, const QString &cityId);

    void getTopCityList();

    FramelessWidget *framelessWidget = nullptr;
    TitleWidget     *titleWidget = nullptr;
    QWidget         *centerWidget = nullptr;
    QPushButton     *editCityButton = nullptr;

    SearchComBoxWidget  *searchWidget = nullptr;
    QGridLayout         *topCityLayout = nullptr;
    QGridLayout         *favCityLayout = nullptr;

    static QList<TopCity>   topCityList;
    QList<FavCityWidget*>   favCityList;
};

#endif // CITYMANAGERDIALOG_H
