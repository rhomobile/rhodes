#ifndef BLUETOOTHHELPER_H
#define BLUETOOTHHELPER_H

#include <QObject>
#ifdef OS_SAILFISH
#include <QtDBus/QDBusInterface>
#endif
#include "bluetoothdevicelabel.h"
#include "bluetoothclient.h"
#include "bluetoothserver.h"
#include <QString>
#include "mainwindowinterface.h"
#include <QTimer>
#include <QBluetoothHostInfo>
#include <QQmlContext>
#include <QQuickView>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>

class BluetoothHelper : public QObject
{
    Q_OBJECT

    explicit BluetoothHelper(QObject *parent = 0) : QObject(parent)
    {
        QList<QObject *> objectList;
        objectList.append(this);
        #ifdef OS_SAILFISH
        QtMainWindow::setContextProperty("bluetoothModel", objectList);
        QtMainWindow::setContextProperty("bluetoothListModel", QList<QObject*>());
        #endif
        connect(&timerRefresh, SIGNAL(timeout()), this, SLOT(refresh()));
        lastError = "OK";
    }
public:
    static BluetoothHelper * getInstance(){
        static BluetoothHelper * instance = nullptr;
        if (instance == nullptr){
            instance = new BluetoothHelper(QtMainWindow::getLastInstance());
        }
        return instance;
    }

    BluetoothSender * getSession(const char* connected_device_name){
        return getSession(QString::fromLatin1(connected_device_name));
    }

    BluetoothSender * getSession(QString connected_device_name){
        QMutexLocker locker(&mutex);
        if (BluetoothSender::getKeeper()->contains(connected_device_name)){
            return BluetoothSender::getKeeper()->value(connected_device_name);
        }else{
            return nullptr;
        }
    }

    QString getLastError() const{return lastError;}
    QString getDeviceName(){
        if (hostInfo.name().isEmpty()){
            hostInfo.setName("defaultuser");
        }
        return hostInfo.name();

    }

private:
    rho::apiGenerator::CMethodResult lastSavedCallback;
    QTimer timerRefresh;
    QHash<QString, BluetoothDeviceLabel *> devicesKeeper;
    QBluetoothDeviceDiscoveryAgent discoverer;
    QString lastError;
    QBluetoothHostInfo hostInfo;
    QBluetoothLocalDevice localDevice;
    QMutex mutex;




signals:

public slots:
    void setDeviceName(QString name){hostInfo.setName(name);}

    void sendMessage(QString connected_device_name, QString message){
        qDebug() << "Sending message" << connected_device_name << message;
        BluetoothSender * session = BluetoothHelper::getInstance()->getSession(connected_device_name);
        if (session != nullptr){
            session->sendMessage(message);
        }else{
            qDebug() << "Can't get a session";
        }
    }

    void setCallback(QString connected_device_name, rho::apiGenerator::CMethodResult callback){
        qDebug() << connected_device_name;
        BluetoothSender * session = getSession(connected_device_name);
        if (session != nullptr){
            session->setCallBack(callback);
        }
    }

    void remove(QString & connected_device_name){
        QMutexLocker locker(&mutex);
        if (BluetoothSender::getKeeper()->contains(connected_device_name)){
            BluetoothSender::getKeeper()->value(connected_device_name)->deleteLater();
            BluetoothSender::getKeeper()->remove(connected_device_name);
        }
    }

