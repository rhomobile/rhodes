#include "SystemBase.h"


namespace rho {

rho::common::CAutoPtr< CSystemFactoryBase> CSystemFactoryBase::m_pInstance;

///////////////////////////////////////
//string constants definiton 
const char ISystem::PLATFORM_WM_CE[] = "WINDOWS"; 
const char ISystem::PLATFORM_ANDROID[] = "ANDROID"; 
const char ISystem::PLATFORM_IOS[] = "APPLE"; 
const char ISystem::PLATFORM_WP8[] = "WP8"; 
const char ISystem::PLATFORM_WINDOWS_DESKTOP[] = "WINDOWS_DESKTOP"; 
const char ISystem::SCREEN_PORTRAIT[] = "portrait"; 
const char ISystem::SCREEN_LANDSCAPE[] = "landscape"; 

////////////////////////////////////////////////

//TODO: support property aliases
CSystemBase::CSystemBase()
{

    m_mapPropAccessors["platform"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPlatform ); 
    m_mapPropAccessors["hasCamera"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCamera ); 
    m_mapPropAccessors["has_camera"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCamera ); 
    m_mapPropAccessors["screenWidth"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenWidth ); 
    m_mapPropAccessors["screen_width"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenWidth ); 
    m_mapPropAccessors["screenHeight"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenHeight ); 
    m_mapPropAccessors["screen_height"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenHeight ); 
    m_mapPropAccessors["realScreenWidth"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getRealScreenWidth ); 
    m_mapPropAccessors["real_screen_width"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getRealScreenWidth ); 
    m_mapPropAccessors["realScreenHeight"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getRealScreenHeight ); 
    m_mapPropAccessors["real_screen_height"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getRealScreenHeight ); 
    m_mapPropAccessors["screenOrientation"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenOrientation ); 
    m_mapPropAccessors["screen_orientation"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenOrientation ); 
    m_mapPropAccessors["ppiX"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPpiX ); 
    m_mapPropAccessors["ppi_x"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPpiX ); 
    m_mapPropAccessors["ppiY"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPpiY ); 
    m_mapPropAccessors["ppi_y"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPpiY ); 
    m_mapPropAccessors["phoneNumber"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPhoneNumber ); 
    m_mapPropAccessors["phone_number"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPhoneNumber ); 
    m_mapPropAccessors["deviceOwnerEmail"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceOwnerEmail ); 
    m_mapPropAccessors["device_owner_email"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceOwnerEmail ); 
    m_mapPropAccessors["deviceOwnerName"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceOwnerName ); 
    m_mapPropAccessors["device_owner_name"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceOwnerName ); 
    m_mapPropAccessors["devicePushId"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDevicePushId ); 
    m_mapPropAccessors["deviceId"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDevicePushId ); 
    m_mapPropAccessors["device_push_id"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDevicePushId ); 
    m_mapPropAccessors["phoneId"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPhoneId ); 
    m_mapPropAccessors["phone_id"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getPhoneId ); 
    m_mapPropAccessors["deviceName"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceName ); 
    m_mapPropAccessors["device_name"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getDeviceName ); 
    m_mapPropAccessors["osVersion"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getOsVersion ); 
    m_mapPropAccessors["os_version"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getOsVersion ); 
    m_mapPropAccessors["locale"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLocale ); 
    m_mapPropAccessors["country"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getCountry ); 
    m_mapPropAccessors["isEmulator"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getIsEmulator ); 
    m_mapPropAccessors["is_emulator"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getIsEmulator ); 
    m_mapPropAccessors["hasCalendar"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCalendar ); 
    m_mapPropAccessors["has_calendar"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCalendar ); 
    m_mapPropAccessors["isMotorolaDevice"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getIsMotorolaDevice ); 
    m_mapPropAccessors["is_motorola_device"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getIsMotorolaDevice ); 
    m_mapPropAccessors["oemInfo"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getOemInfo ); 
    m_mapPropAccessors["oem_info"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getOemInfo ); 
    m_mapPropAccessors["uuid"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getUuid ); 
    m_mapPropAccessors["applicationIconBadge"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getApplicationIconBadge ); 
    m_mapPropAccessors["application_icon_badge"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getApplicationIconBadge ); 
    m_mapPropAccessors["httpProxyURI"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHttpProxyURI ); 
    m_mapPropAccessors["http_proxy_url"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHttpProxyURI ); 
    m_mapPropAccessors["http_proxy_uri"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHttpProxyURI ); 
    m_mapPropAccessors["lockWindowSize"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLockWindowSize ); 
    m_mapPropAccessors["lock_window_size"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLockWindowSize ); 
    m_mapPropAccessors["showKeyboard"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getShowKeyboard ); 
    m_mapPropAccessors["show_keyboard"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getShowKeyboard ); 
    m_mapPropAccessors["fullScreen"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getFullScreen ); 
    m_mapPropAccessors["full_screen"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getFullScreen ); 
    m_mapPropAccessors["localServerPort"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLocalServerPort ); 
    m_mapPropAccessors["local_server_port"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getLocalServerPort ); 
    m_mapPropAccessors["freeServerPort"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getFreeServerPort ); 
    m_mapPropAccessors["free_server_port"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getFreeServerPort ); 
    m_mapPropAccessors["screenAutoRotate"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenAutoRotate ); 
    m_mapPropAccessors["screen_auto_rotate"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenAutoRotate ); 
    m_mapPropAccessors["hasTouchscreen"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasTouchscreen ); 
    m_mapPropAccessors["has_touchscreen"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasTouchscreen ); 
    m_mapPropAccessors["securityTokenNotPassed"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getSecurityTokenNotPassed ); 
    m_mapPropAccessors["security_token_not_passed"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getSecurityTokenNotPassed ); 
    m_mapPropAccessors["webviewFramework"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getWebviewFramework ); 
    m_mapPropAccessors["webview_framework"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getWebviewFramework ); 
    m_mapPropAccessors["screenSleeping"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenSleeping ); 
    m_mapPropAccessors["screen_sleeping"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getScreenSleeping ); 
    m_mapPropAccessors["hasNetwork"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasNetwork ); 
    m_mapPropAccessors["has_network"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasNetwork ); 
    m_mapPropAccessors["hasWifiNetwork"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasWifiNetwork ); 
    m_mapPropAccessors["has_wifi_network"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasWifiNetwork ); 
    m_mapPropAccessors["hasCellNetwork"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCellNetwork ); 
    m_mapPropAccessors["has_cell_network"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasCellNetwork ); 
    m_mapPropAccessors["hasSqlite"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasSqlite ); 
    m_mapPropAccessors["has_sqlite"] = new rho::apiGenerator::CMethodAccessor< ISystem >( &ISystem::getHasSqlite ); 


    m_mapPropAccessors["applicationIconBadge"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< int64, int64 >(&ISystem::setApplicationIconBadge) );
    m_mapPropAccessors["httpProxyURI"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< const rho::String&, rho::String >(&ISystem::setHttpProxyURI) );
    m_mapPropAccessors["lockWindowSize"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setLockWindowSize) );
    m_mapPropAccessors["showKeyboard"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setShowKeyboard) );
    m_mapPropAccessors["fullScreen"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setFullScreen) );
    m_mapPropAccessors["localServerPort"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< int64, int64 >(&ISystem::setLocalServerPort) );
    m_mapPropAccessors["screenAutoRotate"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setScreenAutoRotate) );
    m_mapPropAccessors["screenSleeping"]->addSetter( new rho::apiGenerator::CMethodAccessor< ISystem>::CSetter< bool, bool >(&ISystem::setScreenSleeping) );
}

void CSystemBase::getProperty( const rho::String& propertyName, CMethodResult& oResult)
{
    CMethodAccessor< ISystem >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
        
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError("Get unknown property: " + propertyName);
        
    }
}

void CSystemBase::getProperties( const rho::Vector<::rho::String>& arrayofNames, CMethodResult& oResult)
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

void CSystemBase::getAllProperties(CMethodResult& oResult)
{
    oResult.set(m_hashProps);
}

void CSystemBase::setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
{
    CMethodAccessor< ISystem >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError("Set unknown property: " + propertyName);
        
    }
}

void CSystemBase::setProperties( const rho::Hashtable<::rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
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
