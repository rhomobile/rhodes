#include "../../../shared/generated/cpp/ConnectionCheckingBase.h"

namespace rho {

using namespace apiGenerator;

class CConnectionCheckingImpl: public CConnectionCheckingBase
{
public:
    CConnectionCheckingImpl(const rho::String& strID): CConnectionCheckingBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("Win32");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }

};

class CConnectionCheckingSingleton: public CConnectionCheckingSingletonBase
{
    ~CConnectionCheckingSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CConnectionCheckingFactory: public CConnectionCheckingFactoryBase
{
    ~CConnectionCheckingFactory(){}
    virtual IConnectionCheckingSingleton* createModuleSingleton();
    virtual IConnectionChecking* createModuleByID(const rho::String& strID);
};

extern "C" void Init_ConnectionChecking_extension()
{
    CConnectionCheckingFactory::setInstance( new CConnectionCheckingFactory() );
    Init_ConnectionChecking_API();
}

IConnectionChecking* CConnectionCheckingFactory::createModuleByID(const rho::String& strID)
{
    return new CConnectionCheckingImpl(strID);
}

IConnectionCheckingSingleton* CConnectionCheckingFactory::createModuleSingleton()
{
    return new CConnectionCheckingSingleton();
}

void CConnectionCheckingSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CConnectionCheckingSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}