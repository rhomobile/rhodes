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

class BluetoothHelper : public QObject
{
    Q_OBJECT

    explicit BluetoothHelper(QObject *parent = 0) : QObject(parent)
    {
        QList<QObject *> objectList;
        objectList.append(this);
        QtMainWindow::setContextProperty("bluetoothModel", objectList);

        connect(&timerRefresh, SIGNAL(timeout()), this, SLOT(referesh()));
        lastError = "OK";
    }
public:
    BluetoothHelper * getInstance(){
        static BluetoothHelper instance;
        return &instance;
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

    void openDeviceDiscover(){
        timerRefresh.start(1000);
        QtMainWindow::getLastInstance()->openQMLDocument("BluetoothObserverPage.qml");
    }
    QString getLastError() const{return lastError;}
    QString getDeviceName(){return hostInfo.name();}
    void setDeviceName(QString & name){hostInfo.setName(name);}

private:
    QTimer timerRefresh;
    QHash<QString, BluetoothDeviceLabel *> devicesKeeper;
    QBluetoothDeviceDiscoveryAgent discoverer;
    QString lastError;
    QBluetoothHostInfo hostInfo;
signals:

public slots:
    void cancel(){
        stop();
    }
    void stop(){timerRefresh.stop();}
    void selected(){
        stop();
        QBluetoothDeviceInfo = qobject_cast<BluetoothDeviceLabel *>(sender())->getInfo();
    }

    void refresh(){
        QList<QBluetoothDeviceInfo> infos = discoverer.discoveredDevices();
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
                devicesKeeper.insert(info.address().toString(), BluetoothDeviceLabel(info, this));
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
};

#endif // BLUETOOTHHELPER_H






