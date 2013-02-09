#include "ISystem.h"
#include "common/StringConverter.h"


namespace rho {

using namespace rho::apiGenerator;
class CSystemBase: public ISystem
{
protected:

    rho::Hashtable<::rho::StringW, ::rho::StringW> m_hashProps;

public:
    CSystemBase(){}


    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);



};

class CSystemFactoryBase : public CModuleFactoryBase<ISystem, ISystemSingleton, ISystemFactory>
{
protected:
    static rho::common::CAutoPtr<CSystemFactoryBase> m_pInstance;

public:

    static void setInstance(CSystemFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSystemFactoryBase* getInstance(){ return m_pInstance; }

    static ISystemSingleton* getSystemSingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_System_API();


}