    void setWorking(bool enable){
        qDebug() << "Set working " + QString::number(enable);
        #ifdef OS_SAILFISH
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(enable)));
        #endif
    }

    void setVisible(bool visible, quint32 timerInMs = 0){
        #ifdef OS_SAILFISH
        QDBusInterface adapterListInterface("org.bluez", "/", "org.bluez.Manager", QDBusConnection::systemBus(), this);
        QVariant adapterPath = adapterListInterface.call("DefaultAdapter").arguments().at(0);

        QDBusInterface bluetoothAdapter("org.bluez", adapterPath.value<QDBusObjectPath>().path(),
          "org.bluez.Adapter", QDBusConnection::systemBus(), this);
        bluetoothAdapter.call("SetProperty", "DiscoverableTimeout", QVariant::fromValue(QDBusVariant(timerInMs)));
        bluetoothAdapter.call("SetProperty", "Discoverable", QVariant::fromValue(QDBusVariant(visible)));
        #endif
    }

    void openDeviceDiscover(rho::apiGenerator::CMethodResult callback){
        lastSavedCallback = callback;
        discoverer.start();
        refresh();
        timerRefresh.start(1000);
        #ifdef OS_SAILFISH
        QtMainWindow::getLastInstance()->openQMLDocument("BluetoothObserverPage.qml");
        #endif
    }


    void cancel(){
        qDebug() << "Cancelling";
        BluetoothSender::fireCancel(lastSavedCallback);
        stop();
    }
    void stop(){
        timerRefresh.stop();
        discoverer.stop();
    }
    void selected(){
        stop();
        QBluetoothDeviceInfo info = qobject_cast<BluetoothDeviceLabel *>(sender())->getInfo();
        createClient(info, info.name(), lastSavedCallback);
    }

    void refresh(){
        QList<QBluetoothDeviceInfo> infos;

        foreach (QBluetoothDeviceInfo info, discoverer.discoveredDevices()) {
            QBluetoothLocalDevice::Pairing pairingStatus = localDevice.pairingStatus(info.address());
            if (pairingStatus == QBluetoothLocalDevice::Paired ||
                    pairingStatus == QBluetoothLocalDevice::AuthorizedPaired ){
                infos.append(info);
            }
        }

        foreach (QBluetoothDeviceInfo info, infos) {
            qDebug() << QTime::currentTime().toString() + ": found device: " + info.name() + " : " + info.address().toString();
        }

        bool allContains = true;
        if (infos.size() != devicesKeeper.keys().size()){
            allContains = false;
        }else{
            foreach (QBluetoothDeviceInfo info, infos) {
                if (!devicesKeeper.contains(info.address().toString())){
                    allContains = false;
                }
            }
        }
        if (!allContains){
            QList<BluetoothDeviceLabel *> oldLabels = devicesKeeper.values();
            devicesKeeper.clear();
            foreach (QBluetoothDeviceInfo info, infos) {
                devicesKeeper.insert(info.address().toString(), new BluetoothDeviceLabel(info, this));
            }

            QList<QObject *> objectList;
            foreach (BluetoothDeviceLabel * val, devicesKeeper.values()) {
                objectList.append(val);
                connect(val, SIGNAL(onSelect()), this, SLOT(selected()));
            }
            #ifdef OS_SAILFISH
            QtMainWindow::setContextProperty("bluetoothListModel", objectList);
            #endif
            foreach (BluetoothDeviceLabel * val, oldLabels) {val->deleteLater();}

        }

    }


    BluetoothServer * createServer(QString clientName, rho::apiGenerator::CMethodResult callback){
        qDebug() << "createServer" << clientName;

        refresh();

        QBluetoothDeviceInfo info;
        foreach (BluetoothDeviceLabel * vals, devicesKeeper.values()) {
            if ((vals->getInfo().address().toString() == clientName) ||
                (vals->getInfo().name() == clientName)){
                info = vals->getInfo();
            }
        }

        BluetoothServer * server = new BluetoothServer(info, callback, this);

        return server;
    }


    BluetoothClient * createClient(QBluetoothDeviceInfo info, QString name, rho::apiGenerator::CMethodResult callback){
        qDebug() << info.name();

        BluetoothClient * client = new BluetoothClient(info, name, callback, this);
        return client;
    }

    BluetoothClient * createClient(QString serverName, rho::apiGenerator::CMethodResult callback){
        refresh();


        QBluetoothDeviceInfo info;
        foreach (BluetoothDeviceLabel * vals, devicesKeeper.values()) {
            qDebug() << "Avalable device info: " + info.name() + " " + info.address().toString();
            if ((vals->getInfo().address().toString() == serverName) || (vals->getInfo().name() == serverName)){
                info = vals->getInfo();
                qDebug() << "Target device info: " + info.name() + " " + info.address().toString();
            }
        }

        return createClient(info, serverName, callback);
    }
};

#endif // BLUETOOTHHELPER_H






