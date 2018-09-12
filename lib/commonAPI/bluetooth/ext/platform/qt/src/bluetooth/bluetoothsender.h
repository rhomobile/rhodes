#ifndef BLUETOOTHSENDER_H
#define BLUETOOTHSENDER_H

#define RHO_BT_OK  "OK"
#define RHO_BT_CANCEL  "CANCEL"
#define RHO_BT_ERROR  "ERROR"

#define RHO_BT_ROLE_SERVER  "ROLE_SERVER"
#define RHO_BT_ROLE_CLIENT  "ROLE_CLIENT"

#define RHO_BT_SESSION_INPUT_DATA_RECEIVED "SESSION_INPUT_DATA_RECEIVED"
#define RHO_BT_SESSION_DISCONNECT "SESSION_DISCONNECT"

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QtBluetooth/QBluetoothSocket>
#include <QQueue>
//#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

class BluetoothSender : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothSender(QBluetoothDeviceInfo &info, rho::apiGenerator::CMethodResult &createSessionCallBack,
                             QObject *parent) : QObject(parent){
        this->info = info;
        this->createSessionCallBack = createSessionCallBack;
        savedMessageTimer.setSingleShot(true);
        connect(&savedMessageTimer, SIGNAL(timeout()), this, SLOT(sendSavedMessage()));
        qDebug() << "Sender created";
    }
    virtual ~BluetoothSender(){

        if (socket != nullptr){
            if (socket->isOpen()){
                socket->close();
            }
        }
    }

    static QHash<QString, BluetoothSender *> * getKeeper(){
        static QHash<QString, BluetoothSender *> keeper;
        return &keeper;
    }

    void messageReceived(const QString & message){
        qDebug() << message;
        savedMessageTimer.stop();
        if (!message.isEmpty()){
            QMutexLocker locker(&mutex);
            savedMessage.append(message);
        }
        savedMessageTimer.start(50);
    }

    bool sendMessage(const QString & message){
        qDebug() << message;
        if (socket != nullptr){
            if (socket->isOpen()){
                socket->write(message.toUtf8());
                return true;
            }else{
                //fireSessionCallBack(RHO_BT_SESSION_DISCONNECT);
                fireSessionCallBack(RHO_BT_ERROR);
                return false;
            }
        }
        fireSessionCallBack(RHO_BT_ERROR);
        return false;
    }

    void removeSimularDevice(){
        if (getKeeper()->contains(info.name())){
            if (getKeeper()->value(info.name()) != this){
                getKeeper()->value(info.name())->deleteLater();
            }

        }
        if (getKeeper()->contains(info.address().toString())){
            if (getKeeper()->value(info.address().toString()) != this){
                getKeeper()->value(info.address().toString())->deleteLater();
            }
        }
    }

    void connected(){     
        removeSimularDevice();
        setName(info.name());
        getKeeper()->insert(info.name(), this);

        fireCreateSessionCallBack(RHO_BT_OK);
    }

    void disconnected(){
        fireSessionCallBack(RHO_BT_SESSION_DISCONNECT);
    }

    void setName(const QString &value)
    {
        name = value;
    }

    void setCallBack(rho::apiGenerator::CMethodResult &value)
    {
        callBack = value;
    }

    QString getLastMessage(){
        QMutexLocker locker(&mutex);
        if (!messagesKeeper.isEmpty()){
            return std::move(messagesKeeper.dequeue());
        }else{
            return std::move(QString("ERROR"));
        }
    }

    int getQueueSize(){
        QMutexLocker locker(&mutex);
        int msize = messagesKeeper.size();
        if (msize > 0){
            return msize;
        } else if (!socket->isOpen()){
            return -1;
        }
        return 0;
    }

    static void fireCancel(rho::apiGenerator::CMethodResult &mCreateSessionCallback){
        if (!mCreateSessionCallback.hasCallback()) {return;}
        auto map = mCreateSessionCallback.getStringHash();
        map.put("status", RHO_BT_CANCEL);
        mCreateSessionCallback.set(map);
    }

    void fireCreateSessionCallBack(const QString &status) {
        if (!createSessionCallBack.hasCallback()) {return;}
        auto map = createSessionCallBack.getStringHash();
        map.put("status", status.toStdString());
        map.put("connectionID", name.toStdString());
        createSessionCallBack.set(map);
    }

    void fireSessionCallBack(const QString &event_type) {
        if (!callBack.hasCallback()) {return;}
        auto map = callBack.getStringHash();
        map.put("status", event_type.toStdString());
        map.put("connectionID", name.toStdString());
        callBack.set(map);
    }

protected:
    QMutex mutex;
    QBluetoothDeviceInfo info;
    rho::apiGenerator::CMethodResult createSessionCallBack;
    rho::apiGenerator::CMethodResult callBack;
    QBluetoothSocket *socket = nullptr;
    QString name;
    QQueue<QString> messagesKeeper;
    const QString SERVICE_UUID = "00001101-0000-1000-8000-00805F9B34FB";

    QString savedMessage;
    QTimer savedMessageTimer;

signals:

public slots:
    void sendSavedMessage(){
        QMutexLocker locker(&mutex);
        if (!savedMessage.isEmpty()){
            messagesKeeper.enqueue(savedMessage);
            qDebug() << savedMessage;
            savedMessage.clear();
            locker.unlock();
            fireSessionCallBack(RHO_BT_SESSION_INPUT_DATA_RECEIVED);
        }
    }
};

#endif // BLUETOOTHSENDER_H




