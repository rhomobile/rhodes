#include "../../../shared/SystemImplBase.h"

#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include "common/app_build_capabilities.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

#if defined( OS_WINCE ) && !defined( OS_PLATFORM_MOTCE )
#include <cfgmgrapi.h>
#include <getdeviceuniqueid.h>
#endif

extern "C"
{
void rho_sys_bring_to_front();
void rho_wmsys_run_appW(const wchar_t* szPath, const wchar_t* szParams );
int rho_sys_get_screen_width();
int rho_sys_get_screen_height();
const char* rho_sys_win32_getWebviewFramework();

#if defined(OS_WINDOWS_DESKTOP)
void rho_sys_set_window_frame(int x0, int y0, int width, int height);
void rho_sys_set_window_position(int x0, int y0);
void rho_sys_set_window_size(int width, int height);
void rho_sys_lock_window_size(int locked);
#endif
}

namespace rho {

using namespace apiGenerator;
using namespace common;

class CSystemImpl: public CSystemImplBase
{
public:
    CSystemImpl(): CSystemImplBase(){}

    virtual void getScreenWidth(CMethodResult& oResult);
    virtual void getScreenHeight(CMethodResult& oResult);
    virtual void getScreenOrientation(CMethodResult& oResult);
    virtual void getPpiX(CMethodResult& oResult);
    virtual void getPpiY(CMethodResult& oResult);
    virtual void getPhoneId(CMethodResult& oResult);
    virtual void getDeviceName(CMethodResult& oResult);
    virtual void getOsVersion(CMethodResult& oResult);
    virtual void getIsMotorolaDevice(CMethodResult& oResult);
    virtual void getLocale(CMethodResult& oResult);
    virtual void getCountry(CMethodResult& oResult);
    virtual void getIsEmulator(CMethodResult& oResult);
    virtual void getHasCalendar(CMethodResult& oResult);
    virtual void getHasCamera(CMethodResult& oResult);
    virtual void getOemInfo(CMethodResult& oResult);
    virtual void getUuid(CMethodResult& oResult);
    virtual void getHttpProxyURI(CMethodResult& oResult);
    virtual void setHttpProxyURI( const rho::String& value, CMethodResult& oResult);
    virtual void getLockWindowSize(CMethodResult& oResult);
    virtual void setLockWindowSize( bool value, CMethodResult& oResult);
    virtual void getShowKeyboard(CMethodResult& oResult);
    virtual void setShowKeyboard( bool value, CMethodResult& oResult);
    virtual void getScreenAutoRotate(CMethodResult& oResult);
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult);
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult);
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult);

    virtual void applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult);
    virtual void isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult);
    virtual void applicationUninstall( const rho::String& applicationName, CMethodResult& oResult);
    virtual void openUrl( const rho::String& url, CMethodResult& oResult);
    virtual void setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult);
    virtual void getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult);
    virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult);
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult);
    virtual void setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult);
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult);
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult);
    virtual void runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult);

    virtual void set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult);
    virtual void unset_http_proxy(rho::apiGenerator::CMethodResult& oResult);

};

void CSystemImpl::getOsVersion(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    CSystemImplBase::getOsVersion(oResult);
#else
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (!GetVersionEx(&osv))
    {
        LOG(ERROR) + "GetVersionEx failed.";
		oResult.set("0");
    }
    else
    {
	    wchar_t buf[50];
        memset(buf, 0, sizeof(buf));
	    _snwprintf(buf, sizeof(buf) - 1, L"%u.%u.%u", (unsigned)osv.dwMajorVersion,
		    (unsigned)osv.dwMinorVersion, (unsigned)osv.dwBuildNumber);

        oResult.set( StringW(buf));
    }
#endif
}

void CSystemImpl::getIsEmulator(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set(true);
#else

#ifdef OS_WINDOWS_DESKTOP
    oResult.set(false);
#else

	TCHAR buf[255];
	SystemParametersInfo(SPI_GETOEMINFO, 255, &buf, NULL);
	bool bEmulator = (wcsnicmp(buf, _T("Microsoft DeviceEmulator"), 24)==0);

    oResult.set(bEmulator);
#endif

#endif
}

void CSystemImpl::getScreenWidth(CMethodResult& oResult)
{
    oResult.set(rho_sys_get_screen_width());
}

void CSystemImpl::getScreenHeight(CMethodResult& oResult)
{
    oResult.set(rho_sys_get_screen_height());
}

