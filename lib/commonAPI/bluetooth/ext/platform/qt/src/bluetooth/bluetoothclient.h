#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <QObject>
#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>
#ifdef OS_SAILFISH
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusVariant>
#endif
#include "bluetoothsender.h"

class BluetoothClient : public BluetoothSender {
    Q_OBJECT
public:
    explicit BluetoothClient(QBluetoothDeviceInfo & info, QString name,
                             rho::apiGenerator::CMethodResult createSessionCallBack, QObject *parent) :
        BluetoothSender(info, createSessionCallBack, parent)  {
        #ifdef OS_SAILFISH
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
        #endif
        setName(name);
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
    int trying = 0;

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
        connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
        qDebug() << "Trying to connect to service";
        socket->connectToService(info.address(), QBluetoothUuid(SERVICE_UUID));
        //socket->connectToService(address, 0);

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

    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing) {
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
            if (trying < 10){
                trying++;
                discoveryAgent->start();
            }else{
                qDebug() << "Device not found";
            }
        }
    }



    void deviceDiscovered(const QBluetoothDeviceInfo &deviceInfo) {
        qDebug() << "deviceDiscovered() - " + deviceInfo.name() + " - " + deviceInfo.address().toString();

        if (deviceInfo.address().toString() == info.address().toString() || deviceInfo.name() == info.name() ||
            deviceInfo.address().toString() == name || deviceInfo.name() == name){
            info = deviceInfo;
            qDebug() << "Found target device - " + deviceInfo.name();
            foreach (QBluetoothUuid val, deviceInfo.serviceUuids()) {
                qDebug() << "Available service: " + val.toString();
            }
            if (deviceInfo.serviceUuids().contains(QBluetoothUuid(QBluetoothUuid::SerialPort))) {
                requestPairing(deviceInfo.address());
            }else{
                qDebug() << "Device does't contains serial service";
            }
            discoveryAgent->stop();
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

    void socketError(QBluetoothSocket::SocketError error){
        QString msg;
        switch (error) {
            case QBluetoothSocket::UnknownSocketError:
                msg = "An unknown error has occurred.";
                break;
            case QBluetoothSocket::NoSocketError:
                msg = "No error. Used for testing.";
                break;
            case QBluetoothSocket::HostNotFoundError:
                msg = "Could not find the remote host.";
                break;
            case QBluetoothSocket::ServiceNotFoundError:
                msg = "Could not find the service UUID on remote host.";
                break;
            case QBluetoothSocket::NetworkError:
                msg = "Attempt to read or write from socket returned an error";
                break;
            case QBluetoothSocket::UnsupportedProtocolError:
                msg = "The Protocol is not supported on this platform.";
                break;
            case QBluetoothSocket::OperationError:
                msg = "An operation was attempted while the socket was in a state that did not permit it.";
                break;
            default:
                msg = "Unknown error";
                break;
        }
        qDebug() << "Error detected: " + msg;
    }

};


#endif // BLUETOOTHCLIENT_H
