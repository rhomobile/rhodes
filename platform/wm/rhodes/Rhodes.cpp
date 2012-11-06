/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"
#include "MainWindow.h"
#include "IEBrowserEngine.h"
//#if !defined(RHODES_EMULATOR) && !defined(OS_WINDOWS_DESKTOP)
#include "LogMemory.h"
//#endif

#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "common/rhoparams.h"
#include "rho/rubyext/GeoLocationImpl.h"
#include "ruby/ext/rho/rhoruby.h"
#include "net/NetRequestImpl.h"
#include "sync/ClientRegister.h"
#include "common/RhoFilePath.h"
#include "common/app_build_capabilities.h"
#include "common/app_build_configs.h"

using namespace rho;
using namespace rho::common;
using namespace std;
using namespace stdext;

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H

LPTSTR parseToken(LPCTSTR start, LPCTSTR* next_token);
extern "C" void rho_ringtone_manager_stop();
extern "C" void rho_sysimpl_sethas_network(int nValue);
extern "C" void rho_sysimpl_sethas_cellnetwork(int nValue);
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" int rho_sys_check_rollback_bundle(const char* szRhoPath);
extern "C" void registerRhoExtension();
extern "C" void rho_webview_navigate(const char* url, int index);

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
class CEng;
extern rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);
extern "C" CEng* rho_wmimpl_get_webkitbrowser(HWND hParentWnd, HINSTANCE hInstance);
#else
extern "C" void rho_wm_impl_SetApplicationLicenseObj(void* pAppLicenseObj)
{
    if (pAppLicenseObj)
        delete pAppLicenseObj;
}
#endif // APP_BUILD_CAPABILITY_WEBKIT_BROWSER
#ifdef APP_BUILD_CAPABILITY_SHARED_RUNTIME
extern "C" {
	void rho_wmimpl_set_configfilepath(const char* path);
	TCHAR* rho_wmimpl_get_startpage();
	void rho_wmimpl_set_startpage(const char* path);
	const char* rho_wmimpl_get_logpath();
	const char* rho_wmimpl_get_logurl();
	bool rho_wmimpl_get_fullscreen();
	void rho_wmimpl_set_is_version2(const char* path);
	bool rho_wmimpl_get_is_version2();
    const wchar_t* rho_wmimpl_sharedconfig_getvalue(const wchar_t* szName);
#if !defined( APP_BUILD_CAPABILITY_WEBKIT_BROWSER ) && !defined(APP_BUILD_CAPABILITY_MOTOROLA)
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

#if !defined( APP_BUILD_CAPABILITY_MOTOROLA ) && !defined( APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
extern "C" bool rho_wmimpl_get_resize_on_sip()
{
    return true;
}
#endif

#if defined(_WIN32_WCE) && !defined(OS_PLATFORM_MOTCE)
#include <regext.h>

// Global Notification Handle
HREGNOTIFY g_hNotify = NULL, g_hNotifyCell = NULL;

// ConnectionsNetworkCount
// Gets a value indicating the number of network connections that are currently connected.
#define SN_CONNECTIONSNETWORKCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSNETWORKCOUNT_PATH TEXT("System\\State\\Hardware")
#define SN_CONNECTIONSNETWORKCOUNT_VALUE TEXT("WiFi")

#define SN_CELLSYSTEMCONNECTED_PATH TEXT("System\\State\\Phone")
#define SN_CELLSYSTEMCONNECTED_VALUE TEXT("Cellular System Connected")

#endif

//This is hack. MC4900 device failed to enable barcode after webkit initialization. So we enable it before.
#if defined(APP_BUILD_CAPABILITY_BARCODE) && defined(APP_BUILD_CAPABILITY_MOTOROLA) && defined (OS_PLATFORM_MOTCE)
extern "C" void rho_scanner_before_webkit();
extern "C" void rho_scanner_after_webkit();
struct CBarcodeInit
{
    bool m_bMC4900;
    CBarcodeInit()
    {
        m_bMC4900 = false;
/*
        OSVERSIONINFO osv = {0};
		osv.dwOSVersionInfoSize = sizeof(osv);
		if (GetVersionEx(&osv))
			m_bMC4900 = osv.dwMajorVersion == 5;

        RAWLOG_INFO1("CBarcodeInit : OS version :  %d", osv.dwMajorVersion);

        if ( m_bMC4900 )
            rho_scanner_before_webkit();*/
    }

    static DWORD afterWebkit(LPVOID ){ rho_scanner_after_webkit(); return 0; }
    ~CBarcodeInit()
    {
        if ( m_bMC4900 )
            CloseHandle(CreateThread(NULL, 0, &afterWebkit, 0, 0, NULL));
    }
};
#else
struct CBarcodeInit{};
#endif

class CRhodesModule : public CAtlExeModuleT< CRhodesModule >
{
    static HINSTANCE m_hInstance;
    CMainWindow m_appWindow;
    rho::String m_strRootPath, m_strRhodesPath, m_logPort, m_strRuntimePath, m_strAppName;//, m_strDebugHost, m_strDebugPort;*/
	int m_nRestarting;
	bool m_isRhoConnectPush;
#ifndef RHODES_EMULATOR
	HANDLE m_hMutex;
#endif
    CExtManager m_oExtManager;

#ifdef OS_WINDOWS_DESKTOP
    String m_strHttpProxy;
#endif

public :
    static HINSTANCE GetModuleInstance(){return m_hInstance;}
    static void SetModuleInstance(HINSTANCE hInstance){m_hInstance = hInstance;}
    HWND GetMainWindow() { return m_appWindow.m_hWnd;}
	CMainWindow* GetMainWindowObject() { return &m_appWindow;}
	CMainWindow& GetAppWindow() { return m_appWindow; }
	HWND GetWebViewWindow(int index) {	return m_appWindow.getWebViewHWND(
#if defined(OS_WINDOWS_DESKTOP)
            index
#endif
        );
	}

    bool ParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode ) throw( );
    HRESULT PreMessageLoop(int nShowCmd) throw();
    void RunMessageLoop( ) throw( );
    const rho::String& getRhoRootPath();
    const rho::String& getRhoRuntimePath();
    const rho::String& getAppName();
};

