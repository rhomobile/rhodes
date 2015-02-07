#include "../../../shared/generated/cpp/DevelopmentBase.h"

namespace rho {

using namespace apiGenerator;

class CDevelopmentImpl: public CDevelopmentBase
{
public:
    CDevelopmentImpl(const rho::String& strID): CDevelopmentBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

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