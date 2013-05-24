#include "../../../shared/generated/cpp/SimpleOnlyStaticModuleBase.h"

namespace rho {

using namespace apiGenerator;

class CSimpleOnlyStaticModuleImpl: public CSimpleOnlyStaticModuleBase
{
public:
    CSimpleOnlyStaticModuleImpl(const rho::StringW& strID): CSimpleOnlyStaticModuleBase()
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

class CSimpleOnlyStaticModuleSingleton: public CSimpleOnlyStaticModuleSingletonBase
{
    ~CSimpleOnlyStaticModuleSingleton(){}
    virtual rho::StringW getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CSimpleOnlyStaticModuleFactory: public CSimpleOnlyStaticModuleFactoryBase
{
    ~CSimpleOnlyStaticModuleFactory(){}
    virtual ISimpleOnlyStaticModuleSingleton* createModuleSingleton();
    virtual ISimpleOnlyStaticModule* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_SimpleOnlyStaticModule_extension()
{
    CSimpleOnlyStaticModuleFactory::setInstance( new CSimpleOnlyStaticModuleFactory() );
    Init_SimpleOnlyStaticModule_API();
}

ISimpleOnlyStaticModule* CSimpleOnlyStaticModuleFactory::createModuleByID(const rho::StringW& strID)
{
    return new CSimpleOnlyStaticModuleImpl(strID);
}

ISimpleOnlyStaticModuleSingleton* CSimpleOnlyStaticModuleFactory::createModuleSingleton()
{
    return new CSimpleOnlyStaticModuleSingleton();
}

void CSimpleOnlyStaticModuleSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::StringW> arIDs;
    arIDs.addElement(L"SC1");
    arIDs.addElement(L"SC2");

    oResult.set(arIDs);
}

rho::StringW CSimpleOnlyStaticModuleSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}