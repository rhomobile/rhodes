#include "ISystem.h"


namespace rho {

using namespace ::rho::apiGenerator;
class CSystemBase: public ISystem
{
protected:

    ::rho::Hashtable<::rho::StringW, ::rho::StringW> m_hashProps;

public:
    CSystemBase(){}


    virtual void getProperty( const ::rho::StringW& propertyName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[propertyName]);
    }
    virtual void getProperties( const ::rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult)
    {
        ::rho::Hashtable<::rho::StringW, ::rho::StringW> res;
        oResult.setCollectionMode(true);
        for ( int i = 0; i < (int)arrayofNames.size(); i++ )
        {
            getProperty(arrayofNames[i], oResult);

            if ( oResult.isError() )
                break;

            res[arrayofNames[i]] = oResult.toString();
        }

        oResult.setCollectionMode(false);
        if ( oResult.isError() )
            oResult.callCallback();
        else
            oResult.set(res);
    }
    virtual void getAllProperties(CMethodResult& oResult)
    {
        oResult.set(m_hashProps);
    }
    virtual void setProperty( const ::rho::StringW& propertyName,  const ::rho::StringW& propertyValue, CMethodResult& /*oResult*/)
    {
        m_hashProps.put(propertyName, propertyValue);
    }
    virtual void setProperties( const ::rho::Hashtable<::rho::StringW, ::rho::StringW>& propertyMap, CMethodResult& oResult)
    {
        for ( ::rho::Hashtable<::rho::StringW, ::rho::StringW>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
        {
            setProperty( it->first, it->second, oResult );
            if ( oResult.isError() )
                break;
        }
    }
    virtual void clearAllProperties(CMethodResult& oResult)
    {
        m_hashProps.clear();
    }

};

class CSystemFactoryBase : public ::rho::apiGenerator::CModuleFactoryBase<ISystem, ISystemSingleton, ISystemFactory>
{
protected:
    static ::rho::common::CAutoPtr<CSystemFactoryBase> m_pInstance;

public:

    static void setInstance(CSystemFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSystemFactoryBase* getInstance(){ return m_pInstance; }

    static ISystemSingleton* getSystemSingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_System_API();


}
