#include "ISQLite3.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"


namespace rho {
namespace database {

using namespace rho::apiGenerator;

class CSQLite3FactoryBase : public CModuleFactoryBase<ISQLite3, ISQLite3Singleton, ISQLite3Factory>
{
protected:
    static rho::common::CAutoPtr<CSQLite3FactoryBase> m_pInstance;
    HashtablePtr<rho::String,ISQLite3*> m_hashModules;

public:

    static void setInstance(CSQLite3FactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSQLite3FactoryBase* getInstance(){ return m_pInstance; }

    static ISQLite3Singleton* getSQLite3SingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual ISQLite3* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            ISQLite3* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual ISQLite3* createModuleByID(const rho::String& strID){ return (ISQLite3*)0; };
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class CSQLite3SingletonBase : public CModuleSingletonBase< ISQLite3Singleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;





public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    CSQLite3SingletonBase();
    ~CSQLite3SingletonBase();




};

class CSQLite3Base: public ISQLite3
{
protected:
    DEFINE_LOGCLASS;


public:


    CSQLite3Base();


 

};

extern "C" void Init_SQLite3_API();


}
}