void CSystemImpl::getScreenOrientation(CMethodResult& oResult)
{
    if (rho_sys_get_screen_width() <= rho_sys_get_screen_height()) 
	    oResult.set(SCREEN_PORTRAIT);
    else
        oResult.set(SCREEN_LANDSCAPE);
}

void CSystemImpl::getPpiX(CMethodResult& oResult)
{
	HWND hWndDesktop = GetDesktopWindow();
	HDC hdcDesktop = GetDC(hWndDesktop);
	int mms = GetDeviceCaps(hdcDesktop, HORZSIZE);
	int pixels = GetDeviceCaps(hdcDesktop, HORZRES);
	double ret = (pixels*25.4)/mms;

    oResult.set(ret);
}

void CSystemImpl::getPpiY(CMethodResult& oResult)
{
	HWND hWndDesktop = GetDesktopWindow();
	HDC hdcDesktop = GetDC(hWndDesktop);
	int mms = GetDeviceCaps(hdcDesktop, VERTSIZE);
	int pixels = GetDeviceCaps(hdcDesktop, VERTRES);
	double ret = (pixels*25.4)/mms;

    oResult.set(ret);
}

static void _toHexString(int i, String& strRes, int radix)
{
    char buf[33];
	bool neg= false;
	int f, n;
	if(i<0) { neg= true; i= -i; };
	f= 32;
	buf[f--]= 0;
	do
	{
		n= i%radix;
		if(n<10) buf[f]= '0'+n;
		else buf[f]= 'a'+n-10;
		i= i/radix;
		f--;
	}
	while(i>0);

	if(neg) 
        buf[f--]= '-';

    strRes += (buf+f+1);
}

void CSystemImpl::getPhoneId(CMethodResult& oResult)
{
    String strDeviceID;

#if defined( OS_WINCE ) && !defined( OS_PLATFORM_MOTCE )
    String strAppData = "RHODES_" + RHODESAPP().getAppName() + "_DEVICEID";

    BYTE rgDeviceId[20];
    DWORD cbDeviceId = sizeof(rgDeviceId);
    HRESULT hr = GetDeviceUniqueID( (PBYTE)(strAppData.c_str()),
       strAppData.length(),
       GETDEVICEUNIQUEID_V1,
       rgDeviceId,
       &cbDeviceId);

    if ( SUCCEEDED(hr) )
    {
        for(unsigned int i = 0; i < cbDeviceId; i++)
        {
            _toHexString( rgDeviceId[i], strDeviceID, 16);
        }
    }
#endif

    oResult.set( strDeviceID );
}

void CSystemImpl::getDeviceName(CMethodResult& oResult)
{
    String strRes;
#ifdef OS_WINDOWS_DESKTOP
	strRes = "Win32";
#else
	HKEY hKey;
	if (RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("Ident"), 0, KEY_READ, &hKey ) == ERROR_SUCCESS)
    {
        DWORD dwType = REG_SZ;
        DWORD dwDataSize = 0;
        if ( RegQueryValueEx( hKey, _T("name"), 0, &dwType, (PBYTE)NULL, &dwDataSize ) == ERROR_SUCCESS)
        {
            CAtlString deviceName;
            RegQueryValueEx( hKey, _T("name"), 0, &dwType, 
                (PBYTE)deviceName.GetBuffer((dwDataSize/sizeof(TCHAR) + sizeof(TCHAR))), &dwDataSize );
            deviceName.ReleaseBuffer();

            strRes = convertToStringA(deviceName.GetString());
        }
    }
	RegCloseKey(hKey);
#endif
    oResult.set(strRes);
}

void CSystemImpl::getLocale(CMethodResult& oResult)
{
	wchar_t szLang[20];
	int nRes = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SABBREVLANGNAME , szLang, sizeof(szLang)/sizeof(szLang[0]));
	szLang[2] = 0;
	wcslwr(szLang);

	oResult.set(szLang);
}

void CSystemImpl::getCountry(CMethodResult& oResult)
{
    wchar_t szCountry[20];
    int nRes = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SISO3166CTRYNAME , szCountry, sizeof(szCountry)/sizeof(szCountry[0]));
    szCountry[2] = 0;

  	oResult.set(szCountry);
}

void CSystemImpl::getHasCalendar(CMethodResult& oResult)
{
#if defined( OS_PLATFORM_MOTCE )
    oResult.set(false);
#else
    oResult.set(true);
#endif
}