void parseHttpProxyURI(const rho::String &http_proxy);

static String g_strCmdLine;
HINSTANCE CRhodesModule::m_hInstance;
CRhodesModule _AtlModule;
bool g_restartOnExit = false;

#if !defined(OS_WINDOWS_DESKTOP)
rho::IBrowserEngine* rho_wmimpl_createBrowserEngine(HWND hwndParent)
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    return rho_wmimpl_get_webkitBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
#else
    return new CIEBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
#endif //APP_BUILD_CAPABILITY_WEBKIT_BROWSER
}
#endif //!OS_WINDOWS_DESKTOP

bool CRhodesModule::ParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode ) throw( )
{
	m_nRestarting = 1;
	LPCTSTR lpszToken = lpCmdLine;
	LPCTSTR nextToken;
    getRhoRootPath();

	m_logPort = "";
	m_isRhoConnectPush = false;

	while (lpszToken != NULL)
	{
		// skip leading spaces and double-quote (if present)
	    bool doubleQuote = false;
		while ((*lpszToken != 0) && ((*lpszToken==' ') || ((!doubleQuote) && (*lpszToken=='"')))) {
			if (*lpszToken=='"')
				doubleQuote = true;
			lpszToken++;
		}
		// skip leading spaces and check for leading '/' or '-' of command line option
		bool isCmdLineOpt = false;
		while ((*lpszToken != 0) && ((*lpszToken==' ') || ((!isCmdLineOpt) && ((*lpszToken=='/') || (*lpszToken=='-'))))) {
			if ((*lpszToken=='/') || (*lpszToken=='-'))
				isCmdLineOpt = true;
			lpszToken++;
		}
		// finish command line processing on EOL
		if (*lpszToken == 0) break;

		// if option starts with double-quote, find its end by next double-quote;
		// otherwise the end will be found automatically
		nextToken = doubleQuote ? FindOneOf(lpszToken, _T("\"")) : NULL;

		//parseToken will allocate extra byte at the end of the returned token value
		LPTSTR value = parseToken( lpszToken, &nextToken );

		if (isCmdLineOpt) {
			if (WordCmpI(lpszToken, _T("Restarting"))==0) {
				m_nRestarting = 10;
			}

			if (WordCmpI(lpszToken, _T("rhoconnectpush"))==0) {
				m_isRhoConnectPush = true;
			}

			else if (wcsncmp(lpszToken, _T("log"), 3)==0) 
			{
				if (value) {
					m_logPort = convertToStringA(value);
				}
				else {
					m_logPort = rho::String("11000");
				}
			}

#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
			else if (wcsnicmp(lpszToken, _T("s"),1)==0)
			{
				if (value) {
					// RhoElements v1.0 compatibility mode
					String strPath = convertToStringA(value);
					rho_wmimpl_set_startpage(strPath.c_str());
				}
			}
			else if (wcsnicmp(lpszToken, _T("c"),1)==0)
			{
				if (value) {
					String strPath = convertToStringA(value);
					if (strPath.substr(0,7).compare("file://")==0)
						strPath.erase(0,7);
					rho_wmimpl_set_configfilepath(strPath.c_str());
				}
			}
#endif // APP_BUILD_CAPABILITY_SHARED_RUNTIME

#if defined(OS_WINDOWS_DESKTOP)
			else if (wcsncmp(lpszToken, _T("http_proxy_url"),14)==0) 
			{
				if (value) {
					m_strHttpProxy = convertToStringA(value);
				}
				else 
					LOG(WARNING) + "invalid value for \"http_proxy_url\" cmd parameter";

			} else if (wcsncmp(lpszToken, _T("approot"),7)==0) 
			{
				if (value) {
					m_strRootPath = convertToStringA(value);
					String_replace(m_strRootPath, '\\', '/');
					if (m_strRootPath.at(m_strRootPath.length()-1)!='/')
						m_strRootPath.append("/");
				}
			} else if (wcsncmp(lpszToken, _T("rhodespath"),10)==0) 
			{
				if (value) {
					m_strRhodesPath = convertToStringA(value);
					String_replace(m_strRhodesPath, '\\', '/');
				}
			} /* else if (wcsncmp(lpszToken, _T("appname"),7)==0) 
			{
				if (value) {
					m_strAppNameW = convertToStringW(value);
				}
			} else if (wcsncmp(lpszToken, _T("debughost"),9)==0) 
			{
				if (value) {
					m_strDebugHost = convertToStringA(value);
				}
			} else if (wcsncmp(lpszToken, _T("debugport"),9)==0) 
			{
				if (value) {
					m_strDebugPort = convertToStringA(value);
				}
			} */
#else
			else if (wcsnicmp(lpszToken, _T("approot"),7)==0)
			{
				if (value) {
					// RhoElements v2.0 Shared Runtime command line parameter
					m_strRootPath = convertToStringA(value);
					if (m_strRootPath.substr(0,7).compare("file://")==0)
						m_strRootPath.erase(0,7);
					String_replace(m_strRootPath, '\\', '/');
					if (m_strRootPath.at(m_strRootPath.length()-1)!='/')
						m_strRootPath.append("/");
					m_strRootPath.append("rho/");
#ifdef APP_BUILD_CAPABILITY_SHARED_RUNTIME
					rho_wmimpl_set_is_version2(m_strRootPath.c_str());
#endif
        		}
			}
#endif
		}
		if (value) free(value);
		lpszToken = nextToken;
	}

	return __super::ParseCommandLine(lpCmdLine, pnRetCode);
}

