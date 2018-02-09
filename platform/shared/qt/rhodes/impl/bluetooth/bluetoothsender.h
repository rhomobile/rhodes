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
#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"

class BluetoothSender : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothSender(QBluetoothDeviceInfo &info, QString createSessionCallBack, QObject *parent = 0){
        this->info = info;
        this->createSessionCallBack = createSessionCallBack;
    }
    virtual ~BluetoothSender(){

        if (socket != nullptr){
            if (socket->isOpen()){
                socket->close();
            }
        }
    }

    void messageReceived(const QString & message){
        if (!message.isEmpty()){
            messagesKeeper.enqueue(message);
            qDebug() << message;
            fireSessionCallBack(RHO_BT_SESSION_INPUT_DATA_RECEIVED);
        }
    }

    bool sendMessage(const QString & message){
        if (socket != nullptr){
            if (socket->isOpen()){
                socket->write(message.toUtf8());
                return true;
            }else{
                fireSessionCallBack(RHO_BT_SESSION_DISCONNECT);
                return false;
            }
        }
        fireSessionCallBack(RHO_BT_ERROR);
        return false;
    }

    void connected(){
        fireCreateSessionCallBack(RHO_BT_OK);
    }

    void disconnected(){
        fireSessionCallBack(RHO_BT_SESSION_DISCONNECT);
    }

    void setName(const QString &value)
    {
        name = value;
    }

    void setCallBack(const QString &value)
    {
        callBack = value;
    }

    QString getLastMessage(){
        return messagesKeeper.dequeue();
    }

    static void fireCancel(const QString &mCreateSessionCallback){
        if (mCreateSessionCallback.isEmpty()) {return;}
        QString body("&status=\"\"&connected_device_name=" + QString::fromLocal8Bit(RHO_BT_CANCEL));
        fireRhodesCallback(mCreateSessionCallback.toStdString().c_str(), body.toStdString().c_str(), true);
    }

    void fireCreateSessionCallBack(const QString &status) {
        if (createSessionCallBack.isEmpty()) {return;}
        QString body("&status=" + status + "&connected_device_name=" + name);
        fireRhodesCallback(createSessionCallBack.toStdString().c_str(), body.toStdString().c_str(), true);
    }

    void fireSessionCallBack(const QString &event_type) {
        if (callBack.isEmpty()) {return;}
        QString body("&connected_device_name=" + name + "&event_type=" + event_type);
        fireRhodesCallback(callBack.toStdString().c_str(), body.toStdString().c_str(), true);
    }

    static void fireRhodesCallback(const char* callback_url, const char* body, bool in_thread) {
        char* norm_url = rho_http_normalizeurl(callback_url);
        if (in_thread) {rho_net_request_with_data_in_separated_thread(norm_url, body);}
        else {rho_net_request_with_data(norm_url, body);}
        rho_http_free(norm_url);
    }






protected:
    QBluetoothDeviceInfo info;
    QString createSessionCallBack;
    QString callBack;
    QBluetoothSocket *socket = nullptr;
    QString name;
    QQueue<QString> messagesKeeper;
signals:

public slots:
};

#endif // BLUETOOTHSENDER_H