void CSystemImpl::getIsMotorolaDevice(CMethodResult& oResult)
{
#if defined( APP_BUILD_CAPABILITY_MOTOROLA ) && defined( OS_WINCE )
    //get the system OEM string
    TCHAR szPlatform[MAX_PATH+1];
    memset(szPlatform, 0, MAX_PATH*sizeof(TCHAR));
    SystemParametersInfo(SPI_GETOEMINFO, MAX_PATH, szPlatform, 0);
    _wcslwr(szPlatform);
    if(wcsstr(szPlatform, L"symbol") || wcsstr(szPlatform, L"motorola"))
        oResult.set(true);
    else
        oResult.set(false);
#else
    oResult.set(false);
#endif
}

void CSystemImpl::getOemInfo(CMethodResult& oResult)
{
    //TODO: getOemInfo - copy from RE1
}

void CSystemImpl::getUuid(CMethodResult& oResult)
{
    //TODO: getUuid - copy from RE1
}

void CSystemImpl::getLockWindowSize(CMethodResult& oResult){}

void CSystemImpl::setLockWindowSize( bool value, CMethodResult& oResult)
{
#if defined(OS_WINDOWS_DESKTOP)
    rho_sys_lock_window_size(value?1:0);
#endif
}

void CSystemImpl::getShowKeyboard(CMethodResult& oResult)
{
    //TODO: getShowKeyboard
}

void CSystemImpl::setShowKeyboard( bool value, CMethodResult& oResult)
{
    //TODO: setShowKeyboard
}
/*
extern "C" void rho_wmsys_set_full_screen( int nFull );
extern "C" int rho_wmsys_get_full_screen();
void CSystemImpl::getFullScreen(CMethodResult& oResult)
{
    oResult.set(rho_wmsys_get_full_screen() != 0 ? true : false );
}

void CSystemImpl::setFullScreen( bool value, CMethodResult& oResult)
{
    rho_wmsys_set_full_screen( value ? 1 : 0 );
}*/

void CSystemImpl::getScreenAutoRotate(CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImpl::setScreenAutoRotate( bool value, CMethodResult& oResult)
{
    //TODO:setScreenAutoRotate
    //HKLM\System\GDI\Rotation\
    //HKCU\Software\HTC\HTCSensor\Gsensor:AutoRotation = 0 
}

extern "C" int rho_wmsys_has_touchscreen()
{
#if defined( OS_WINDOWS_DESKTOP ) || defined( OS_PLATFORM_MOTCE )
        return 1;
#else
        BOOL bRet;
        TCHAR oem[257];
#ifndef WIN32_PLATFORM_WFSP
        // fix for all supported platforms expect Smartphone 2003:
        // DeviceEmulator sould report it has a touch screen
        bRet = SystemParametersInfo(SPI_GETOEMINFO, sizeof(oem), oem, 0);
        if (bRet && wcsstr(oem, _T("DeviceEmulator"))!=NULL)
            return 1;
#endif

        bRet = SystemParametersInfo(SPI_GETPLATFORMTYPE, sizeof(oem), oem, 0);
        // SmartPhone has no touch screen
        if (bRet && wcsstr(oem, _T("SmartPhone"))!=NULL)
            return 0;
        // PocketPC has touch screen
        if (bRet && wcsstr(oem, _T("PocketPC"))!=NULL)
            return 1;

        // otherwise check mouse info
        int aMouseInfo[3] = {0};
        bRet = SystemParametersInfo(SPI_GETMOUSE, sizeof(aMouseInfo), aMouseInfo, 0);
        return (bRet && aMouseInfo[0] != 0) ? 1 : 0;
#endif
}

void CSystemImpl::getHasTouchscreen(CMethodResult& oResult)
{
    oResult.set( rho_wmsys_has_touchscreen() == 1 ? true : false );
}

void CSystemImpl::getScreenSleeping(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImpl::setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult)
{
    //TODO: setScreenSleeping
    // http://stackoverflow.com/questions/246407/disable-sleep-mode-in-windows-mobile-6
}

void CSystemImpl::applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult)
{
#ifdef OS_WINDOWS_DESKTOP
	String sUrl = applicationUrl;
    CFilePath oFile(sUrl);
	String filename = RHODESAPP().getRhoUserPath()+ oFile.getBaseName();
	if (CRhoFile::isFileExist(filename.c_str()) && (CRhoFile::deleteFile(filename.c_str()) != 0)) {
		LOG(ERROR) + "rho_sys_app_install() file delete failed: " + filename;
	} else {
		NetRequest NetRequest;
		NetResponse resp = getNetRequest(&NetRequest).pullFile(sUrl, filename, NULL, NULL);
		if (resp.isOK()) {
			StringW filenameW = convertToStringW(filename);
			LOG(INFO) + "Downloaded " + sUrl + " to " + filename;
			rho_wmsys_run_appW(filenameW.c_str(), L"");
		} else {
			LOG(ERROR) + "rho_sys_app_install() download failed: " + sUrl;
		}
	}
#else
    rho_wmsys_run_appW( convertToStringW(applicationUrl).c_str(), 0 );
#endif
}

