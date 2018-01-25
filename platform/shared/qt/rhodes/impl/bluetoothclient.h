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

class BluetoothClient : public QObject {
    Q_OBJECT
public:
    explicit BluetoothClient(QObject *parent = 0) : QObject(parent) {
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice.address());
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothClient::deviceDiscovered);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothClient::deviceSearchFinished);
        connect(&localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothClient::pairingFinished);
        connect(&localDevice, &QBluetoothLocalDevice::error, this, &BluetoothClient::pairingError);
    }
    ~BluetoothClient() {
        stopClient();
    }
    Q_INVOKABLE void startDiscovery(const QString &messageToSend) {
        if (socket != NULL) stopClient();
        qDebug() << "startDiscovery()";
        this->message = messageToSend;
        discoveryAgent->start();
        emit clientStatusChanged("Searching for device");
    }
    Q_INVOKABLE void stopDiscovery() {
        qDebug() << "stopDiscovery()";
        discoveryAgent->stop();
    }
private:
    const QString SERVICE_UUID = "1f2d6c5b-6a86-4b30-8b4e-3990043d73f1";
    QString message;
    QBluetoothSocket *socket = NULL;
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothDeviceInfo device;
    QBluetoothLocalDevice localDevice;
    void requestPairing(const QBluetoothAddress &address) {
        qDebug() << "requestPairing()";
        emit clientStatusChanged("Pairing devices");
        if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
            startClient(address);
        } else {
            localDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
        }
    }
    void startClient(const QBluetoothAddress &address) {
        qDebug() << "startClient()";
        if (socket != NULL) {
            socket->disconnectFromService();
            delete socket;
        }
        socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(socket, &QBluetoothSocket::connected, this, &BluetoothClient::socketConnected);
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
signals:
    void messageReceived(QString message);
    void clientStatusChanged(QString text);
private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &deviceInfo) {
        qDebug() << "deviceDiscovered()";
        qDebug() << deviceInfo.name();
        if (deviceInfo.serviceUuids().contains(QBluetoothUuid(SERVICE_UUID))) {
            emit clientStatusChanged("Device found");
            discoveryAgent->stop();
            requestPairing(deviceInfo.address());
        }
    }
    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing) {
        qDebug() << "pairingFinished()";
        startClient(address);
    }
    void pairingError(QBluetoothLocalDevice::Error error) {
        qDebug() << "pairingError()";
        emit clientStatusChanged("Unable to pair devices");
    }
    void socketConnected() {
        qDebug() << "socketConnected()";
        emit clientStatusChanged("Connected to socket");
        socket->write(message.toUtf8());
    }
    void deviceSearchFinished() {
        qDebug() << "deviceSearchFinished()";
        if (socket == NULL) emit clientStatusChanged("Device not found");
    }
    void readSocket() {
        qDebug() << "readSocket()";
        QString receivedMessage = QString::fromUtf8(socket->readLine().trimmed());
        emit messageReceived(receivedMessage);
        emit clientStatusChanged("Message received");
        stopClient();
    }
};


#endif // BLUETOOTHCLIENT_H