extern "C" void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg)
{
    //alert_show_status( "Bundle update.", ("Error happen when replace bundle: " + strError).c_str(), 0 );
    StringW strMsgW, strTitleW;
    convertToStringW(szMsg, strMsgW);
    convertToStringW(szTitle, strTitleW);
    ::MessageBoxW(0, strMsgW.c_str(), strTitleW.c_str(), MB_ICONERROR | MB_OK);
}

extern "C" void rho_scanner_TEST(HWND hwnd);
extern "C" void rho_scanner_TEST2();
extern "C" void rho_wm_impl_CheckLicense();

// This method is called immediately before entering the message loop.
// It contains initialization code for the application.
// Returns:
// S_OK => Success. Continue with RunMessageLoop() and PostMessageLoop().
// S_FALSE => Skip RunMessageLoop(), call PostMessageLoop().
// error code => Failure. Skip both RunMessageLoop() and PostMessageLoop().
HRESULT CRhodesModule::PreMessageLoop(int nShowCmd) throw()
{
    HRESULT hr = __super::PreMessageLoop(nShowCmd);
    if (FAILED(hr))
    {
        return hr;
    }
    // Note: In this sample, we don't respond differently to different hr success codes.

#if !defined(OS_WINDOWS_DESKTOP)
    // Allow only one instance of the application.
    // the "| 0x01" activates the correct owned window of the previous instance's main window
	HWND hWnd = NULL;
	for (int wait = 0; wait < m_nRestarting; wait++) {
		hWnd = FindWindow(CMainWindow::GetWndClassInfo().m_wc.lpszClassName, NULL);
		if (hWnd && m_nRestarting > 1) {
			Sleep(1000);
		} else {
			break;
		}
	}
	//EnumWindows(EnumRhodesWindowsProc, (LPARAM)&hWnd);

	if (hWnd)
	{
        SendMessage( hWnd, PB_WINDOW_RESTORE, NULL, TRUE);
        SetForegroundWindow( hWnd );
		return S_FALSE;
	}

	// creating mutex
/*	m_hMutex = CreateMutex(NULL, TRUE, CMainWindow::GetWndClassInfo().m_wc.lpszClassName);
	if (m_hMutex==NULL) {
		// Failed to create mutex
		return S_FALSE;
	}
	if ((GetLastError() == ERROR_ALREADY_EXISTS) && (WaitForSingleObject(m_hMutex, 60000L) != WAIT_OBJECT_0)) {
        rho_sys_impl_exit_with_errormessage( "Initialization", "Another instance of the application is running. Please, exit it or use Task Manager to terminate it.");
        return S_FALSE;
	}*/
#endif

    if ( !rho_sys_check_rollback_bundle(rho_native_rhopath()) )
    {
        rho_sys_impl_exit_with_errormessage( "Bundle update", "Application is corrupted. Reinstall it, please.");
        return S_FALSE;
    }

#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    rho_logconf_Init((rho_wmimpl_get_logpath()[0]==0 ? m_strRootPath.c_str() : rho_wmimpl_get_logpath()), m_strRootPath.c_str(), m_logPort.c_str());
    if (rho_wmimpl_get_logurl()[0]!=0)
		LOGCONF().setLogURL(rho_wmimpl_get_logurl());
	if (rho_wmimpl_get_logmaxsize())
		LOGCONF().setMaxLogFileSize(*rho_wmimpl_get_logmaxsize());
    if (rho_wmimpl_get_loglevel())
		LOGCONF().setMinSeverity(*rho_wmimpl_get_loglevel());
    if (rho_wmimpl_get_fullscreen())
        RHOCONF().setBool("full_screen", true, false);
	if (rho_wmimpl_get_logmemperiod())
		LOGCONF().setCollectMemoryInfoInterval(*rho_wmimpl_get_logmemperiod());
#else
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());
#endif // APP_BUILD_CAPABILITY_SHARED_RUNTIME

//#if !defined(RHODES_EMULATOR) && !defined(OS_WINDOWS_DESKTOP)
	LOGCONF().setMemoryInfoCollector(CLogMemory::getInstance());
//#endif // RHODES_EMULATOR

#ifdef RHODES_EMULATOR
    RHOSIMCONF().setAppConfFilePath(CFilePath::join( m_strRootPath, RHO_EMULATOR_DIR"/rhosimconfig.txt").c_str());
    RHOSIMCONF().loadFromFile();
    if ( m_strRhodesPath.length() > 0 )
        RHOSIMCONF().setString("rhodes_path", m_strRhodesPath, false );
    RHOCONF().setString("rhosim_platform", RHOSIMCONF().getString("platform"), false);
    RHOCONF().setString("app_version", RHOSIMCONF().getString("app_version"), false);
	String start_path = RHOSIMCONF().getString("start_path");
    if ( start_path.length() > 0 )
	    RHOCONF().setString("start_path", start_path, false);
    RHOSIMCONF().setString("ext_path", RHOSIMCONF().getString("ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/debugger;"), false);
    RHOSIMCONF().setString("ext_path", RHOSIMCONF().getString("ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/uri;"), false);
    RHOSIMCONF().setString("ext_path", RHOSIMCONF().getString("ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/timeout;"), false);
    RHOSIMCONF().setString("ext_path", RHOSIMCONF().getString("ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/digest;"), false);
    RHOSIMCONF().setString("ext_path", RHOSIMCONF().getString("ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/openssl;"), false);
#endif

    if ( !rho_rhodesapp_canstartapp(g_strCmdLine.c_str(), " /-,") )
    {
#ifdef OS_WINDOWS_DESKTOP
	    ::MessageBoxW(0, L"This is hidden app and can be started only with security key.", L"Security Token Verification Failed", MB_ICONERROR | MB_OK);
#endif
		LOG(INFO) + "This is hidden app and can be started only with security key.";
		if (RHOCONF().getString("invalid_security_token_start_path").length() <= 0)
			return S_FALSE;
    }

	LOG(INFO) + "Rhodes started";
