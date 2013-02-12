#include "SystemBase.h"


namespace rho {

rho::common::CAutoPtr< CSystemFactoryBase> CSystemFactoryBase::m_pInstance;

///////////////////////////////////////
//string constants definiton 
const wchar_t ISystem::PLATFORM_WM_CE[] = L"WINDOWS"; 
const wchar_t ISystem::PLATFORM_ANDROID[] = L"ANDROID"; 
const wchar_t ISystem::PLATFORM_IOS[] = L"APPLE"; 
const wchar_t ISystem::PLATFORM_WP8[] = L"WP8"; 
const wchar_t ISystem::PLATFORM_WINDOWS_DESKTOP[] = L"WINDOWS_DESKTOP"; 
const wchar_t ISystem::SCREEN_PORTRAIT[] = L"portrait"; 
const wchar_t ISystem::SCREEN_LANDSCAPE[] = L"landscape"; 

////////////////////////////////////////////////


CSystemBase::CSystemBase()
{

    m_mapPropAccessors[L"platform"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPlatform ); 
    m_mapPropAccessors[L"hasCamera"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCamera ); 
    m_mapPropAccessors[L"screenWidth"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenWidth ); 
    m_mapPropAccessors[L"screenHeight"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenHeight ); 
    m_mapPropAccessors[L"realScreenWidth"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getRealScreenWidth ); 
    m_mapPropAccessors[L"realScreenHeight"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getRealScreenHeight ); 
    m_mapPropAccessors[L"screenOrientation"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenOrientation ); 
    m_mapPropAccessors[L"ppiX"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPpiX ); 
    m_mapPropAccessors[L"ppiY"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPpiY ); 
    m_mapPropAccessors[L"phoneNumber"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPhoneNumber ); 
    m_mapPropAccessors[L"deviceOwnerEmail"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceOwnerEmail ); 
    m_mapPropAccessors[L"deviceOwnerName"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceOwnerName ); 
    m_mapPropAccessors[L"devicePushId"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDevicePushId ); 
    m_mapPropAccessors[L"phoneId"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPhoneId ); 
    m_mapPropAccessors[L"deviceName"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceName ); 
    m_mapPropAccessors[L"osVersion"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getOsVersion ); 
    m_mapPropAccessors[L"locale"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLocale ); 
    m_mapPropAccessors[L"country"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getCountry ); 
    m_mapPropAccessors[L"isEmulator"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getIsEmulator ); 
    m_mapPropAccessors[L"hasCalendar"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCalendar ); 
    m_mapPropAccessors[L"isMotorolaDevice"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getIsMotorolaDevice ); 
    m_mapPropAccessors[L"oemInfo"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getOemInfo ); 
    m_mapPropAccessors[L"uuid"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getUuid ); 
    m_mapPropAccessors[L"applicationIconBadge"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getApplicationIconBadge ); 
    m_mapPropAccessors[L"httpProxyURI"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHttpProxyURI ); 
    m_mapPropAccessors[L"lockWindowSize"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLockWindowSize ); 
    m_mapPropAccessors[L"showKeyboard"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getShowKeyboard ); 
    m_mapPropAccessors[L"fullScreen"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getFullScreen ); 
    m_mapPropAccessors[L"localServerPort"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLocalServerPort ); 
    m_mapPropAccessors[L"freeServerPort"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getFreeServerPort ); 
    m_mapPropAccessors[L"screenAutoRotate"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenAutoRotate ); 
    m_mapPropAccessors[L"hasTouchscreen"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasTouchscreen ); 
    m_mapPropAccessors[L"securityTokenNotPassed"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getSecurityTokenNotPassed ); 


    m_mapPropAccessors[L"applicationIconBadge"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< int64, int64 >(&ISystem::setApplicationIconBadge) );
    m_mapPropAccessors[L"httpProxyURI"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< const rho::StringW&, rho::StringW >(&ISystem::setHttpProxyURI) );
    m_mapPropAccessors[L"lockWindowSize"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setLockWindowSize) );
    m_mapPropAccessors[L"showKeyboard"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setShowKeyboard) );
    m_mapPropAccessors[L"fullScreen"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setFullScreen) );
    m_mapPropAccessors[L"localServerPort"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< int64, int64 >(&ISystem::setLocalServerPort) );
    m_mapPropAccessors[L"screenAutoRotate"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setScreenAutoRotate) );
}

void CSystemBase::getProperty( const rho::StringW& propertyName, CMethodResult& oResult)
{
    CMethodAccessor< ISystem >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
        
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError(L"Get unknown property: " + propertyName);
        
    }
}

void CSystemBase::getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult)
{
    rho::Hashtable<::rho::StringW, rho::StringW> res;
    oResult.setCollectionMode(true);
    for ( int i = 0; i < (int)arrayofNames.size(); i++ )
    {
        getProperty(arrayofNames[i], oResult);

        if ( oResult.isError() )
            break;

        res[arrayofNames[i]] = common::convertToStringW(oResult.toString());
    }

    oResult.setCollectionMode(false);
    if ( oResult.isError() )
        oResult.callCallback();
    else
        oResult.set(res);
}

void CSystemBase::getAllProperties(CMethodResult& oResult)
{
    oResult.set(m_hashProps);
}

void CSystemBase::setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult)
{
    CMethodAccessor< ISystem >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError(L"Set unknown property: " + propertyName);
        
    }
}

void CSystemBase::setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::StringW, rho::StringW>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
    {
        setProperty( it->first, it->second, oResult );
        if ( oResult.isError() )
            break;
    }
}

void CSystemBase::clearAllProperties(CMethodResult& oResult)
{
    m_hashProps.clear();
}


}
