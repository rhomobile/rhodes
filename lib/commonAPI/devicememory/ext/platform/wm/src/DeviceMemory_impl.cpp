#include "../../../shared/generated/cpp/DeviceMemoryBase.h"

namespace rho {

using namespace apiGenerator;

class CDeviceMemoryImpl: public CDeviceMemoryBase
{
public:
    CDeviceMemoryImpl(const rho::String& strID): CDeviceMemoryBase()
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

class CDeviceMemorySingleton: public CDeviceMemorySingletonBase
{
    ~CDeviceMemorySingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CDeviceMemoryFactory: public CDeviceMemoryFactoryBase
{
    ~CDeviceMemoryFactory(){}
    virtual IDeviceMemorySingleton* createModuleSingleton();
    virtual IDeviceMemory* createModuleByID(const rho::String& strID);
};

extern "C" void Init_DeviceMemory_extension()
{
    CDeviceMemoryFactory::setInstance( new CDeviceMemoryFactory() );
    Init_DeviceMemory_API();
}

IDeviceMemory* CDeviceMemoryFactory::createModuleByID(const rho::String& strID)
{
    return new CDeviceMemoryImpl(strID);
}

IDeviceMemorySingleton* CDeviceMemoryFactory::createModuleSingleton()
{
    return new CDeviceMemorySingleton();
}

void CDeviceMemorySingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CDeviceMemorySingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}