#ifdef OS_WINDOWS_DESKTOP
	if (m_strHttpProxy.length() > 0) {
		parseHttpProxyURI(m_strHttpProxy);
	} else
#endif
	{
		if (RHOCONF().isExist("http_proxy_url")) {
			parseHttpProxyURI(RHOCONF().getString("http_proxy_url"));
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
		} else {
			// it's important to call this method from here to perform
			// a proper initialization of proxy implementation for Win32
			GetAppWindow().setProxy();
#endif
		}
	}

#ifdef RHODES_EMULATOR
    if (RHOSIMCONF().getString("debug_host").length() > 0)
        SetEnvironmentVariableA("RHOHOST", RHOSIMCONF().getString("debug_host").c_str() );
    if (RHOSIMCONF().getString("debug_port").length() > 0)
        SetEnvironmentVariableA("rho_debug_port", RHOSIMCONF().getString("debug_port").c_str() );
#endif

    //::SetThreadPriority(GetCurrentThread(),10);

	//Check for bundle directory is exists.
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	
	// rootpath + "rho/"
	if (m_strRootPath.at(m_strRootPath.length()-1) == '/') {
		hFind = FindFirstFile(convertToStringW(m_strRootPath.substr(0, m_strRootPath.find_last_of('/'))).c_str(), &wfd);
	} else if (m_strRootPath.at(m_strRootPath.length()-1) == '\\') {
		//delete all '\' from the end of the pathname
		int i = m_strRootPath.length();
		for ( ; i != 1; i--) {
			if (m_strRootPath.at(i-1) != '\\')
				break;
		}
		hFind = FindFirstFile(convertToStringW(m_strRootPath.substr(0, i)).c_str(), &wfd);
	}

	if (INVALID_HANDLE_VALUE == hFind || !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		int last = 0, pre_last = 0;
		last = getRhoRootPath().find_last_of('\\');
		pre_last = getRhoRootPath().substr(0, last).find_last_of('\\');
		String appName = getRhoRootPath().substr(pre_last + 1, last - pre_last - 1);

		String messageText = "Bundle directory \"" + 
								m_strRootPath.substr(0, m_strRootPath.find_last_of('/')) + 
								"\" is  missing\n";

		LOG(INFO) + messageText;
		int msgboxID = MessageBox(NULL,
									convertToStringW(messageText).c_str(),
									convertToStringW(appName).c_str(),
									MB_ICONERROR | MB_OK);


		return S_FALSE;
    }

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRuntimePath);
    RHODESAPP().setExtManager( &m_oExtManager );

#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    if ((!rho_wmimpl_get_is_version2()) && (rho_wmimpl_get_startpage()[0] != 0)) {
        String spath = convertToStringA(rho_wmimpl_get_startpage());
        RHOCONF().setString("start_path", spath, false);
    }
#endif // APP_BUILD_CAPABILITY_SHARED_RUNTIME

    DWORD dwStyle = WS_VISIBLE;

#if !defined(_WIN32_WCE)
    dwStyle |= WS_OVERLAPPEDWINDOW;
#endif
    // Create the main application window
#if defined(OS_WINDOWS_DESKTOP)
#ifdef RHODES_EMULATOR
    StringW windowTitle = convertToStringW(RHOSIMCONF().getString("app_name"));
#else
    StringW windowTitle = convertToStringW(RHODESAPP().getAppTitle());
#endif
    m_appWindow.Initialize(windowTitle.c_str());
    if (NULL == m_appWindow.m_hWnd)
    {
        return S_FALSE;
    }
    m_appWindow.ShowWindow(nShowCmd);

#ifndef RHODES_EMULATOR
    rho_wm_impl_CheckLicense();
#endif

#else
    String strTitle = RHODESAPP().getAppTitle();
    m_appWindow.Create(NULL, CWindow::rcDefault, convertToStringW(strTitle).c_str(), dwStyle);

	//if( m_isRhoConnectPush )
	//	m_appWindow.ShowWindow(SW_MINIMIZE);

    if (NULL == m_appWindow.m_hWnd)
    {
        return S_FALSE;
    }

/*#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    {
        CBarcodeInit oBarcodeInit;

        if (!rho_wmimpl_get_webkitbrowser( (HWND)m_appWindow.m_hWnd, rho_wmimpl_get_appinstance() )) {
            MessageBox(NULL, L"Failed to initialize WebKit engine.", convertToStringW(strTitle).c_str(), MB_ICONERROR | MB_OK);
	        return S_FALSE;
        }
    }
#endif
*/
    m_appWindow.InvalidateRect(NULL, TRUE);
    m_appWindow.UpdateWindow();

    m_appWindow.initBrowserWindow();
#endif

    bool bRE1App = false;
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    if (!rho_wmimpl_get_is_version2())
        bRE1App = true;
#endif

    if (bRE1App)
    {
#if defined(APP_BUILD_CAPABILITY_MOTOROLA)
        registerRhoExtension();
#endif
	    m_appWindow.Navigate2(_T("about:blank")
#if defined(OS_WINDOWS_DESKTOP)
            , -1
#endif
        );
        
        rho_webview_navigate( RHOCONF().getString("start_path").c_str(), 0 );
/*    	m_appWindow.Navigate2( convertToStringW( RHOCONF().getString("start_path") ).c_str()
#if defined(OS_WINDOWS_DESKTOP)
            , -1
#endif
        );*/
    }
    else
    {
        RHODESAPP().startApp();

        // Navigate to the "loading..." page
	    m_appWindow.Navigate2(_T("about:blank")
    #if defined(OS_WINDOWS_DESKTOP)
            , -1
    #endif
        );
    }
    // Show the main application window
    //m_appWindow.ShowWindow(nShowCmd);

