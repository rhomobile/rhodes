#ifndef BLUETOOTHHELPER_H
#define BLUETOOTHHELPER_H

#include <QObject>
#include <QtDBus/QDBusInterface>


class BluetoothHelper : public QObject
{
    Q_OBJECT

    explicit BluetoothHelper(QObject *parent = 0) : QObject(parent)
    {

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

signals:

public slots:
};

#endif // BLUETOOTHHELPER_H
