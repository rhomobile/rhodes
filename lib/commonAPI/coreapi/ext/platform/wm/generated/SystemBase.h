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


    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[propertyName]);
    }
    virtual void getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult)
    {
        rho::Hashtable<::rho::StringW, rho::StringW> res;
        oResult.setCollectionMode(true);
        for ( int i = 0; i < (int)arrayofNames.size(); i++ )
        {
            getProperty(arrayofNames[i], oResult);

            if ( oResult.isError() )
                break;

            res[arrayofNames[i]] = oResult.toStringW();
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
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult)
    {

        if (false){}


        else if (_wcsicmp(L"applicationIconBadge",propertyName.c_str()) == 0){

            int64 arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setApplicationIconBadge( arg, oResult);

        }

        else if (_wcsicmp(L"httpProxyURI",propertyName.c_str()) == 0){

            rho::StringW arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setHttpProxyURI( arg, oResult);

        }

        else if (_wcsicmp(L"lockWindowSize",propertyName.c_str()) == 0){

            bool arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setLockWindowSize( arg, oResult);

        }

        else if (_wcsicmp(L"showKeyboard",propertyName.c_str()) == 0){

            bool arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setShowKeyboard( arg, oResult);

        }

        else if (_wcsicmp(L"fullScreen",propertyName.c_str()) == 0){

            bool arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setFullScreen( arg, oResult);

        }

        else if (_wcsicmp(L"localServerPort",propertyName.c_str()) == 0){

            int64 arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setLocalServerPort( arg, oResult);

        }

        else if (_wcsicmp(L"screenAutoRotate",propertyName.c_str()) == 0){

            bool arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            setScreenAutoRotate( arg, oResult);

        }
    }
    virtual void setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult)
    {
        for ( rho::Hashtable<rho::StringW, rho::StringW>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
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