#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )

    DWORD dwConnCount = 0;
    hr = RegistryGetDWORD( SN_CONNECTIONSNETWORKCOUNT_ROOT,
		SN_CONNECTIONSNETWORKCOUNT_PATH, 
		SN_CONNECTIONSNETWORKCOUNT_VALUE, 
        &dwConnCount
    );
    rho_sysimpl_sethas_network(dwConnCount);

    DWORD dwCellConnected = 0;
    hr = RegistryGetDWORD( SN_CONNECTIONSNETWORKCOUNT_ROOT,
		SN_CELLSYSTEMCONNECTED_PATH, 
		SN_CELLSYSTEMCONNECTED_VALUE, 
        &dwCellConnected
    );
    rho_sysimpl_sethas_cellnetwork(dwCellConnected);

	// Register for changes in the number of network connections
	hr = RegistryNotifyWindow(SN_CONNECTIONSNETWORKCOUNT_ROOT,
		SN_CONNECTIONSNETWORKCOUNT_PATH, 
		SN_CONNECTIONSNETWORKCOUNT_VALUE, 
		m_appWindow.m_hWnd, 
		WM_CONNECTIONSNETWORKCOUNT, 
		0, 
		NULL, 
		&g_hNotify);

	hr = RegistryNotifyWindow(SN_CONNECTIONSNETWORKCOUNT_ROOT,
		SN_CELLSYSTEMCONNECTED_PATH, 
		SN_CELLSYSTEMCONNECTED_VALUE, 
		m_appWindow.m_hWnd, 
		WM_CONNECTIONSNETWORKCELL, 
		0, 
		NULL, 
		&g_hNotifyCell);

#elif !defined( OS_PLATFORM_MOTCE )
    if (RHOCONF().getString("test_push_client").length() > 0 ) 
	    rho_clientregister_create(RHOCONF().getString("test_push_client").c_str());//"win32_client");
#endif

    return S_OK;
}

void CRhodesModule::RunMessageLoop( ) throw( )
{
#if defined(OS_WINDOWS_DESKTOP)
    m_appWindow.MessageLoop();
#else
    m_appWindow.getWebKitEngine()->RunMessageLoop(m_appWindow);
#endif

#if defined(OS_WINCE)&& !defined( OS_PLATFORM_MOTCE )
    if (g_hNotify)
        RegistryCloseNotification(g_hNotify);

    if ( g_hNotifyCell )
        RegistryCloseNotification(g_hNotifyCell);

    CGPSController* pGPS = CGPSController::Instance();
    pGPS->DeleteInstance();
#endif
    rho_ringtone_manager_stop();

//#if !defined(_WIN32_WCE)
//    rho::sync::CClientRegister::Destroy();
//#endif

#if defined(OS_WINDOWS_DESKTOP)
    m_appWindow.DestroyUi();
#endif

    rho::common::CRhodesApp::Destroy();

    net::CNetRequestImpl::deinitConnection();

#if !defined(OS_WINDOWS_DESKTOP)
//	ReleaseMutex(m_hMutex);
#endif
}

const rho::String& CRhodesModule::getRhoRootPath()
{
    if ( m_strRootPath.length() == 0 )
        m_strRootPath = getRhoRuntimePath();

    return m_strRootPath;
}

const rho::String& CRhodesModule::getAppName()
{
    if ( m_strAppName.length() == 0 )
    {
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
        bool bRE1App = false;
        if (!rho_wmimpl_get_is_version2())
            bRE1App = true;
        if ( bRE1App )
            m_strAppName = convertToStringA( rho_wmimpl_sharedconfig_getvalue( L"General\\Name" ) );
        else
        {
            String path = getRhoRootPath();
            String_replace(path, '/', '\\');

            int nEnd = path.find_last_of('\\');
            nEnd = path.find_last_of('\\', nEnd-1)-1;

            int nStart = path.find_last_of('\\', nEnd) +1;
            m_strAppName = path.substr( nStart, nEnd-nStart+1);
        }
#else
        m_strAppName = get_app_build_config_item("name");
#endif
    }

    return m_strAppName;
}

const rho::String& CRhodesModule::getRhoRuntimePath()
{
    if ( m_strRuntimePath.length() == 0 )
    {
        char rootpath[MAX_PATH];
        int len;
        if ( (len = GetModuleFileNameA(NULL,rootpath,MAX_PATH)) == 0 )
            strcpy(rootpath,".");
        else
        {
            while( !(rootpath[len] == '\\'  || rootpath[len] == '/') )
              len--;
            rootpath[len+1]=0;
        }

        m_strRuntimePath = rootpath;
        m_strRuntimePath += "rho\\";

        for(unsigned int i = 0; i < m_strRuntimePath.length(); i++ )
            if ( m_strRuntimePath.at(i) == '\\' )
                m_strRuntimePath[i] = '/';
    }

    return m_strRuntimePath; 
}

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
                                LPTSTR lpCmdLine, int nShowCmd)
{
	INITCOMMONCONTROLSEX ctrl;


	//Required to use datetime picker controls.
	ctrl.dwSize = sizeof(ctrl);
	ctrl.dwICC = ICC_DATE_CLASSES|ICC_BAR_CLASSES;
	InitCommonControlsEx(&ctrl);

    g_strCmdLine = convertToStringA(lpCmdLine);
    _AtlModule.SetModuleInstance(hInstance);

	return _AtlModule.WinMain(nShowCmd);
}

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask) 
{
    CMainWindow* mainWnd = _AtlModule.GetMainWindowObject();
    mainWnd->performOnUiThread(pTask);    
}

extern "C" HWND getMainWnd() {
	return _AtlModule.GetMainWindow();
}

CMainWindow& getAppWindow() 
{
	return _AtlModule.GetAppWindow();
}

extern "C" HWND getWebViewWnd(int index) {
	return _AtlModule.GetWebViewWindow(index);
}

