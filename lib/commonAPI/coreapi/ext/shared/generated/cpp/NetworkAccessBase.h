#include "INetworkAccess.h"
#include "common/StringConverter.h"


namespace rho {

using namespace rho::apiGenerator;

class CNetworkAccessFactoryBase : public CModuleFactoryBase<INetworkAccess, INetworkAccessSingleton, INetworkAccessFactory>
{
protected:
    static rho::common::CAutoPtr<CNetworkAccessFactoryBase> m_pInstance;
    Hashtable<rho::String,INetworkAccess*> m_hashModules;

public:

    static void setInstance(CNetworkAccessFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CNetworkAccessFactoryBase* getInstance(){ return m_pInstance; }

    static INetworkAccessSingleton* getNetworkAccessSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual INetworkAccess* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            INetworkAccess* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual INetworkAccess* createModuleByID(const rho::String& strID){ return (INetworkAccess*)0; };

};

class CNetworkAccessSingletonBase : public CModuleSingletonBase< INetworkAccessSingleton >
{
protected:




public:


    CNetworkAccessSingletonBase();



};

class CNetworkAccessBase: public INetworkAccess
{
protected:

public:


 

};

extern "C" void Init_NetworkAccess_API();


}
