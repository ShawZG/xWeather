#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <QObject>

class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    static SignalManager* instance();

signals:
    void sigShowAddCityDialog();
    void sigAddCity(QString cityName, QString cityId);
    void sigDelCity(QString cityName, QString cityId);
    void sigShowCity(QString cityName, QString cityId);

private:
    Q_DISABLE_COPY(SignalManager)
};

#endif // SIGNALMANAGER_H
