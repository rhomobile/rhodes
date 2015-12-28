#ifndef RHODES_H
#define RHODES_H

#include "stdafx.h"
#include "MainWindow.h"
#include "browser/BrowserFactory.h"
#include "LogMemory.h"
#include "camera/Camera.h"

#include "logging/RhoLog.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "common/rhoparams.h"
#include "rho/rubyext/GeoLocationImpl.h"
#include "ruby/ext/rho/rhoruby.h"
#include "net/NetRequestImpl.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "common/app_build_capabilities.h"
#include "common/app_build_configs.h"
#include "api_generator/js_helpers.h"

using namespace rho;
using namespace rho::common;
using namespace std;
using namespace stdext;

extern int winversion;

extern UINT WM_CREATE_SHORTCUT;

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H
LPTSTR parseToken(LPCTSTR start, LPCTSTR* next_token);

extern "C" HWND getMainWnd();
extern "C" void rho_wm_impl_SetApplicationLicenseObj(void* pAppLicenseObj);
typedef LPCWSTR (WINAPI *PCL)(HWND, LPCWSTR, LPCWSTR, LPCWSTR);
typedef int (WINAPI *FUNC_IsLicenseOK)();
typedef void* (WINAPI *FUNC_GetAppLicenseObj)();
extern "C" void rho_wm_impl_CheckLicense();
extern "C" bool rho_wmimpl_is_logging_native_type();
extern "C" bool bParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode);
extern "C" void rho_ringtone_manager_stop();
extern "C" void rho_sysimpl_sethas_network(int nValue);
extern "C" void rho_sysimpl_sethas_cellnetwork(int nValue);
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" int rho_sys_check_rollback_bundle(const char* szRhoPath);
extern "C" void registerRhoExtension();
extern "C" void rho_webview_navigate(const char* url, int index);
extern "C" void createPowerManagementThread();
static void rho_platform_check_restart_application();
static void set_bridge_direct_callback();

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
class CEng;
//extern rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);
extern "C" CEng* rho_wmimpl_get_webkitbrowser(HWND hParentWnd, HINSTANCE hInstance);

#if !defined(APP_BUILD_CAPABILITY_SYMBOL)
extern "C" LRESULT	rho_wm_appmanager_ProcessOnTopMostWnd(WPARAM wParam, LPARAM lParam){ return 0;}
#else
extern "C" void initialiseRhoElementsExt();
#endif

#else
/*extern "C" void rho_wm_impl_SetApplicationLicenseObj(void* pAppLicenseObj)
{
    if (pAppLicenseObj)
        delete pAppLicenseObj;
}*/
#endif // APP_BUILD_CAPABILITY_WEBKIT_BROWSER
#ifdef APP_BUILD_CAPABILITY_SHARED_RUNTIME
extern "C" {
	void rho_wmimpl_set_configfilepath(const char* path);
	TCHAR* rho_wmimpl_get_startpage();
	void rho_wmimpl_set_startpage(const char* path);
	const char* rho_wmimpl_get_logpath();
	const char* rho_wmimpl_get_logurl();
	const char* rho_wmimpl_get_httpproxy();  
	bool rho_wmimpl_get_fullscreen();
	void rho_wmimpl_set_is_version2(const char* path);
	bool rho_wmimpl_get_is_version2();
    const wchar_t* rho_wmimpl_sharedconfig_getvalue(const wchar_t* szName);
#if !defined( APP_BUILD_CAPABILITY_WEBKIT_BROWSER ) && !defined(APP_BUILD_CAPABILITY_SYMBOL)
    bool rho_wmimpl_get_is_version2(){ return 1;}
    void rho_wmimpl_set_is_version2(const char* path){}
    void rho_wmimpl_set_configfilepath(const char* path){}
    void rho_wmimpl_set_configfilepath_wchar(const WCHAR* path){}
    void rho_wmimpl_set_startpage(const char* path){}
    TCHAR* rho_wmimpl_get_startpage(){ return L""; }
    const unsigned int* rho_wmimpl_get_logmemperiod(){ return 0; }
    const unsigned int* rho_wmimpl_get_logmaxsize(){ return 0; }
    const char* rho_wmimpl_get_logurl(){ return ""; }
    bool rho_wmimpl_get_fullscreen(){ return 0; }
    const char* rho_wmimpl_get_logpath(){ return ""; }
    int rho_wmimpl_is_loglevel_enabled(int nLogLevel){ return true; }
	const int* rho_wmimpl_get_loglevel(){ return NULL; }
    const wchar_t* rho_wmimpl_sharedconfig_getvalue(const wchar_t* szName){return 0;}
#endif

	const unsigned int* rho_wmimpl_get_logmaxsize();
	const int* rho_wmimpl_get_loglevel();
	const unsigned int* rho_wmimpl_get_logmemperiod();
};
#endif // APP_BUILD_CAPABILITY_SHARED_RUNTIME

class CRhodesModule : public CAtlExeModuleT< CRhodesModule >
{
    static HINSTANCE m_hInstance;
    CMainWindow m_appWindow;
    rho::String m_strRootPath, m_strRhodesPath, m_logPort, m_strRuntimePath, m_strAppName;
	bool m_isDefinedOSWince;
	bool m_bRestarting;
    bool m_bMinimized;
	bool m_isRhoConnectPush;
    bool m_startAtBoot, m_bJSApplication;
    String m_strTabName;

	HANDLE m_hMutex;

public :
	CRhodesModule()
	{
#ifdef OS_WINCE
		m_isDefinedOSWince = true;
#else
		m_isDefinedOSWince = false;
#endif
#ifdef RHO_NO_RUBY
		m_bJSApplication   = true;        
#else
		m_bJSApplication   = false;
#endif
	}
    static HINSTANCE GetModuleInstance(){return m_hInstance;}
    static void SetModuleInstance(HINSTANCE hInstance){m_hInstance = hInstance;}
    HWND GetMainWindow() { return m_appWindow.m_hWnd;}
	CMainWindow* GetMainWindowObject() { return &m_appWindow;}
	CMainWindow& GetAppWindow() { return m_appWindow; }
	HWND GetWebViewWindow(int index) { return m_appWindow.getWebViewHWND(); }

    bool ParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode ) throw( );
    HRESULT PreMessageLoop(int nShowCmd) throw();
	HRESULT PostMessageLoop() throw();
    void RunMessageLoop( ) throw( );
    const rho::String& getRhoRootPath();
    const rho::String& getRhoRuntimePath();
    const rho::String& getAppName();
    void createAutoStartShortcut();

    bool isJSApplication()const{ return m_bJSApplication; }
};
#endif