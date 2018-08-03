#include "../../../shared/generated/cpp/BluetoothConnectionBase.h"

namespace rho {

using namespace apiGenerator;

class CBluetoothConnectionImpl: public CBluetoothConnectionBase
{
public:
    CBluetoothConnectionImpl(const rho::String& strID): CBluetoothConnectionBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("WM");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }

};

class CBluetoothConnectionSingleton: public CBluetoothConnectionSingletonBase
{
    ~CBluetoothConnectionSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBluetoothConnectionFactory: public CBluetoothConnectionFactoryBase
{
    ~CBluetoothConnectionFactory(){}
    virtual IBluetoothConnectionSingleton* createModuleSingleton();
    virtual IBluetoothConnection* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Bluetooth_extension()
{
    CBluetoothConnectionFactory::setInstance( new CBluetoothConnectionFactory() );
    Init_BluetoothConnection_API();
}

IBluetoothConnection* CBluetoothConnectionFactory::createModuleByID(const rho::String& strID)
{
    return new CBluetoothConnectionImpl(strID);
}

IBluetoothConnectionSingleton* CBluetoothConnectionFactory::createModuleSingleton()
{
    return new CBluetoothConnectionSingleton();
}

void CBluetoothConnectionSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CBluetoothConnectionSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}