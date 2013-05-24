#pragma once

#include "IMegamodule.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"


namespace rho {
namespace examples {

using namespace rho::apiGenerator;

// hash keys used in properties and parameters 
namespace megamodule {

    static const char HK_ITEM_ARRAY[] = "itemArray"; 

    static const char HK_ITEM_STR[] = "itemStr"; 

    static const char HK_PARAMA2[] = "parama2"; 

    static const char HK_PARAME1[] = "parame1"; 

    static const char HK_STR_PARAM[] = "strParam"; 
 
} 

class CMegamoduleFactoryBase : public CModuleFactoryBase<IMegamodule, IMegamoduleSingleton, IMegamoduleFactory>
{
protected:
    static rho::common::CAutoPtr<CMegamoduleFactoryBase> m_pInstance;
    HashtablePtr<rho::String,IMegamodule*> m_hashModules;

public:

    static void setInstance(CMegamoduleFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CMegamoduleFactoryBase* getInstance(){ return m_pInstance; }

    static IMegamoduleSingleton* getMegamoduleSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual IMegamodule* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            IMegamodule* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual IMegamodule* createModuleByID(const rho::String& strID){ return (IMegamodule*)0; };
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class CMegamoduleSingletonBase : public CModuleSingletonBase< IMegamoduleSingleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;


    rho::String m_strDefaultID;




public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    CMegamoduleSingletonBase();
    ~CMegamoduleSingletonBase();




    virtual void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }
    virtual rho::String getDefaultID()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getInitialDefaultID());
        return m_strDefaultID; 
    }

};

class CMegamoduleBase: public IMegamodule
{
protected:
    DEFINE_LOGCLASS;



    rho::Hashtable<rho::String, rho::String> m_hashProps;

    rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< IMegamodule > *> m_mapPropAccessors;

public:


    CMegamoduleBase();

    virtual void getProperty( const rho::String& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);


    virtual void getStringProperty(rho::apiGenerator::CMethodResult& oResult);

    virtual void getIntegerProperty(rho::apiGenerator::CMethodResult& oResult);

    virtual void setIntegerProperty( int IntegerProperty, rho::apiGenerator::CMethodResult& oResult);


    static CMegamoduleBase* getInstance(){ return static_cast< CMegamoduleBase* >(CMegamoduleFactoryBase::getInstance()->getModuleByID(CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID())); }
 

};

extern "C" void Init_Megamodule_API();


}
}
