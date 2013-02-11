#include "../../../shared/SystemImplBase.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

#include <windows.h>

namespace rho {

using namespace apiGenerator;

class CSystemImpl: public CSystemImplBase
{
public:
    CSystemImpl(): CSystemImplBase(){}

    virtual void getScreenWidth(CMethodResult& oResult);
    virtual void getScreenHeight(CMethodResult& oResult);
    virtual void getRealScreenWidth(CMethodResult& oResult);
    virtual void getRealScreenHeight(CMethodResult& oResult);
    virtual void getScreenOrientation(CMethodResult& oResult);
    virtual void getPpiX(CMethodResult& oResult);
    virtual void getPpiY(CMethodResult& oResult);
    virtual void getPhoneNumber(CMethodResult& oResult);
    virtual void getDeviceOwnerEmail(CMethodResult& oResult);
    virtual void getDeviceOwnerName(CMethodResult& oResult);
    virtual void getPhoneId(CMethodResult& oResult);
    virtual void getDeviceName(CMethodResult& oResult);
    virtual void getOsVersion(CMethodResult& oResult);
    virtual void getIsMotorolaDevice(CMethodResult& oResult);
    virtual void getLocale(CMethodResult& oResult);
    virtual void getCountry(CMethodResult& oResult);
    virtual void getIsEmulator(CMethodResult& oResult);
    virtual void getHasCalendar(CMethodResult& oResult);
    virtual void getOemInfo(CMethodResult& oResult);
    virtual void getUuid(CMethodResult& oResult);
    virtual void getApplicationIconBadge(CMethodResult& oResult);
    virtual void setApplicationIconBadge( __int64 value, CMethodResult& oResult);
    virtual void getHttpProxyURI(CMethodResult& oResult);
    virtual void setHttpProxyURI( const rho::StringW& value, CMethodResult& oResult);
    virtual void getLockWindowSize(CMethodResult& oResult);
    virtual void setLockWindowSize( bool value, CMethodResult& oResult);
    virtual void getShowKeyboard(CMethodResult& oResult);
    virtual void setShowKeyboard( bool value, CMethodResult& oResult);
    virtual void getFullScreen(CMethodResult& oResult);
    virtual void setFullScreen( bool value, CMethodResult& oResult);
    virtual void getScreenAutoRotate(CMethodResult& oResult);
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult);

    virtual void applicationInstall( const rho::StringW& applicationUrl, CMethodResult& oResult);
    virtual void isApplicationInstalled( const rho::StringW& applicationName, CMethodResult& oResult);
    virtual void applicationUninstall( const rho::StringW& applicationName, CMethodResult& oResult);
    virtual void openUrl( const rho::StringW& url, CMethodResult& oResult);
    virtual void runApplication( const rho::StringW& appName,  const rho::StringW& params,  bool blockingCall, CMethodResult& oResult);
    virtual void setRegistrySetting( const rho::StringW& keyPath,  const rho::StringW& keyValue, CMethodResult& oResult);
    virtual void getRegistrySetting( const rho::StringW& keyPath, CMethodResult& oResult);
    virtual void setWindowFrame( __int64 x,  __int64 y,  __int64 width,  __int64 height, CMethodResult& oResult);
    virtual void setWindowPosition( __int64 x,  __int64 y, CMethodResult& oResult);
};

void CSystemImpl::getOsVersion(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    CSystemImpl::getOsVersion(oResult)
#else
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (!GetVersionEx(&osv))
    {
        LOG(ERROR) + "GetVersionEx failed.";
		oResult.set(L"0");
    }
    else
    {
	    wchar_t buf[50];
        memset(buf, 0, sizeof(buf));
	    _snwprintf(buf, sizeof(buf) - 1, L"%u.%u.%u", (unsigned)osv.dwMajorVersion,
		    (unsigned)osv.dwMinorVersion, (unsigned)osv.dwBuildNumber);

        oResult.set(StringW(buf));
    }
#endif
}