static LONG _openRegAppPath( const rho::String& applicationName, CRegKey& oKey, StringW& strKeyPath)
{
    CFilePath oPath( applicationName );
    String strAppName = oPath.getFolderName();

#ifdef OS_WINDOWS_DESKTOP
    strKeyPath = L"SOFTWARE\\";
#else
    strKeyPath = L"Software\\Apps\\";
#endif

    strKeyPath += convertToStringW(strAppName);
    String_replace(strKeyPath, '/', '\\' );

#ifdef OS_WINDOWS_DESKTOP
    return oKey.Open(HKEY_CURRENT_USER, strKeyPath.c_str(), KEY_READ|KEY_WOW64_64KEY );
#else
    return oKey.Open(HKEY_LOCAL_MACHINE, strKeyPath.c_str(), KEY_READ ); 
#endif
}

void CSystemImpl::isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult)
{
    bool bRet = false;
#ifdef OS_WINDOWS_DESKTOP
    CRegKey oKey;
    StringW strKeyPath;
    LONG res = _openRegAppPath(applicationName, oKey, strKeyPath);

	bRet = res == ERROR_SUCCESS;
#else
    CFilePath oPath( applicationName );
    StringW strAppName = convertToStringW(oPath.getFolderName());
    
    StringW strRequest = 
        L"<wap-provisioningdoc><characteristic type=\"UnInstall\">"
        L"<characteristic-query type=\"";
    strRequest += strAppName + L"\"/>"
        L"</characteristic></wap-provisioningdoc>"; 

#if defined( OS_WINCE ) && !defined( OS_PLATFORM_MOTCE )
    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    hr = DMProcessConfigXML(strRequest.c_str(), CFGFLAG_PROCESS, &wszOutput);
    if (FAILED(hr) || !wszOutput )
        LOG(ERROR) + "DMProcessConfigXML failed: " + hr;
    else
    {
        StringW strResp = L"<characteristic type=\"";
        strResp += strAppName + L"\">";
        bRet = wcsstr(wszOutput, strResp.c_str()) != 0;
    }

    if ( wszOutput )
        free( wszOutput );
#endif
#endif

    oResult.set(bRet);
}

void CSystemImpl::applicationUninstall( const rho::String& applicationName, CMethodResult& oResult)
{
#ifdef OS_WINDOWS_DESKTOP
    CRegKey oKey;
    StringW strKeyPath;
    LONG res = _openRegAppPath(applicationName, oKey, strKeyPath);
    if ( res != ERROR_SUCCESS )
    {
        LOG(ERROR) + "Cannot open registry key: " + strKeyPath + "; Code:" + res;
    } else
    {
        TCHAR szBuf[MAX_PATH+1];
        ULONG nChars = MAX_PATH;

        res = oKey.QueryStringValue(L"Uninstaller", szBuf, &nChars);
        if ( res != ERROR_SUCCESS )
            LOG(ERROR) + "Cannot read registry key: Uninstaller; Code:" + res;
        else
        {
            StringW strFullPathW = szBuf;
			rho_wmsys_run_appW( strFullPathW.c_str(), L"" );
		}
	}
#else
    CFilePath oPath( applicationName );
    StringW strAppName = convertToStringW(oPath.getFolderName());
    
    StringW strRequest = 
        L"<wap-provisioningdoc><characteristic type=\"UnInstall\">"
        L"<characteristic type=\"";
    strRequest += strAppName + L"\">"
        L"<parm name=\"uninstall\" value=\"1\"/>"
        L"</characteristic>"
        L"</characteristic></wap-provisioningdoc>";

#if defined( OS_WINCE )&& !defined( OS_PLATFORM_MOTCE )
    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    hr = DMProcessConfigXML(strRequest.c_str(), CFGFLAG_PROCESS, &wszOutput);
    if (FAILED(hr) || !wszOutput )
        LOG(ERROR) + "DMProcessConfigXML failed: " + hr;
    else
    {
    }

    if ( wszOutput )
        free( wszOutput );
#endif
#endif
}

