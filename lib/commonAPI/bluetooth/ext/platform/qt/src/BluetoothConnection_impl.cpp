#include "../../../shared/generated/cpp/BluetoothConnectionBase.h"
#include "bluetooth/bluetooththread.h"

namespace rho {

using namespace apiGenerator;

class CBluetoothConnectionImpl: public CBluetoothConnectionBase
{
public:
    CBluetoothConnectionImpl(const rho::String& strID): CBluetoothConnectionBase()
    {
        connected_device_name = QString::fromStdString(strID);
    }

    virtual void getConnectionID(rho::apiGenerator::CMethodResult& oResult) {
        auto hash = oResult.getStringHash();
        hash.put("connectionID", connected_device_name.toStdString());
        oResult.set(hash);
    } 
    QString connected_device_name;
    virtual void getStatus(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothSender * session = BluetoothThread::getInstance()->getSession(connected_device_name);
        if (session != nullptr){
            qDebug() << "Status is " + QString::number(session->getQueueSize());
            oResult.set(session->getQueueSize());
            return;
        }
        oResult.set(-1);
    } 

    virtual void setCallback(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothThread::getInstance()->setCallback(connected_device_name, oResult);
    } 

    virtual void writeData( const rho::Vector<rho::String>& data, rho::apiGenerator::CMethodResult& oResult) {
        for(int i = 0; i < data.size(); i++){
            BluetoothThread::getInstance()->sendMessage(connected_device_name, QString::fromStdString(data.elementAt(i)));
        }
    } 

    virtual void readData(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothSender * session = BluetoothThread::getInstance()->getSession(connected_device_name);
        if (session != nullptr){
            QString array = session->getLastMessage();
            //oResult.set((unsigned char*)(const_cast<char *>(array.toStdString().c_str())));
            oResult.set(array.toStdString());
        }
    } 

    virtual void writeString( const rho::String& string, rho::apiGenerator::CMethodResult& oResult) {
        BluetoothThread::getInstance()->sendMessage(connected_device_name, QString::fromStdString(string));
    } 

    virtual void readString(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothSender * session = BluetoothThread::getInstance()->getSession(connected_device_name);
        if (session != nullptr){
            QString array = session->getLastMessage();
            qDebug() << "Returnging string: " + array;
            oResult.set(array.toStdString());
        }
    } 

    virtual void disconnect(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothThread::getInstance()->remove(connected_device_name);
    } 

};

class CBluetoothConnectionSingleton: public CBluetoothConnectionSingletonBase
{
    ~CBluetoothConnectionSingleton(){}
    virtual rho::String getInitialDefaultID()
    {
        CMethodResult oRes;
        enumerate(oRes);

        rho::Vector<rho::String>& arIDs = oRes.getStringArray();

        return arIDs[0];
    }
    virtual void enumerate(CMethodResult& oResult)
    {
        rho::Vector<rho::String> arIDs;
        arIDs.addElement("SC1");
        arIDs.addElement("SC2");

        oResult.set(arIDs);
    }

    virtual void getIsAvailable(rho::apiGenerator::CMethodResult& oResult) {
        // RAWLOGC_INFO("getIsAvailable","BluetoothConnection");

    }
    // deviceName getter for "deviceName" property
    virtual void getDeviceName(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set(BluetoothThread::getInstance()->getDeviceName().toStdString().c_str());
    }
    // deviceName= setter for "deviceName" property
    virtual void setDeviceName( const rho::String& deviceName, rho::apiGenerator::CMethodResult& oResult) {
        BluetoothThread::getInstance()->setDeviceName(QString::fromStdString(deviceName));
    }
    // lastError getter for "lastError" property
    virtual void getLastError(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set(BluetoothThread::getInstance()->getLastError().toStdString().c_str());
    }
    // enableBluetooth enable BT HW
    virtual void enableBluetooth(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothThread::getInstance()->setWorking(true);

    }
    // disableBluetooth disable BT HW
    virtual void disableBluetooth(rho::apiGenerator::CMethodResult& oResult) {
        BluetoothThread::getInstance()->setWorking(false);
    }

    virtual void getConnectionByID( const rho::String& connectionID, rho::apiGenerator::CMethodResult& oResult) {
        // RAWLOGC_INFO("getConnectionByID","BluetoothConnection");
        oResult.set(connectionID);
    }

    virtual void createConnection(const rho::String& role, rho::apiGenerator::CMethodResult& oResult) {
        qDebug() << "Trying to create connection with role: " + QString::fromStdString(role);
        if(role == RHO_BT_ROLE_SERVER){
            BluetoothThread::getInstance()->createServer("", oResult);
        }else if(role == RHO_BT_ROLE_CLIENT){
            BluetoothThread::getInstance()->openDeviceDiscover(oResult);
        }else{
            qDebug() << "Strange role";
        }
    }

    virtual void createServerConnection(rho::apiGenerator::CMethodResult& oResult) {
        qDebug() << "Creating server connection";
        BluetoothThread::getInstance()->createServer("", oResult);
    }

    virtual void createClientConnection(const rho::String& serverName, rho::apiGenerator::CMethodResult& oResult) {
        qDebug() << "Creating client connection " + QString::fromStdString(serverName);
        BluetoothThread::getInstance()->createClient(QString::fromStdString(serverName), oResult);
    }

    virtual void stopCurrentConnectionProcess(rho::apiGenerator::CMethodResult& oResult) {
        // RAWLOGC_INFO("stopCurrentConnectionProcess","BluetoothConnection");

    }

};

class CBluetoothConnectionFactory: public CBluetoothConnectionFactoryBase
{
    ~CBluetoothConnectionFactory(){}
    virtual IBluetoothConnectionSingleton* createModuleSingleton()
    {
        return new CBluetoothConnectionSingleton();
    }
    virtual IBluetoothConnection* createModuleByID(const rho::String& strID)
    {
        return new CBluetoothConnectionImpl(strID);
    }
};

extern "C" void Init_Bluetooth_extension()
{
    BluetoothThread::getInstance();
    CBluetoothConnectionFactory::setInstance( new CBluetoothConnectionFactory() );
    Init_BluetoothConnection_API();
}

}
