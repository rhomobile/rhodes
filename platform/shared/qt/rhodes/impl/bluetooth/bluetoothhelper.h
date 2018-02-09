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
class BluetoothHelper : public QObject
{
    Q_OBJECT

    explicit BluetoothHelper(QObject *parent = 0) : QObject(parent)
    {
        QList<QObject *> objectList;
        objectList.append(this);
        QtMainWindow::setContextProperty("bluetoothModel", objectList);
        QtMainWindow::setContextProperty("bluetoothListModel", QList<QObject*>());

        connect(&timerRefresh, SIGNAL(timeout()), this, SLOT(referesh()));
        lastError = "OK";
    }
public:
    static BluetoothHelper * getInstance(){
        static BluetoothHelper instance;
        return &instance;
    }

    BluetoothSender * getSession(const char* connected_device_name){
        QString name = QString::fromLatin1(connected_device_name);
        if (keeper.contains(name)){
            return keeper.value(name);
        }else{
            return nullptr;
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
    QString getLastError() const{return lastError;}
    QString getDeviceName(){return hostInfo.name();}
    void setDeviceName(QString name){hostInfo.setName(name);}

    BluetoothServer * createServer(QString clientName, QString callback){
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
        keeper.insert(getDeviceName(), server);
        return server;
    }


    BluetoothClient * createClient(QBluetoothDeviceInfo info, QString callback){
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
            if ((vals->getInfo().address().toString() == serverName) ||
                (vals->getInfo().name() == serverName)){
                info = vals->getInfo();
            }
        }

        return createClient(info, callback);
    }

private:
    QString lastSavedCallback;
    QTimer timerRefresh;
    QHash<QString, BluetoothDeviceLabel *> devicesKeeper;
    QBluetoothDeviceDiscoveryAgent discoverer;
    QString lastError;
    QBluetoothHostInfo hostInfo;

    QHash<QString, BluetoothSender *> keeper;

signals:

public slots:
    void cancel(){
        BluetoothSender::fireCancel(lastSavedCallback);
        stop();
    }
    void stop(){
        timerRefresh.stop();
        discoverer.stop();
    }
    void selected(){
        QBluetoothDeviceInfo info = qobject_cast<BluetoothDeviceLabel *>(sender())->getInfo();
        createClient(info, lastSavedCallback);
    }

    void refresh(){
        QBluetoothLocalDevice localDevice;
        QList<QBluetoothAddress> remotes;
        if (localDevice.isValid()) {
            localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
            remotes = localDevice.connectedDevices();
        }
        QList<QBluetoothDeviceInfo> infos;
        foreach (QBluetoothDeviceInfo info, discoverer.discoveredDevices()) {
            if (remotes.contains(info.address())){infos.append(info);}
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
            QtMainWindow::getView()->update();

        }



    }
};

#endif // BLUETOOTHHELPER_H






