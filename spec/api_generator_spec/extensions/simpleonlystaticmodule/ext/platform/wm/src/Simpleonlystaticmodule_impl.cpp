#include "../../../shared/generated/cpp/SimpleOnlyStaticModuleBase.h"

namespace rho {

using namespace apiGenerator;
using namespace examples;

class CSimpleOnlyStaticModuleImpl: public CSimpleOnlyStaticModuleBase
{
public:
    CSimpleOnlyStaticModuleImpl(): CSimpleOnlyStaticModuleBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}

};

extern "C" void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide);

class CSimpleOnlyStaticModuleSingleton: public CSimpleOnlyStaticModuleSingletonBase
{
    ~CSimpleOnlyStaticModuleSingleton(){}
//    virtual rho::StringW getInitialDefaultID();
//    virtual void enumerate(CMethodResult& oResult);

    virtual void calcSumm( int x,  int y, rho::apiGenerator::CMethodResult& oResult){ oResult.set(x+y); }
    virtual void joinStrings( const rho::Vector<rho::String>& strings, rho::apiGenerator::CMethodResult& oResult)
    { 
        String strRes;
        for(int i = 0; i < strings.size(); i++)
            strRes += strings[i];

        oResult.set(strRes); 
    }
    virtual void getPlatform(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set("Windows");
    }

    virtual void showAlertFromUIThread(rho::apiGenerator::CMethodResult& oResult)
    {
        alert_show_status("Test1", "Test2 message", "Hide");
    }

};

class CSimpleOnlyStaticModuleFactory: public CSimpleOnlyStaticModuleFactoryBase
{
    ~CSimpleOnlyStaticModuleFactory(){}
    virtual ISimpleOnlyStaticModuleSingleton* createModuleSingleton();
    //virtual ISimpleOnlyStaticModule* createModuleByID(const rho::StringW& strID);

};

extern "C" void Init_SimpleOnlyStaticModule()
{
    CSimpleOnlyStaticModuleFactory::setInstance( new CSimpleOnlyStaticModuleFactory() );
    Init_SimpleOnlyStaticModule_API();
}

//ISimpleOnlyStaticModule* CSimpleOnlyStaticModuleFactory::createModuleByID(const rho::StringW& strID)
//{
//    return new CSimpleOnlyStaticModuleImpl(strID);
//}

ISimpleOnlyStaticModuleSingleton* CSimpleOnlyStaticModuleFactory::createModuleSingleton()
{
    return new CSimpleOnlyStaticModuleSingleton();
}
/*
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
}*/

}