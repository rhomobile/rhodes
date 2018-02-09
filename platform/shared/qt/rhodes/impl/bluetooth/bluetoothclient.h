#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <QObject>
#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusVariant>
#include "bluetoothsender.h"

class BluetoothClient : public BluetoothSender {
    Q_OBJECT
public:
    explicit BluetoothClient(QBluetoothDeviceInfo & info, QString callback, QObject *parent = 0) : BluetoothSender(info, callback, parent)  {
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
        setName(info.name());
        requestPairing(info.address());
    }
    ~BluetoothClient() {
        stopClient();
    }


private:
    const QString SERVICE_UUID = "00000000-0000-1000-8000-00805F9B34FB";
    QString message;
    QString callback;


    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceInfo info;


    bool requestPairing(const QBluetoothAddress &address) {
        if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
            startClient(address);
            return true;
        }
        return false;
    }
    void startClient(const QBluetoothAddress &address) {
        qDebug() << "startClient()";
        if (socket != nullptr) {
            socket->disconnectFromService();
            delete socket;
        }
        socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(socket, &QBluetoothSocket::connected, this, &BluetoothClient::socketConnected);
        connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothClient::socketDisconnected);
        connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothClient::readSocket);
        socket->connectToService(address, 1);
    }
    void stopClient() {
        qDebug() << "stopClient()";
        if (socket != NULL) {
            socket->disconnectFromService();
            delete socket;
        }
        socket = NULL;
    }

private slots:
    /*void deviceDiscovered(const QBluetoothDeviceInfo &deviceInfo) {
        qDebug() << "deviceDiscovered()";
        qDebug() << deviceInfo.name();
        if (deviceInfo.serviceUuids().contains(QBluetoothUuid(SERVICE_UUID))) {
            emit clientStatusChanged("Device found");
            discoveryAgent->stop();
            requestPairing(deviceInfo.address());
        }
    }*/

    void socketConnected() {
        qDebug() << "socketConnected()";
        connected();
    }
    void socketDisconnected(){
        disconnected();
    }

    void readSocket() {
        qDebug() << "readSocket()";
        messageReceived(QString::fromUtf8(socket->readAll()));
    }
};


#endif // BLUETOOTHCLIENT_H
