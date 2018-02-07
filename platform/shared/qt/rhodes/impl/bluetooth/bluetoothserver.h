#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtDBus/QDBusInterface>

class BluetoothServer : public QObject {
    Q_OBJECT
public:
    explicit BluetoothServer(QObject *parent = 0) : QObject(parent) {
        QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                          "net.connman.Technology", QDBusConnection::systemBus(), this);
        bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));

        QDBusInterface adapterListInterface("org.bluez", "/", "org.bluez.Manager",
                                            QDBusConnection::systemBus(), this);
        QVariant adapterPath = adapterListInterface.call("DefaultAdapter").arguments().at(0);
        QDBusInterface bluetoothAdapter("org.bluez", adapterPath.value<QDBusObjectPath>().path(),
                                        "org.bluez.Adapter", QDBusConnection::systemBus(), this);
        bluetoothAdapter.call("SetProperty", "DiscoverableTimeout", QVariant::fromValue(QDBusVariant(0U)));
        bluetoothAdapter.call("SetProperty", "Discoverable", QVariant::fromValue(QDBusVariant(true)));
    }
    ~BluetoothServer() {
        stopServer();
    }
    Q_INVOKABLE void startServer() {
        qDebug() << "startServer()";
        if (bluetoothServer) return;
        bluetoothServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(bluetoothServer, &QBluetoothServer::newConnection,
                this, &BluetoothServer::clientConnected);
        QBluetoothAddress bluetoothAddress = QBluetoothLocalDevice().address();
        bluetoothServer->listen(bluetoothAddress);

        QBluetoothServiceInfo::Sequence classId;
        classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);
        classId.prepend(QVariant::fromValue(QBluetoothUuid(SERVICE_UUID)));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, "BT message sender");
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription, "Message sender");
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, "tau-technologies.com");
        serviceInfo.setServiceUuid(QBluetoothUuid(SERVICE_UUID));

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
    }
    Q_INVOKABLE void stopServer() {
        qDebug() << "stopServer()";
        if (serviceInfo.isRegistered()) serviceInfo.unregisterService();
        if (bluetoothServer != NULL) delete bluetoothServer;
        bluetoothServer = NULL;
    }

signals:
    void messageReceived(QString message);
private:
    QBluetoothServer *bluetoothServer;
    QBluetoothServiceInfo serviceInfo;
    QBluetoothSocket *socket;

    const QString SERVICE_UUID = "00000000-0000-1000-8000-00805F9B34FB";
private slots:
    void clientConnected() {
        qDebug() << "clientConnected()";
        socket = bluetoothServer->nextPendingConnection();
        connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothServer::readSocket);
        connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothServer::clientDisconnected);
    }
    void clientDisconnected() {
        qDebug() << "clientDisconnected()";
        socket->deleteLater();
        socket = NULL;
    }
    void readSocket() {
        qDebug() << "readSocket()";
        const QString message = QString::fromUtf8(socket->readAll());
        emit messageReceived(message);
    }
};



#endif // BLUETOOTHSERVER_H
