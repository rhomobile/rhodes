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

void CSystemBase::getProperty( const rho::StringW& propertyName, CMethodResult& oResult)
{

    if (false){}

    else if (_wcsicmp(L"platform",propertyName.c_str()) == 0){

        getPlatform( oResult );
    }
    else if (_wcsicmp(L"hasCamera",propertyName.c_str()) == 0){

        getHasCamera( oResult );
    }
    else if (_wcsicmp(L"screenWidth",propertyName.c_str()) == 0){

        getScreenWidth( oResult );
    }
    else if (_wcsicmp(L"screenHeight",propertyName.c_str()) == 0){

        getScreenHeight( oResult );
    }
    else if (_wcsicmp(L"realScreenWidth",propertyName.c_str()) == 0){

        getRealScreenWidth( oResult );
    }
    else if (_wcsicmp(L"realScreenHeight",propertyName.c_str()) == 0){

        getRealScreenHeight( oResult );
    }
    else if (_wcsicmp(L"screenOrientation",propertyName.c_str()) == 0){

        getScreenOrientation( oResult );
    }
    else if (_wcsicmp(L"ppiX",propertyName.c_str()) == 0){

        getPpiX( oResult );
    }
    else if (_wcsicmp(L"ppiY",propertyName.c_str()) == 0){

        getPpiY( oResult );
    }
    else if (_wcsicmp(L"phoneNumber",propertyName.c_str()) == 0){

        getPhoneNumber( oResult );
    }
    else if (_wcsicmp(L"deviceOwnerEmail",propertyName.c_str()) == 0){

        getDeviceOwnerEmail( oResult );
    }
    else if (_wcsicmp(L"deviceOwnerName",propertyName.c_str()) == 0){

        getDeviceOwnerName( oResult );
    }
    else if (_wcsicmp(L"deviceId",propertyName.c_str()) == 0){

        getDeviceId( oResult );
    }
    else if (_wcsicmp(L"phoneId",propertyName.c_str()) == 0){

        getPhoneId( oResult );
    }
    else if (_wcsicmp(L"deviceName",propertyName.c_str()) == 0){

        getDeviceName( oResult );
    }
    else if (_wcsicmp(L"osVersion",propertyName.c_str()) == 0){

        getOsVersion( oResult );
    }
    else if (_wcsicmp(L"locale",propertyName.c_str()) == 0){

        getLocale( oResult );
    }
    else if (_wcsicmp(L"country",propertyName.c_str()) == 0){

        getCountry( oResult );
    }
    else if (_wcsicmp(L"isEmulator",propertyName.c_str()) == 0){

        getIsEmulator( oResult );
    }
    else if (_wcsicmp(L"hasCalendar",propertyName.c_str()) == 0){

        getHasCalendar( oResult );
    }
    else if (_wcsicmp(L"isMotorolaDevice",propertyName.c_str()) == 0){

        getIsMotorolaDevice( oResult );
    }
    else if (_wcsicmp(L"oemInfo",propertyName.c_str()) == 0){

        getOemInfo( oResult );
    }
    else if (_wcsicmp(L"uuid",propertyName.c_str()) == 0){

        getUuid( oResult );
    }
    else if (_wcsicmp(L"applicationIconBadge",propertyName.c_str()) == 0){

        getApplicationIconBadge( oResult );
    }
    else if (_wcsicmp(L"httpProxyURI",propertyName.c_str()) == 0){

        getHttpProxyURI( oResult );
    }
    else if (_wcsicmp(L"lockWindowSize",propertyName.c_str()) == 0){

        getLockWindowSize( oResult );
    }
    else if (_wcsicmp(L"showKeyboard",propertyName.c_str()) == 0){

        getShowKeyboard( oResult );
    }
    else if (_wcsicmp(L"fullScreen",propertyName.c_str()) == 0){

        getFullScreen( oResult );
    }
    else if (_wcsicmp(L"localServerPort",propertyName.c_str()) == 0){

        getLocalServerPort( oResult );
    }
    else if (_wcsicmp(L"screenAutoRotate",propertyName.c_str()) == 0){

        getScreenAutoRotate( oResult );
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

        res[arrayofNames[i]] = oResult.toStringW();
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
