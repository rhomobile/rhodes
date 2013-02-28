#include "INetworkAccess.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"


namespace rho {

using namespace rho::apiGenerator;

class CNetworkAccessFactoryBase : public CModuleFactoryBase<INetworkAccess, INetworkAccessSingleton, INetworkAccessFactory>
{
protected:
    static rho::common::CAutoPtr<CNetworkAccessFactoryBase> m_pInstance;
    HashtablePtr<rho::String,INetworkAccess*> m_hashModules;

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
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class CNetworkAccessSingletonBase : public CModuleSingletonBase< INetworkAccessSingleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;





public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    CNetworkAccessSingletonBase();
    ~CNetworkAccessSingletonBase();






};

class CNetworkAccessBase: public INetworkAccess
{
protected:
    DEFINE_LOGCLASS;


public:



 

};

extern "C" void Init_NetworkAccess_API();


}