CMainWindow* Rhodes_getMainWindow() {
	return _AtlModule.GetMainWindowObject();
}

extern "C" void rho_wmsys_run_app(const char* szPath, const char* szParams );
void rho_platform_restart_application() 
{
	char module[MAX_PATH];
    ::GetModuleFileNameA(NULL,module,MAX_PATH);
                                       
    rho_wmsys_run_app(module, (g_strCmdLine + " -restarting").c_str());
}

typedef bool (WINAPI *PCSD)();

#ifdef APP_BUILD_CAPABILITY_MOTOROLA
extern "C" void rho_wm_impl_CheckLicenseWithBarcode(HWND hParent, HINSTANCE hLicenseInstance);
#endif
extern "C" void rho_wm_impl_SetApplicationLicenseObj(void* pAppLicenseObj);

typedef LPCWSTR (WINAPI *PCL)(HWND, LPCWSTR, LPCWSTR, LPCWSTR);
typedef int (WINAPI *FUNC_IsLicenseOK)();
typedef void* (WINAPI *FUNC_GetAppLicenseObj)();

extern "C" void rho_wm_impl_CheckLicense()
{
    int nRes = 0;
    HINSTANCE hLicenseInstance = LoadLibrary(L"license_rc.dll");
    if(hLicenseInstance)
    {
#ifdef OS_WINDOWS_DESKTOP
        PCL pCheckLicense = (PCL) ::GetProcAddress(hLicenseInstance, "_CheckLicense@16");
        FUNC_IsLicenseOK pIsOK = (FUNC_IsLicenseOK) ::GetProcAddress(hLicenseInstance, "_IsLicenseOK@0");
#else
        PCL pCheckLicense = (PCL) GetProcAddress(hLicenseInstance, L"CheckLicense");
        FUNC_IsLicenseOK pIsOK = (FUNC_IsLicenseOK) GetProcAddress(hLicenseInstance, L"IsLicenseOK");
#endif
        LPCWSTR szLogText = 0;
        if(pCheckLicense)
        {
            StringW strLicenseW;
            common::convertToStringW( get_app_build_config_item("motorola_license"), strLicenseW );

            StringW strCompanyW;
            common::convertToStringW( get_app_build_config_item("motorola_license_company"), strCompanyW );

            bool bRE1App = false;
        #if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
            if (!rho_wmimpl_get_is_version2())
                bRE1App = true;

            if ( bRE1App )
            {
                LPCTSTR szLicense = rho_wmimpl_sharedconfig_getvalue( L"LicenseKey" );
                if ( szLicense )
                    strLicenseW = szLicense;

                LPCTSTR szLicenseComp = rho_wmimpl_sharedconfig_getvalue( L"LicenseKeyCompany" );
                if ( szLicenseComp )
                    strCompanyW = szLicenseComp;
            }
        #endif

            StringW strAppNameW;
//#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
            strAppNameW = RHODESAPP().getAppNameW();
//#else
//            common::convertToStringW( get_app_build_config_item("name"), strAppNameW );
//#endif
            szLogText = pCheckLicense( getMainWnd(), strAppNameW.c_str(), strLicenseW.c_str(), strCompanyW.c_str() );
        }

        if ( szLogText && *szLogText )
            LOGC(INFO, "License") + szLogText;

        nRes = pIsOK ? pIsOK() : 0;
    }

#ifdef APP_BUILD_CAPABILITY_MOTOROLA
    if ( nRes == 0 )
    {
        rho_wm_impl_CheckLicenseWithBarcode(getMainWnd(),hLicenseInstance);
        return;
    }
#endif

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    if ( nRes )
    {
        FUNC_GetAppLicenseObj pGetAppLicenseObj = (FUNC_GetAppLicenseObj) GetProcAddress(hLicenseInstance, L"GetAppLicenseObj");
        if ( pGetAppLicenseObj )
            rho_wm_impl_SetApplicationLicenseObj( pGetAppLicenseObj() );
    }
#endif

    if ( !nRes )
        ::PostMessage( getMainWnd(), WM_SHOW_LICENSE_WARNING, 0, 0);
}

static inline char *
translate_char(char *p, int from, int to)
{
    while (*p) {
	if ((unsigned char)*p == from)
	    *p = to;
	p = CharNextA(p);
    }
    return p;
}

extern "C" const char* rho_native_get_appname()
{
    return _AtlModule.getAppName().c_str();
}

extern "C" const char* rho_native_rhopath() 
{
    return _AtlModule.getRhoRootPath().c_str();
}

extern "C" const char* rho_native_reruntimepath() 
{
    return _AtlModule.getRhoRuntimePath().c_str();
}

extern "C" HINSTANCE rho_wmimpl_get_appinstance()
{
    return _AtlModule.GetModuleInstance();
}

extern "C" HWND rho_wmimpl_get_mainwnd() 
{
	return _AtlModule.GetMainWindow();
}

extern "C" void rho_conf_show_log()
{
    ::PostMessage(getMainWnd(),WM_COMMAND,IDM_LOG,0);
}

//Hook for ruby call to refresh web view

extern "C" void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
}
/*
extern "C" void mapview_create(rho_param *p) {
    //TODO: mapview_create
}

extern "C" void mapview_close() {
    //TODO: mapview_close
}

extern "C" VALUE mapview_state_started() {
    //TODO: mapview_state_started
    return 0;
}

extern "C" double mapview_state_center_lat() {
    //TODO:
    return 0;
}

extern "C" double mapview_state_center_lon() {
    //TODO:
    return 0;
}
*/
extern "C" void rho_map_location(char* query)
{
}

extern "C" void rho_appmanager_load( void* httpContext, char* szQuery)
{
}

//extern "C" void Init_openssl(void)
//{
//}

//extern "C" void Init_digest(void)
//{
//}

#if !defined(OS_WINDOWS_DESKTOP)
extern "C" void Init_fcntl(void)
{
}
#endif

