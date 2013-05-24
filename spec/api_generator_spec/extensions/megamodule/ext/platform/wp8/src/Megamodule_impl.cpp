#include "../../../shared/generated/cpp/MegaModuleBase.h"

namespace rho {

using namespace apiGenerator;

class CMegaModuleImpl: public CMegaModuleBase
{
public:
    CMegaModuleImpl(const rho::StringW& strID): CMegaModuleBase()
    {
        /*m_hashProps.put( L"display", L"LCD");
        m_hashProps.put( L"sound", L"Dolby");*/
    }

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}

};

class CMegaModuleSingleton: public CMegaModuleSingletonBase
{
    ~CMegaModuleSingleton(){}
    virtual rho::StringW getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CMegaModuleFactory: public CMegaModuleFactoryBase
{
    ~CMegaModuleFactory(){}
    virtual IMegaModuleSingleton* createModuleSingleton();
    virtual IMegaModule* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_MegaModule_extension()
{
    CMegaModuleFactory::setInstance( new CMegaModuleFactory() );
    Init_MegaModule_API();
}

IMegaModule* CMegaModuleFactory::createModuleByID(const rho::StringW& strID)
{
    return new CMegaModuleImpl(strID);
}

IMegaModuleSingleton* CMegaModuleFactory::createModuleSingleton()
{
    return new CMegaModuleSingleton();
}

void CMegaModuleSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::StringW> arIDs;
    arIDs.addElement(L"SC1");
    arIDs.addElement(L"SC2");

    oResult.set(arIDs);
}

rho::StringW CMegaModuleSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}