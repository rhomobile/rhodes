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
        qDebug() << "Client addreass " + info.address().toString();
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice.address(), this);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothClient::deviceSearchFinished);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothClient::deviceDiscovered);
        connect(&localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothClient::pairingFinished);
        connect(&localDevice, &QBluetoothLocalDevice::error, this, &BluetoothClient::pairingError);
        discoveryAgent->start();
        //requestPairing(info.address());
    }
    ~BluetoothClient() {
        stopClient();
    }


private:

    QString message;
    QString callback;
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent * discoveryAgent;


    void requestPairing(const QBluetoothAddress &address) {
        qDebug() << "requestPairing";
        if ((localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) ||
                (localDevice.pairingStatus(address) == QBluetoothLocalDevice::AuthorizedPaired)) {
            qDebug() << "Already paired";
            startClient(address);
        }else{
            qDebug() << "Trying to pair";
            localDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
        }

    }



    void startClient(const QBluetoothAddress &address) {
        qDebug() << "startClient() " + address.toString();
        if (socket != nullptr) {
            socket->disconnectFromService();
            delete socket;
        }
        socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
        qDebug() << "Socket created";
        connect(socket, &QBluetoothSocket::connected, this, &BluetoothClient::socketConnected);
        connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothClient::socketDisconnected);
        connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothClient::readSocket);
        qDebug() << "Trying to connect to service";
        socket->connectToService(address, QBluetoothUuid(QBluetoothUuid::SerialPort));


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

    void pairingFinished(const QBluetoothAddress &address,
                                          QBluetoothLocalDevice::Pairing pairing) {
        qDebug() << "pairingFinished()";
        startClient(address);
    }

    void pairingError(QBluetoothLocalDevice::Error error) {
        qDebug() << "pairingError()";
    }

    void stopDiscovery() {
        qDebug() << "stopDiscovery()";
        discoveryAgent->stop();
    }

    void deviceSearchFinished() {
        qDebug() << "deviceSearchFinished()";
        if (socket == NULL) {
            qDebug() << "Device not found";
        }
    }


    void deviceDiscovered(const QBluetoothDeviceInfo &deviceInfo) {
        qDebug() << "deviceDiscovered() - " + deviceInfo.name() + " - " + deviceInfo.address().toString();

        if (deviceInfo.address().toString() == info.address().toString()){
            qDebug() << "Found target device - " + info.name();
            if (deviceInfo.serviceUuids().contains(QBluetoothUuid(QBluetoothUuid::SerialPort))) {
                requestPairing(deviceInfo.address());
                discoveryAgent->stop();
            }else{
                qDebug() << "Device does't contains serial service";
            }
        }else{
            qDebug() << "Not interested in " + deviceInfo.name();
        }
    }

    void socketConnected() {
        qDebug() << "socketConnected()";
        connected();
    }
    void socketDisconnected(){
        qDebug() << "socketDisconnected()";
        disconnected();
    }

    void readSocket() {
        qDebug() << "readSocket()";
        messageReceived(QString::fromUtf8(socket->readAll()));
    }
};


#endif // BLUETOOTHCLIENT_H
