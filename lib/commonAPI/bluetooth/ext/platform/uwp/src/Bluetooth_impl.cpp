#include "../../../shared/generated/cpp/BluetoothBase.h"

namespace rho {

using namespace apiGenerator;

class CBluetoothImpl: public CBluetoothBase
{
public:
    CBluetoothImpl(const rho::String& strID): CBluetoothBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("UWP");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
};

class CBluetoothSingleton: public CBluetoothSingletonBase
{
    ~CBluetoothSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBluetoothFactory: public CBluetoothFactoryBase
{
    ~CBluetoothFactory(){}
    virtual IBluetoothSingleton* createModuleSingleton();
    virtual IBluetooth* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Bluetooth_extension()
{
    CBluetoothFactory::setInstance( new CBluetoothFactory() );
    Init_Bluetooth_API();
}

IBluetooth* CBluetoothFactory::createModuleByID(const rho::String& strID)
{
    return new CBluetoothImpl(strID);
}

IBluetoothSingleton* CBluetoothFactory::createModuleSingleton()
{
    return new CBluetoothSingleton();
}

void CBluetoothSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CBluetoothSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}