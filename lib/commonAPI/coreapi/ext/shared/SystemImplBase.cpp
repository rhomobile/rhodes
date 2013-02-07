#include "SystemImplBase.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

namespace rho {

using namespace apiGenerator;

/*static*/ CSystemImplBase* getInstance()
{
    return static_cast<CSystemImplBase*>(CSystemFactoryBase::getInstance()->getModuleByID(CSystemFactoryBase::getSystemSingletonS()->getDefaultID()));
}

void CSystemImplBase::getPlatform(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    rho::String strPlatform = RHOSIMCONF().getString("platform");

    if ( strPlatform.compare("bb") == 0 || strPlatform.compare("bb6") == 0 )
        oResult.set( L"Blackberry" );
    else if ( strPlatform.compare("wm") == 0 )
        oResult.set( L"WINDOWS" );
    else if ( strPlatform.compare("win32") == 0 )
        oResult.set( L"WINDOWS_DESKTOP" );
    else if ( strPlatform.compare("wp") == 0 )
        oResult.set( L"WP7" );
    else if ( strPlatform.compare("wp8") == 0 )
        oResult.set( L"WP8");
    else if ( strPlatform.compare("android") == 0 )
        oResult.set( L"ANDROID" );
    else if ( strPlatform.compare("iphone") == 0 )
        oResult.set( L"APPLE" );
    else if ( strPlatform.compare("symbian") == 0 )
        oResult.set( L"SYMBIAN" );
    else
        oResult.set( L"UNKNOWN" );

#else //!RHODES_EMULATOR

#if defined(OS_MACOSX)
	oResult.set( L"APPLE" );
#elif defined(OS_WINDOWS_DESKTOP)
	oResult.set( L"WINDOWS_DESKTOP" );
#elif defined(WINDOWS_PLATFORM)
#if defined(OS_WP8)
	oResult.set( L"WP8" );
#else
	oResult.set( L"WINDOWS" );
#endif
#elif defined(OS_SYMBIAN)
	oResult.set( L"SYMBIAN" );
#elif defined(OS_ANDROID)
    oResult.set( L"ANDROID" );
#elif defined(OS_LINUX)
    oResult.set( L"LINUX" );
#else
	oResult.set( L"UNKNOWN" );
#endif			

#endif
}

void CSystemImplBase::getHasCamera(CMethodResult& oResult){}
void CSystemImplBase::getScreenWidth(CMethodResult& oResult){}
void CSystemImplBase::getScreenHeight(CMethodResult& oResult){}
void CSystemImplBase::getRealScreenWidth(CMethodResult& oResult){}
void CSystemImplBase::getRealScreenHeight(CMethodResult& oResult){}
void CSystemImplBase::getScreenOrientation(CMethodResult& oResult){}
void CSystemImplBase::getPpiX(CMethodResult& oResult){}
void CSystemImplBase::getPpiY(CMethodResult& oResult){}
void CSystemImplBase::getPhoneNumber(CMethodResult& oResult){}
void CSystemImplBase::getDeviceOwnerEmail(CMethodResult& oResult){}
void CSystemImplBase::getDeviceOwnerName(CMethodResult& oResult){}
void CSystemImplBase::getDeviceId(CMethodResult& oResult){}
void CSystemImplBase::getPhoneId(CMethodResult& oResult){}
void CSystemImplBase::getDeviceName(CMethodResult& oResult){}

void CSystemImplBase::getOsVersion(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set( RHOSIMCONF().getString("os_version") );
#endif
}

void CSystemImplBase::getLocale(CMethodResult& oResult){}
void CSystemImplBase::getCountry(CMethodResult& oResult){}

void CSystemImplBase::getIsEmulator(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set( true );
#endif
}

void CSystemImplBase::getHasCalendar(CMethodResult& oResult){}
void CSystemImplBase::getIsMotorolaDevice(CMethodResult& oResult){}
void CSystemImplBase::getOemInfo(CMethodResult& oResult){}
void CSystemImplBase::getUuid(CMethodResult& oResult){}
void CSystemImplBase::getApplicationIconBadge(CMethodResult& oResult){}
void CSystemImplBase::setApplicationIconBadge( __int64 value, CMethodResult& oResult){}
void CSystemImplBase::getHttpProxyURI(CMethodResult& oResult){}
void CSystemImplBase::setHttpProxyURI( const rho::StringW& value, CMethodResult& oResult){}
void CSystemImplBase::getLockWindowSize(CMethodResult& oResult){}
void CSystemImplBase::setLockWindowSize( bool value, CMethodResult& oResult){}
void CSystemImplBase::getShowKeyboard(CMethodResult& oResult){}
void CSystemImplBase::setShowKeyboard( bool value, CMethodResult& oResult){}
void CSystemImplBase::getFullScreen(CMethodResult& oResult){}
void CSystemImplBase::setFullScreen( bool value, CMethodResult& oResult){}
void CSystemImplBase::getLocalServerPort(CMethodResult& oResult){}
void CSystemImplBase::setLocalServerPort( __int64 value, CMethodResult& oResult){}
void CSystemImplBase::getScreenAutoRotate(CMethodResult& oResult){}
void CSystemImplBase::setScreenAutoRotate( bool value, CMethodResult& oResult){}

void CSystemImplBase::applicationInstall( const rho::StringW& applicationUrl, CMethodResult& oResult){}
void CSystemImplBase::isApplicationInstalled( const rho::StringW& applicationName, CMethodResult& oResult){}
void CSystemImplBase::applicationUninstall( const rho::StringW& applicationName, CMethodResult& oResult){}
void CSystemImplBase::openUrl( const rho::StringW& url, CMethodResult& oResult){}
void CSystemImplBase::runApplication( const rho::StringW& appName,  const rho::StringW& params,  bool blockingCall, CMethodResult& oResult){}
void CSystemImplBase::unzipFile( const rho::StringW& localPathToZip, CMethodResult& oResult){}
void CSystemImplBase::zipFile( const rho::StringW& localPathToZip,  const rho::StringW& localPathToFile,  const rho::StringW& password, CMethodResult& oResult){}
void CSystemImplBase::zipFiles( const rho::StringW& localPathToZip,  const rho::StringW& basePath,  const rho::Vector<rho::StringW>& filePathsToZip,  const rho::StringW& password, CMethodResult& oResult){}
void CSystemImplBase::setRegistrySetting( const rho::StringW& keyPath,  const rho::StringW& keyValue, CMethodResult& oResult){}
void CSystemImplBase::getRegistrySetting( const rho::StringW& keyPath, CMethodResult& oResult){}
void CSystemImplBase::setWindowFrame( __int64 x,  __int64 y,  __int64 width,  __int64 height, CMethodResult& oResult){}
void CSystemImplBase::setWindowPosition( __int64 x,  __int64 y, CMethodResult& oResult){}

////////////////////////////////////////////////////////////////////////
class CSystemSingleton: public CModuleSingletonBase<ISystemSingleton>
{
public:
    ~CSystemSingleton(){}
    rho::StringW getInitialDefaultID(){return L"1";}
};

class CSystemFactory: public CSystemFactoryBase
{
public:
    ~CSystemFactory(){}

    ISystemSingleton* createModuleSingleton(){ return new CSystemSingleton(); }
    ISystem* createModuleByID(const rho::StringW& strID){ return new CSystemImplBase(strID); }
};

extern "C" void Init_System_extension()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();
}

}