#include "ISystem.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"


namespace rho {

using namespace rho::apiGenerator;

class CSystemFactoryBase : public CModuleFactoryBase<ISystem, ISystemSingleton, ISystemFactory>
{
protected:
    static rho::common::CAutoPtr<CSystemFactoryBase> m_pInstance;
    HashtablePtr<rho::String,ISystem*> m_hashModules;

public:

    static void setInstance(CSystemFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSystemFactoryBase* getInstance(){ return m_pInstance; }

    static ISystemSingleton* getSystemSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual ISystem* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            ISystem* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual ISystem* createModuleByID(const rho::String& strID){ return (ISystem*)0; };
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class CSystemSingletonBase : public CModuleSingletonBase< ISystemSingleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;




    rho::Hashtable<rho::String, rho::String> m_hashProps;
    rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< ISystemSingleton > *> m_mapPropAccessors;


public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    CSystemSingletonBase();
    ~CSystemSingletonBase();




    virtual void getProperty( const rho::String& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);



};

class CSystemBase: public ISystem
{
protected:
    DEFINE_LOGCLASS;


public:



 

};

extern "C" void Init_System_API();


}
