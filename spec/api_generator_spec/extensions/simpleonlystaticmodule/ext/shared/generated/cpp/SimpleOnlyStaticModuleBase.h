#include "ISimpleOnlyStaticModule.h"
#include "common/StringConverter.h"


namespace rho {
namespace examples {

using namespace rho::apiGenerator;

class CSimpleOnlyStaticModuleFactoryBase : public CModuleFactoryBase<ISimpleOnlyStaticModule, ISimpleOnlyStaticModuleSingleton, ISimpleOnlyStaticModuleFactory>
{
protected:
    static rho::common::CAutoPtr<CSimpleOnlyStaticModuleFactoryBase> m_pInstance;
    Hashtable<rho::String,ISimpleOnlyStaticModule*> m_hashModules;

public:

    static void setInstance(CSimpleOnlyStaticModuleFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSimpleOnlyStaticModuleFactoryBase* getInstance(){ return m_pInstance; }

    static ISimpleOnlyStaticModuleSingleton* getSimpleOnlyStaticModuleSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual ISimpleOnlyStaticModule* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            ISimpleOnlyStaticModule* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual ISimpleOnlyStaticModule* createModuleByID(const rho::String& strID){ return (ISimpleOnlyStaticModule*)0; };

};

class CSimpleOnlyStaticModuleSingletonBase : public CModuleSingletonBase< ISimpleOnlyStaticModuleSingleton >
{
protected:




public:




};

class CSimpleOnlyStaticModuleBase: public ISimpleOnlyStaticModule
{
protected:

public:

    CSimpleOnlyStaticModuleBase();


 

};

extern "C" void Init_SimpleOnlyStaticModule_API();


}
}
