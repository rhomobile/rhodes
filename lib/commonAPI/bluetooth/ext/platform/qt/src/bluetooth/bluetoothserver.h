#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothServiceInfo>
#ifdef OS_SAILFISH
#include <QtDBus/QDBusInterface>
#endif
#include <QtBluetooth/QBluetoothDeviceInfo>
#include "bluetoothsender.h"
class BluetoothServer : public BluetoothSender {
    Q_OBJECT
public:
    explicit BluetoothServer(QBluetoothDeviceInfo &info, rho::apiGenerator::CMethodResult &createSessionCallBack, QObject *parent = 0) :
        BluetoothSender(info, createSessionCallBack, parent) {
        bluetoothServer = nullptr;
#ifdef OS_SAILFISH
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
#endif

        startServer();
    }
    ~BluetoothServer() {
        stopServer();
    }


private:
    QBluetoothServer *bluetoothServer;
    QBluetoothServiceInfo serviceInfo;


private slots:
    void startServer() {
        qDebug() << "startServer()";
        if (bluetoothServer) return;
        bluetoothServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(bluetoothServer, &QBluetoothServer::newConnection, this, &BluetoothServer::clientConnected);
        QBluetoothAddress bluetoothAddress = QBluetoothLocalDevice().address();
        QBluetoothHostInfo hostInfo;
        setName(hostInfo.name());
        bluetoothServer->listen(bluetoothAddress);

        QBluetoothServiceInfo::Sequence classId;
        classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);
        classId.prepend(QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort)));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, "BT message sender");
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription, "Message sender");
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, "tau-technologies.com");
        serviceInfo.setServiceUuid(QBluetoothUuid(QBluetoothUuid::SerialPort));

        QBluetoothServiceInfo::Sequence publicBrowse;
        publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

        QBluetoothServiceInfo::Sequence protocol;
        QBluetoothServiceInfo::Sequence protocolDescriptorList;
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
                 << QVariant::fromValue(quint8(bluetoothServer->serverPort()));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);

        serviceInfo.registerService(bluetoothAddress);

        qDebug() << "Service registred";
    }
    void stopServer() {
        qDebug() << "stopServer()";
        if (serviceInfo.isRegistered()) serviceInfo.unregisterService();
        if (bluetoothServer != nullptr) delete bluetoothServer;
        bluetoothServer = nullptr;
    }

    void clientConnected() {
        qDebug() << "clientConnected()";
        socket = bluetoothServer->nextPendingConnection();
        if (info.address().toString() != socket->peerAddress().toString()){
            info = QBluetoothDeviceInfo(socket->peerAddress(), socket->peerName(), 0);
        }
        connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothServer::readSocket);
        connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothServer::clientDisconnected);
        connected();
    }
    void clientDisconnected() {
        qDebug() << "clientDisconnected()";
        socket->deleteLater();
        socket = NULL;
        disconnected();
    }
    void readSocket() {
        qDebug() << "readSocket()";
        messageReceived(QString::fromUtf8(socket->readAll()));
    }

};



#endif // BLUETOOTHSERVER_H
