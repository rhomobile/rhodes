#include "../../../shared/generated/cpp/DevelopmentBase.h"

namespace rho {

using namespace apiGenerator;

class CDevelopmentImpl: public CDevelopmentBase
{
public:
    CDevelopmentImpl(const rho::String& strID): CDevelopmentBase()
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

class CDevelopmentSingleton: public CDevelopmentSingletonBase
{
    ~CDevelopmentSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CDevelopmentFactory: public CDevelopmentFactoryBase
{
    ~CDevelopmentFactory(){}
    virtual IDevelopmentSingleton* createModuleSingleton();
    virtual IDevelopment* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Development_extension()
{
    CDevelopmentFactory::setInstance( new CDevelopmentFactory() );
    Init_Development_API();
}

IDevelopment* CDevelopmentFactory::createModuleByID(const rho::String& strID)
{
    return new CDevelopmentImpl(strID);
}

IDevelopmentSingleton* CDevelopmentFactory::createModuleSingleton()
{
    return new CDevelopmentSingleton();
}

void CDevelopmentSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CDevelopmentSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}