//extern "C" void Init_RhoEvent()
//{
//}

//extern "C" void Init_Calendar()
//{
//}

/*BOOL EnumRhodesWindowsProc(HWND hwnd,LPARAM lParam)
{
	char buf[255] = {0};
	static char current_path[255] = {0};
	if ( strlen(current_path) == 0 )
		GetWindowModuleFileName(getMainWnd(), current_path, 255);
	
	GetWindowModuleFileName(hwnd, buf, 255);

	if ( strncmp( buf, current_path, 255 ) == 0 )
	{
		HWND* pWnd = (HWND*)lParam;
		*pWnd = hwnd;
		return FALSE;
	}

	return TRUE;
}*/


//parseToken will allocate extra byte at the end of the 
//returned token value
LPTSTR parseToken (LPCTSTR start, LPCTSTR* next_token) {
    int nNameLen = 0;
    while(*start==' '){ start++; }

    int i = 0;
    while( start[i] != L'\0' )
	{
        if (( start[i] == L'=' ) || ( start[i] == L':' ) || ( start[i] == L' ' )) {
            if ( i > 0 ){
                int s = i-1;
                for(; s >= 0 && start[s]==' '; s-- );
                nNameLen = s+1;
                break;
            }else 
                break;
        }
		++i;
    }

	if ( nNameLen == 0 )
        return NULL;

    LPCTSTR szValue = start + i+1;
    int nValueLen = 0;

	// skip leading spaces and optional delimiter (that is either ' or ") and keep the delimiter for further use
	TCHAR delimiter = L' ';
	while ((*szValue==delimiter) || ((delimiter==L' ') && ((*szValue==L'\'') || (*szValue==L'"')))) {
		if ((delimiter==L' ') && ((*szValue==L'\'') || (*szValue==L'"'))) {
			// delimiter found
			delimiter = *szValue;
			szValue++;
			break;
		}
		szValue++;
	}
	// search for value end -- that is either end of line or delimiter
	while ((szValue[nValueLen] != L'\0') && (szValue[nValueLen] != delimiter)) { nValueLen++; }
	// next token begins with the character next to delimiter (if it's not the end of the line)
	if (*next_token == NULL)
		*next_token = szValue + nValueLen + (szValue[nValueLen] != L'\0' ? 1 : 0 );

	TCHAR* value = (TCHAR*) malloc((nValueLen+2)*sizeof(TCHAR));
	wcsncpy(value, szValue, nValueLen);
	value[nValueLen] = L'\0';

	return value;
}

#if defined(OS_WINDOWS_DESKTOP)
/*
// char -> wchar_t 
wchar_t* wce_mbtowc(const char* a)
{
	int length;
	wchar_t *wbuf;

	length = MultiByteToWideChar(CP_UTF8, 0, 
		a, -1, NULL, 0);
	wbuf = (wchar_t*)malloc( (length+1)*sizeof(wchar_t) );
	MultiByteToWideChar(CP_UTF8, 0,
		a, -1, wbuf, length);

	return wbuf;
}

// wchar_t -> char
char* wce_wctomb(const wchar_t* w)
{
	DWORD charlength;
	char* pChar;

	charlength = WideCharToMultiByte(CP_UTF8, 0, w,
					-1, NULL, 0, NULL, NULL);
	pChar = (char*)malloc(charlength+1);
	WideCharToMultiByte(CP_UTF8, 0, w,
		-1, pChar, charlength, NULL, NULL);

	return pChar;
}*/

#endif

#if defined( OS_PLATFORM_MOTCE )
#include <Imaging.h>
HBITMAP SHLoadImageFile(  LPCTSTR pszFileName )
{
    if ( !pszFileName || !*pszFileName )
        return 0;

    String strFileName = convertToStringA(pszFileName);
    /*if ( String_endsWith(strFileName, ".bmp") )
    {
        return (HBITMAP)::LoadImage(NULL, pszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }*/

    if ( !String_endsWith(strFileName, ".png") && !String_endsWith(strFileName, ".bmp") )
        return 0;

	IImagingFactory *pImgFactory = NULL;
    IImage *pImage = NULL;
    //CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HBITMAP hResult = 0;
    if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
                                    NULL,
                                    CLSCTX_INPROC_SERVER,
                                    IID_IImagingFactory,
                                    (void **)&pImgFactory)))
    {
		ImageInfo imageInfo;
		if (SUCCEEDED(pImgFactory->CreateImageFromFile(CA2W(strFileName.c_str()), &pImage))
			&& SUCCEEDED(pImage->GetImageInfo(&imageInfo)))
        {
			CWindowDC dc(getMainWnd());
			CDC dcBitmap;
			dcBitmap.CreateCompatibleDC(dc.m_hDC);
			hResult = CreateCompatibleBitmap(dc.m_hDC, imageInfo.Width, imageInfo.Height);
			if (hResult) 
			{
				HBITMAP hOldBitmap = dcBitmap.SelectBitmap(hResult);
                //dcBitmap.FillSolidRect( 0,0, imageInfo.Width, imageInfo.Height, RGB(255,255,255));

                CRect rc(0, 0, imageInfo.Width, imageInfo.Height);
	            COLORREF clrOld = ::SetBkColor(dcBitmap.m_hDC, RGB(255,255,255));
	            ::ExtTextOut(dcBitmap.m_hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
	            ::SetBkColor(dcBitmap.m_hDC, clrOld);

				pImage->Draw(dcBitmap.m_hDC, rc, NULL);
				dcBitmap.SelectBitmap(hOldBitmap);
			}
			pImage->Release();
       }
       pImgFactory->Release();
    }
    //CoUninitialize();

	return hResult;
}

#endif

#if !defined(_WIN32_WCE)
#include <gdiplus.h>
#include <Gdiplusinit.h>
using namespace Gdiplus;