void CSystemImpl::openUrl( const rho::String& url, CMethodResult& oResult)
{
    rho_wmsys_run_appW( convertToStringW(url).c_str(), 0 );
}

void CSystemImpl::runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult)
{
    CRegKey oKey;
    StringW strKeyPath;
    LONG res = _openRegAppPath(appName, oKey, strKeyPath);

    if ( res != ERROR_SUCCESS )
    {
        LOG(ERROR) + "Cannot open registry key: " + strKeyPath + "; Code:" + res;
    } else
    {
        TCHAR szBuf[MAX_PATH+1];
        ULONG nChars = MAX_PATH;

        res = oKey.QueryStringValue(L"InstallDir", szBuf, &nChars);
        if ( res != ERROR_SUCCESS )
            LOG(ERROR) + "Cannot read registry key: InstallDir; Code:" + res;
        else
        {
            StringW strFullPath = szBuf;

            if ( strFullPath[strFullPath.length()-1] != '/' && strFullPath[strFullPath.length()-1] != '\\' )
                strFullPath += L"\\";

			StringW strBaseName;
            CFilePath oPath(appName);
            convertToStringW(oPath.getBaseName(), strBaseName);
            strFullPath += strBaseName;

            rho_wmsys_run_appW( strFullPath.c_str(), convertToStringW(params).c_str());
        }
    }
}

void CSystemImpl::setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{
    //TODO: setRegistrySetting - copy from RE1
}

void CSystemImpl::getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult)
{
    //TODO: getRegistrySetting - copy from RE1
    oResult.set("");
}

void CSystemImpl::setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult)
{
#if defined(OS_WINDOWS_DESKTOP)
    rho_sys_set_window_frame( (int)x, (int)y, (int)width, (int)height);
#endif

}

void CSystemImpl::setWindowPosition( int x,  int y, CMethodResult& oResult)
{
#if defined(OS_WINDOWS_DESKTOP)
    rho_sys_set_window_position((int)x, (int)y);
#endif
}

void CSystemImpl::setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult)
{
#if defined(OS_WINDOWS_DESKTOP)
    rho_sys_set_window_size((int)width, (int)height);
#endif
}

void CSystemImpl::getWebviewFramework(rho::apiGenerator::CMethodResult& oResult)
{
    String strRes = "";

#if defined(OS_WINDOWS_DESKTOP)
	strRes = rho_sys_win32_getWebviewFramework();
#elif defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
	strRes = "WEBKIT/MOTOROLA";
#else
	strRes = "IE";
    //TODO: get IE version for WM/CE
#endif

    oResult.set(strRes);
}

void CSystemImpl::bringToFront(rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_bring_to_front();
}

extern "C" void rho_sys_set_http_proxy_url(const char* url);
void CSystemImpl::set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_set_http_proxy_url( proxyURI.c_str() );
}

extern "C" void rho_sys_unset_http_proxy();
void CSystemImpl::unset_http_proxy(rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_unset_http_proxy();
}

extern "C" const char* rho_sys_get_http_proxy_url();
void CSystemImpl::getHttpProxyURI(CMethodResult& oResult)
{
    oResult.set(rho_sys_get_http_proxy_url());
}

void CSystemImpl::setHttpProxyURI( const rho::String& value, CMethodResult& oResult)
{
    if ( value.length() )
        rho_sys_set_http_proxy_url( value.c_str() );
    else
        rho_sys_unset_http_proxy();
}


void CSystemImpl::getHasCamera(CMethodResult& oResult)
{
#if defined( OS_WINDOWS_DESKTOP ) || defined( OS_PLATFORM_MOTCE )
    oResult.set(false);
#else
    oResult.set(true);
#endif

}

////////////////////////////////////////////////////////////////////////

class CSystemFactory: public CSystemFactoryBase
{
public:
    ~CSystemFactory(){}

    ISystemSingleton* createModuleSingleton(){ return new CSystemImpl(); }
};

}

extern "C" void Init_System()
{
    rho::CSystemFactory::setInstance( new rho::CSystemFactory() );
    rho::Init_System_API();
}
