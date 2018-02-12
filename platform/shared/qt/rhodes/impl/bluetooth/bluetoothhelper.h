#ifndef BLUETOOTHHELPER_H
#define BLUETOOTHHELPER_H


#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QString>
#include "../sailfish/src/QtMainWindow.h"
#include <QTimer>
#include <QBluetoothHostInfo>
#include "bluetoothdevicelabel.h"
#include "bluetoothclient.h"
#include "bluetoothserver.h"
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
        QtMainWindow::setContextProperty("bluetoothModel", objectList);
        QtMainWindow::setContextProperty("bluetoothListModel", QList<QObject*>());

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
        if (keeper.contains(connected_device_name)){
            return keeper.value(connected_device_name);
        }else{
            return nullptr;
        }
    }

    QString getLastError() const{return lastError;}
    QString getDeviceName(){
        if (hostInfo.name().isEmpty()){
            hostInfo.setName("nemo");
        }
        return hostInfo.name();

    }

private:
    QString lastSavedCallback;
    QTimer timerRefresh;
    QHash<QString, BluetoothDeviceLabel *> devicesKeeper;
    QBluetoothDeviceDiscoveryAgent discoverer;
    QString lastError;
    QBluetoothHostInfo hostInfo;
    QBluetoothLocalDevice localDevice;

    QHash<QString, BluetoothSender *> keeper;

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

    void setCallback(QString connected_device_name, QString callback){
        qDebug() << connected_device_name << callback;
        BluetoothSender * session = getSession(connected_device_name);
        if (session != nullptr){
            session->setCallBack(callback);
        }
    }

    void remove(const char* connected_device_name){
        QString name = QString::fromLatin1(connected_device_name);
        if (keeper.contains(name)){
            keeper.value(name)->deleteLater();
            keeper.remove(name);
        }
    }

    void setWorking(bool enable){
        qDebug() << "Set working " + QString::number(enable);
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(enable)));
    }

    void setVisible(bool visible, quint32 timerInMs = 0){
        QDBusInterface adapterListInterface("org.bluez", "/", "org.bluez.Manager", QDBusConnection::systemBus(), this);
        QVariant adapterPath = adapterListInterface.call("DefaultAdapter").arguments().at(0);

        QDBusInterface bluetoothAdapter("org.bluez", adapterPath.value<QDBusObjectPath>().path(),
          "org.bluez.Adapter", QDBusConnection::systemBus(), this);
        bluetoothAdapter.call("SetProperty", "DiscoverableTimeout", QVariant::fromValue(QDBusVariant(timerInMs)));
        bluetoothAdapter.call("SetProperty", "Discoverable", QVariant::fromValue(QDBusVariant(visible)));
    }

    void openDeviceDiscover(QString callbackUrl){
        lastSavedCallback = callbackUrl;
        discoverer.start();
        refresh();
        timerRefresh.start(1000);
        QtMainWindow::getLastInstance()->openQMLDocument("BluetoothObserverPage.qml");
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
        createClient(info, lastSavedCallback);
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
            QtMainWindow::setContextProperty("bluetoothListModel", objectList);

            foreach (BluetoothDeviceLabel * val, oldLabels) {val->deleteLater();}

        }

    }


    BluetoothServer * createServer(QString clientName, QString callback){
        qDebug() << "createServer" << clientName << callback;

        refresh();
        if (keeper.contains(clientName)){
            keeper.value(clientName)->deleteLater();
        }

        QBluetoothDeviceInfo info;
        foreach (BluetoothDeviceLabel * vals, devicesKeeper.values()) {
            if ((vals->getInfo().address().toString() == clientName) ||
                (vals->getInfo().name() == clientName)){
                info = vals->getInfo();
            }
        }


        BluetoothServer * server = new BluetoothServer(info, callback, this);
        server->setName(getDeviceName());
        keeper.insert(getDeviceName(), server);
        qDebug() << "Server name is " + getDeviceName();
        return server;
    }


    BluetoothClient * createClient(QBluetoothDeviceInfo info, QString callback){
        qDebug() << info.name() << callback;
        if (keeper.contains(info.name())){
            keeper.value(info.name())->deleteLater();
            keeper.remove(info.name());
        }
        if (keeper.contains(info.address().toString())){
            keeper.value(info.address().toString())->deleteLater();
            keeper.remove(info.address().toString());
        }

        BluetoothClient * client = new BluetoothClient(info, callback, this);

        keeper.insert(info.name(), client);
        return client;
    }

    BluetoothClient * createClient(QString serverName, QString callback){
        refresh();

        QBluetoothDeviceInfo info;
        foreach (BluetoothDeviceLabel * vals, devicesKeeper.values()) {
            if ((vals->getInfo().address().toString() == serverName) || (vals->getInfo().name() == serverName)){
                info = vals->getInfo();
            }
        }

        return createClient(info, callback);
    }
};

#endif // BLUETOOTHHELPER_H






