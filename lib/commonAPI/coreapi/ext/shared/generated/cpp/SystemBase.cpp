#include "SystemBase.h"
#include "common/RhodesApp.h"


namespace rho {

IMPLEMENT_LOGCLASS(CSystemSingletonBase, "System");
IMPLEMENT_LOGCLASS(CSystemBase, "System");

rho::common::CAutoPtr< CSystemFactoryBase> CSystemFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 
const char ISystemSingleton::PLATFORM_WM_CE[] = "WINDOWS"; 
const char ISystemSingleton::PLATFORM_ANDROID[] = "ANDROID"; 
const char ISystemSingleton::PLATFORM_IOS[] = "APPLE"; 
const char ISystemSingleton::PLATFORM_WP8[] = "WP8"; 
const char ISystemSingleton::PLATFORM_WINDOWS_DESKTOP[] = "WINDOWS_DESKTOP"; 
const char ISystemSingleton::SCREEN_PORTRAIT[] = "portrait"; 
const char ISystemSingleton::SCREEN_LANDSCAPE[] = "landscape"; 

////////////////////////////////////////////////

CSystemSingletonBase::CSystemSingletonBase()
{

    RHODESAPP().getExtManager().registerExtension( "System", this ); 
    m_mapPropAccessors["platform"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPlatform ); 
    m_mapPropAccessors["hasCamera"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasCamera ); 
    m_mapPropAccessors["has_camera"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasCamera ); 
    m_mapPropAccessors["screenWidth"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenWidth ); 
    m_mapPropAccessors["screen_width"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenWidth ); 
    m_mapPropAccessors["screenHeight"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenHeight ); 
    m_mapPropAccessors["screen_height"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenHeight ); 
    m_mapPropAccessors["realScreenWidth"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getRealScreenWidth ); 
    m_mapPropAccessors["real_screen_width"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getRealScreenWidth ); 
    m_mapPropAccessors["realScreenHeight"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getRealScreenHeight ); 
    m_mapPropAccessors["real_screen_height"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getRealScreenHeight ); 
    m_mapPropAccessors["screenOrientation"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenOrientation ); 
    m_mapPropAccessors["screen_orientation"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenOrientation ); 
    m_mapPropAccessors["ppiX"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPpiX ); 
    m_mapPropAccessors["ppi_x"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPpiX ); 
    m_mapPropAccessors["ppiY"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPpiY ); 
    m_mapPropAccessors["ppi_y"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPpiY ); 
    m_mapPropAccessors["phoneNumber"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPhoneNumber ); 
    m_mapPropAccessors["phone_number"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPhoneNumber ); 
    m_mapPropAccessors["deviceOwnerEmail"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDeviceOwnerEmail ); 
    m_mapPropAccessors["device_owner_email"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDeviceOwnerEmail ); 
    m_mapPropAccessors["deviceOwnerName"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDeviceOwnerName ); 
    m_mapPropAccessors["device_owner_name"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDeviceOwnerName ); 
    m_mapPropAccessors["devicePushId"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDevicePushId ); 
    m_mapPropAccessors["deviceId"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDevicePushId ); 
    m_mapPropAccessors["device_push_id"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDevicePushId ); 
    m_mapPropAccessors["phoneId"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPhoneId ); 
    m_mapPropAccessors["phone_id"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getPhoneId ); 
    m_mapPropAccessors["deviceName"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDeviceName ); 
    m_mapPropAccessors["device_name"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getDeviceName ); 
    m_mapPropAccessors["osVersion"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getOsVersion ); 
    m_mapPropAccessors["os_version"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getOsVersion ); 
    m_mapPropAccessors["locale"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getLocale ); 
    m_mapPropAccessors["country"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getCountry ); 
    m_mapPropAccessors["isEmulator"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getIsEmulator ); 
    m_mapPropAccessors["is_emulator"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getIsEmulator ); 
    m_mapPropAccessors["hasCalendar"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasCalendar ); 
    m_mapPropAccessors["has_calendar"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasCalendar ); 
    m_mapPropAccessors["isMotorolaDevice"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getIsMotorolaDevice ); 
    m_mapPropAccessors["is_motorola_device"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getIsMotorolaDevice ); 
    m_mapPropAccessors["oemInfo"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getOemInfo ); 
    m_mapPropAccessors["oem_info"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getOemInfo ); 
    m_mapPropAccessors["uuid"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getUuid ); 
    m_mapPropAccessors["applicationIconBadge"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getApplicationIconBadge ); 
    m_mapPropAccessors["application_icon_badge"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getApplicationIconBadge ); 
    m_mapPropAccessors["httpProxyURI"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHttpProxyURI ); 
    m_mapPropAccessors["http_proxy_url"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHttpProxyURI ); 
    m_mapPropAccessors["http_proxy_uri"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHttpProxyURI ); 
    m_mapPropAccessors["lockWindowSize"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getLockWindowSize ); 
    m_mapPropAccessors["lock_window_size"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getLockWindowSize ); 
    m_mapPropAccessors["showKeyboard"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getShowKeyboard ); 
    m_mapPropAccessors["show_keyboard"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getShowKeyboard ); 
    m_mapPropAccessors["fullScreen"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getFullScreen ); 
    m_mapPropAccessors["full_screen"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getFullScreen ); 
    m_mapPropAccessors["localServerPort"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getLocalServerPort ); 
    m_mapPropAccessors["local_server_port"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getLocalServerPort ); 
    m_mapPropAccessors["freeServerPort"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getFreeServerPort ); 
    m_mapPropAccessors["free_server_port"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getFreeServerPort ); 
    m_mapPropAccessors["screenAutoRotate"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenAutoRotate ); 
    m_mapPropAccessors["screen_auto_rotate"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenAutoRotate ); 
    m_mapPropAccessors["hasTouchscreen"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasTouchscreen ); 
    m_mapPropAccessors["has_touchscreen"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasTouchscreen ); 
    m_mapPropAccessors["securityTokenNotPassed"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getSecurityTokenNotPassed ); 
    m_mapPropAccessors["security_token_not_passed"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getSecurityTokenNotPassed ); 
    m_mapPropAccessors["webviewFramework"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getWebviewFramework ); 
    m_mapPropAccessors["webview_framework"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getWebviewFramework ); 
    m_mapPropAccessors["screenSleeping"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenSleeping ); 
    m_mapPropAccessors["screen_sleeping"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getScreenSleeping ); 
    m_mapPropAccessors["hasNetwork"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasNetwork ); 
    m_mapPropAccessors["has_network"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasNetwork ); 
    m_mapPropAccessors["hasWifiNetwork"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasWifiNetwork ); 
    m_mapPropAccessors["has_wifi_network"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasWifiNetwork ); 
    m_mapPropAccessors["hasCellNetwork"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasCellNetwork ); 
    m_mapPropAccessors["has_cell_network"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasCellNetwork ); 
    m_mapPropAccessors["hasSqlite"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasSqlite ); 
    m_mapPropAccessors["has_sqlite"] = new rho::apiGenerator::CMethodAccessor< ISystemSingleton >( &ISystemSingleton::getHasSqlite ); 


    m_mapPropAccessors["applicationIconBadge"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< int, int >(&ISystemSingleton::setApplicationIconBadge) );
    m_mapPropAccessors["httpProxyURI"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< const rho::String&, rho::String >(&ISystemSingleton::setHttpProxyURI) );
    m_mapPropAccessors["lockWindowSize"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< bool, bool >(&ISystemSingleton::setLockWindowSize) );
    m_mapPropAccessors["showKeyboard"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< bool, bool >(&ISystemSingleton::setShowKeyboard) );
    m_mapPropAccessors["fullScreen"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< bool, bool >(&ISystemSingleton::setFullScreen) );
    m_mapPropAccessors["localServerPort"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< int, int >(&ISystemSingleton::setLocalServerPort) );
    m_mapPropAccessors["screenAutoRotate"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< bool, bool >(&ISystemSingleton::setScreenAutoRotate) );
    m_mapPropAccessors["screenSleeping"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystemSingleton>::CSetter< bool, bool >(&ISystemSingleton::setScreenSleeping) );
}

void CSystemSingletonBase::getProperty( const rho::String& propertyName, CMethodResult& oResult)
{
    CMethodAccessor< ISystemSingleton >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
        
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError("Get unknown property: " + propertyName);
        
    }
}

void CSystemSingletonBase::getProperties( const rho::Vector<::rho::String>& arrayofNames, CMethodResult& oResult)
{
    rho::Hashtable<::rho::String, rho::String> res;
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

void CSystemSingletonBase::getAllProperties(CMethodResult& oResult)
{
    oResult.set(m_hashProps);
}

void CSystemSingletonBase::setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
{
    CMethodAccessor< ISystemSingleton >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError("Set unknown property: " + propertyName);
        
    }
}

void CSystemSingletonBase::setProperties( const rho::Hashtable<::rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
    {
        setProperty( it->first, it->second, oResult );
        if ( oResult.isError() )
            break;
    }
}

void CSystemSingletonBase::clearAllProperties(CMethodResult& oResult)
{
    m_hashProps.clear();
}



CSystemSingletonBase::~CSystemSingletonBase()
{
    CSystemFactoryBase::setInstance(0);
}


}
