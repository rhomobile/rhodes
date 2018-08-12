#ifndef BLUETOOTHDEVICELABEL_H
#define BLUETOOTHDEVICELABEL_H

#include <QObject>
#include <QString>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothUuid>

class BluetoothDeviceLabel : public QObject{

    Q_OBJECT
    Q_PROPERTY(QString deviceName READ getDeviceName CONSTANT)
    Q_PROPERTY(QString deviceId READ getDeviceId CONSTANT)
public:
    explicit BluetoothDeviceLabel(QBluetoothDeviceInfo &info, QObject *parent) : QObject(parent)
    {
        this->info = info;
        deviceName = info.name();
        deviceId = info.address().toString();
    }
    QString deviceName;
    QString deviceId;

    QBluetoothDeviceInfo getInfo() const{
        return info;
    }

private:
    QBluetoothDeviceInfo info;
signals:
    void onSelect();
public slots:
    void clicked(){
        emit onSelect();
    }
    QString getDeviceName() {return deviceName;}
    QString getDeviceId(){return deviceId;}
};

#endif // BLUETOOTHDEVICELABEL_H
