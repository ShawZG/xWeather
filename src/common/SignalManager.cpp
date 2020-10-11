#include "SignalManager.h"

Q_GLOBAL_STATIC(SignalManager, signalManager)

SignalManager::SignalManager(QObject *parent) : QObject(parent)
{
}

SignalManager *SignalManager::instance()
{
    return signalManager;
}