#define   SelectBitmap(hdc, hbm)  ((HBITMAP)SelectObject((hdc), (HGDIOBJ)(HBITMAP)(hbm)))
HBITMAP SHLoadImageFile(  LPCTSTR pszFileName )
{
    if ( !pszFileName || !*pszFileName )
        return 0;

    String strFileName = convertToStringA(pszFileName);
    if ( String_endsWith(strFileName, ".bmp") )
    {
        return (HBITMAP)::LoadImage(NULL, pszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }

    if ( !String_endsWith(strFileName, ".png") )
        return 0;

    static bool s_GDIInit = false;
    if ( !s_GDIInit)
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        s_GDIInit = true;
    }

    Gdiplus::Image* image = new Gdiplus::Image(convertToStringW(strFileName).c_str());
    SizeF sizePng;
    Status res = image->GetPhysicalDimension(&sizePng);

    HDC hDC = GetDC(getMainWnd());

    HDC hdcMem = CreateCompatibleDC(hDC);
    HBITMAP hBitmap  = ::CreateCompatibleBitmap(hDC, (int)sizePng.Width, (int)sizePng.Height);
    HBITMAP hbmOld = SelectBitmap(hdcMem, hBitmap);

    CRect rc(0,0,(int)sizePng.Width, (int)sizePng.Height);
	COLORREF clrOld = ::SetBkColor(hdcMem, RGB(255,255,255));
	::ExtTextOut(hdcMem, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
	::SetBkColor(hdcMem, clrOld);

    Gdiplus::Graphics grpx(hdcMem);
    res = grpx.DrawImage(image, 0, 0, (int)sizePng.Width, (int)sizePng.Height);

    SelectBitmap(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    DeleteDC(hDC);

    delete image;
    return hBitmap;
}

#endif

extern "C" void rho_sys_unset_http_proxy()
{
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
	_AtlModule.GetAppWindow().setProxy();
#endif
	RHOCONF().removeProperty("http_proxy_host", false);
	RHOCONF().removeProperty("http_proxy_port", false);
	RHOCONF().removeProperty("http_proxy_login", false);
	RHOCONF().removeProperty("http_proxy_password", false);
	RAWLOG_INFO("Unsetting HTTP proxy");
}

void parseHttpProxyURI(const rho::String &http_proxy)
{
	// http://<login>:<passwod>@<host>:<port>
	const char *default_port = "8080";

	if (http_proxy.length() == 0)
		rho_sys_unset_http_proxy();

	if (http_proxy.length() < 8) {
		RAWLOG_ERROR("invalid http proxy url");
		return;
	}

	int index = http_proxy.find("http://", 0, 7);
	if (index == string::npos) {
		RAWLOG_ERROR("http proxy url should starts with \"http://\"");
		return;
	}
	index = 7;

	enum {
		ST_START,
		ST_LOGIN,
		ST_PASSWORD,
		ST_HOST,
		ST_PORT,
		ST_FINISH
	};

	String token, login, password, host, port;
	char c, state = ST_START, prev_state = state;
	int length = http_proxy.length();

	for (int i = index; i < length; i++) {
		c = http_proxy[i];

		switch (state) {
		case ST_START:
			if (c == '@') {
				prev_state = state; state = ST_HOST;
			} else if (c == ':') {
				prev_state = state; state = ST_PASSWORD;
			} else {
				token +=c;
				state = ST_HOST;
			}
			break;
		case ST_HOST:
			if (c == ':') {
				host = token; token.clear();			
				prev_state = state; state = ST_PORT;
			} else if (c == '@') {
				host = token; token.clear();		
				prev_state = state;	state = ST_LOGIN;					
			} else {
				token += c;
				if (i == (length - 1)) {
					host = token; token.clear();								
				}
			}
			break;
		case ST_PORT:
			if (c == '@') {
				port = token; token.clear();			
				prev_state = state; state = ST_LOGIN;
			} else {
				token += c;
				if (i == (length - 1)) {
					port = token; token.clear();
				}
			}
			break;
		case ST_LOGIN:
			if (prev_state == ST_PORT || prev_state == ST_HOST) {
				login    = host; host.clear();
				password = port; port.clear();
				prev_state = state; state = ST_HOST;
				token += c;
			} else {
				token += c;
				if (i == (length - 1)) {
					login = token; token.clear();								
				}
			}
			break;
		case ST_PASSWORD:
			if (c == '@') {
				password = token; token.clear();			
				prev_state = state; state = ST_HOST;
			} else {
				token += c;
				if (i == (length - 1)) {
					password = token; token.clear();								
				}
			}
			break;
		default:
			;
		}
	}

	RAWLOG_INFO("Setting up HTTP proxy:");
	RAWLOG_INFO1("URI: %s", http_proxy.c_str());
	RAWLOG_INFO1("HTTP proxy login    = %s", login.c_str());
	RAWLOG_INFO1("HTTP proxy password = %s", password.c_str());
	RAWLOG_INFO1("HTTP proxy host     = %s", host.c_str());
	RAWLOG_INFO1("HTTP proxy port     = %s", port.c_str());

	if (host.length()) {
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
		_AtlModule.GetAppWindow().setProxy(host, port, login, password);
#endif
		RHOCONF().setString ("http_proxy_host", host, false);

		if (port.length()){
			RHOCONF().setString ("http_proxy_port", port, false);
		} else {
			RAWLOG_WARNING("there is no proxy port defined");
		}

		if (login.length())
			RHOCONF().setString ("http_proxy_login", login, false);

		if (password.length())
			RHOCONF().setString ("http_proxy_password", password, false);

	} else {
		RAWLOG_ERROR("empty host name in HTTP-proxy URL");
	}
}

extern "C" void rho_sys_set_http_proxy_url(const char* url)
{
	String m_strHttpProxy = url;
    parseHttpProxyURI(m_strHttpProxy);
}