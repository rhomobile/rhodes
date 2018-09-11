#ifndef BLUETOOTHTHREAD_H
#define BLUETOOTHTHREAD_H

#include <QThread>
#include "bluetoothhelper.h"

class BluetoothThread : public QThread
{
    Q_OBJECT

    explicit BluetoothThread(QObject *parent = 0) : QThread(parent){
        start(QThread::TimeCriticalPriority);
        msleep(1);
        QMutexLocker locker(&mutex);
    }

public:
    static BluetoothThread * getInstance(){
        static BluetoothThread instance;
        return &instance;
    }
    void run(){
        mutex.lock();
        BluetoothHelper::getInstance();
        connect(this, SIGNAL(sendMessage(QString,QString)), BluetoothHelper::getInstance(), SLOT(sendMessage(QString,QString)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(setCallback(QString,rho::apiGenerator::CMethodResult)), BluetoothHelper::getInstance(), SLOT(setCallback(QString,rho::apiGenerator::CMethodResult)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(openDeviceDiscover(rho::apiGenerator::CMethodResult)), BluetoothHelper::getInstance(), SLOT(openDeviceDiscover(rho::apiGenerator::CMethodResult)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(createClient(QString,rho::apiGenerator::CMethodResult)), BluetoothHelper::getInstance(),
                SLOT(createClient(QString,rho::apiGenerator::CMethodResult)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(createServer(QString,rho::apiGenerator::CMethodResult)), BluetoothHelper::getInstance(),
                SLOT(createServer(QString,rho::apiGenerator::CMethodResult)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(setWorking(bool)), BluetoothHelper::getInstance(), SLOT(setWorking(bool)),
                Qt::QueuedConnection);
        connect(this, SIGNAL(setDeviceName(QString)), BluetoothHelper::getInstance(), SLOT(setDeviceName(QString)),
                Qt::QueuedConnection);


        connect(this, SIGNAL(finished()), BluetoothHelper::getInstance(), SLOT(deleteLater()));

        mutex.unlock();
        exec();
    }

    QString getDeviceName(){
        return BluetoothHelper::getInstance()->getDeviceName();
    }

    BluetoothSender * getSession(QString &connected_device_name){
        return BluetoothHelper::getInstance()->getSession(connected_device_name);
    }

    void remove(QString &connected_device_name){
        BluetoothHelper::getInstance()->remove(connected_device_name);
    }

    QString getLastError(){
        return BluetoothHelper::getInstance()->getLastError();
    }

protected:
    QMutex mutex;
signals:
    void setDeviceName(QString name);
    void setWorking(bool b);
    void sendMessage(QString connected_device_name, QString message);
    void setCallback(QString connected_device_name, rho::apiGenerator::CMethodResult callback);

    void openDeviceDiscover(rho::apiGenerator::CMethodResult callbackUrl);

    void createServer(QString clientName, rho::apiGenerator::CMethodResult oResult);

    void createClient(QString serverName, rho::apiGenerator::CMethodResult oResult);

};

#endif // BLUETOOTHTHREAD_H
