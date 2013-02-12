#include "ISystem.h"
#include "common/StringConverter.h"


namespace rho {

using namespace rho::apiGenerator;

class CSystemFactoryBase : public CModuleFactoryBase<ISystem, ISystemSingleton, ISystemFactory>
{
protected:
    static rho::common::CAutoPtr<CSystemFactoryBase> m_pInstance;
    Hashtable<rho::StringW,ISystem*> m_hashModules;

public:

    static void setInstance(CSystemFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSystemFactoryBase* getInstance(){ return m_pInstance; }

    static ISystemSingleton* getSystemSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual ISystem* getModuleByID(const rho::StringW& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            ISystem* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual ISystem* createModuleByID(const rho::StringW& strID){ return (ISystem*)0; };

};

class CSystemSingletonBase : public CModuleSingletonBase< ISystemSingleton >
{
protected:

    rho::StringW m_strDefaultID;

public:


    virtual void setDefaultID(const rho::StringW& strDefaultID){ m_strDefaultID = strDefaultID; }
    virtual rho::StringW getDefaultID()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getInitialDefaultID());
        return m_strDefaultID; 
    }

};

class CSystemBase: public ISystem
{
protected:

    rho::Hashtable<rho::StringW, rho::StringW> m_hashProps;
    rho::Hashtable<rho::StringW, rho::apiGenerator::CMethodAccessor< ISystem > *> m_mapPropAccessors;

public:
    CSystemBase();


    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);


    static CSystemBase* getInstance(){ return static_cast< CSystemBase* >(CSystemFactoryBase::getInstance()->getModuleByID(CSystemFactoryBase::getSystemSingletonS()->getDefaultID())); }
 

};

extern "C" void Init_System_API();


}