void CSystemImpl::getIsEmulator(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    CSystemImpl::getIsEmulator(oResult);
#else

#ifdef OS_WINDOWS_DESKTOP
    oResult.set(true);
#else

	TCHAR buf[255];
	SystemParametersInfo(SPI_GETOEMINFO, 255, &buf, NULL);
	bool bEmulator = (wcsnicmp(buf, _T("Microsoft DeviceEmulator"), 24)==0);

    oResult.set(bEmulator);
#endif

#endif
}

void CSystemImpl::getScreenWidth(CMethodResult& oResult){}
void CSystemImpl::getScreenHeight(CMethodResult& oResult){}
void CSystemImpl::getRealScreenWidth(CMethodResult& oResult){}
void CSystemImpl::getRealScreenHeight(CMethodResult& oResult){}
void CSystemImpl::getScreenOrientation(CMethodResult& oResult){}
void CSystemImpl::getPpiX(CMethodResult& oResult){}
void CSystemImpl::getPpiY(CMethodResult& oResult){}
void CSystemImpl::getPhoneNumber(CMethodResult& oResult){}
void CSystemImpl::getDeviceOwnerEmail(CMethodResult& oResult){}
void CSystemImpl::getDeviceOwnerName(CMethodResult& oResult){}

void CSystemImpl::getPhoneId(CMethodResult& oResult){}
void CSystemImpl::getDeviceName(CMethodResult& oResult){}

void CSystemImpl::getLocale(CMethodResult& oResult){}
void CSystemImpl::getCountry(CMethodResult& oResult){}

void CSystemImpl::getHasCalendar(CMethodResult& oResult){}

void CSystemImpl::getIsMotorolaDevice(CMethodResult& oResult)
{
}

void CSystemImpl::getOemInfo(CMethodResult& oResult){}
void CSystemImpl::getUuid(CMethodResult& oResult){}
void CSystemImpl::getApplicationIconBadge(CMethodResult& oResult){}
void CSystemImpl::setApplicationIconBadge( __int64 value, CMethodResult& oResult){}
void CSystemImpl::getHttpProxyURI(CMethodResult& oResult){}
void CSystemImpl::setHttpProxyURI( const rho::StringW& value, CMethodResult& oResult){}
void CSystemImpl::getLockWindowSize(CMethodResult& oResult){}
void CSystemImpl::setLockWindowSize( bool value, CMethodResult& oResult){}
void CSystemImpl::getShowKeyboard(CMethodResult& oResult){}
void CSystemImpl::setShowKeyboard( bool value, CMethodResult& oResult){}
void CSystemImpl::getFullScreen(CMethodResult& oResult){}
void CSystemImpl::setFullScreen( bool value, CMethodResult& oResult){}

void CSystemImpl::getScreenAutoRotate(CMethodResult& oResult){}
void CSystemImpl::setScreenAutoRotate( bool value, CMethodResult& oResult){}

void CSystemImpl::applicationInstall( const rho::StringW& applicationUrl, CMethodResult& oResult){}
void CSystemImpl::isApplicationInstalled( const rho::StringW& applicationName, CMethodResult& oResult){}
void CSystemImpl::applicationUninstall( const rho::StringW& applicationName, CMethodResult& oResult){}

void CSystemImpl::openUrl( const rho::StringW& url, CMethodResult& oResult){}
void CSystemImpl::runApplication( const rho::StringW& appName,  const rho::StringW& params,  bool blockingCall, CMethodResult& oResult){}

void CSystemImpl::setRegistrySetting( const rho::StringW& keyPath,  const rho::StringW& keyValue, CMethodResult& oResult){}
void CSystemImpl::getRegistrySetting( const rho::StringW& keyPath, CMethodResult& oResult){}
void CSystemImpl::setWindowFrame( __int64 x,  __int64 y,  __int64 width,  __int64 height, CMethodResult& oResult){}
void CSystemImpl::setWindowPosition( __int64 x,  __int64 y, CMethodResult& oResult){}

////////////////////////////////////////////////////////////////////////
class CSystemSingleton: public CSystemSingletonBase
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
    ISystem* createModuleByID(const rho::StringW& strID){ return new CSystemImpl(); }
};

extern "C" void Init_System_extension()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();
}

}