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

int winversion = 0;

extern UINT WM_CREATE_SHORTCUT;

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

#if !defined( APP_BUILD_CAPABILITY_SYMBOL ) && !defined( APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
extern "C" bool rho_wmimpl_get_resize_on_sip()
{
    return true;
}
#endif

#if defined(_WIN32_WCE)
#include <regext.h>
#include "soundfile.h"
#include "bluetooth/Bluetooth.h"

// Global Notification Handle
HREGNOTIFY g_hNotify = NULL, g_hNotifyCell = NULL;

// ConnectionsNetworkCount
// Gets a value indicating the number of network connections that are currently connected.
#define SN_CONNECTIONSNETWORKCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSNETWORKCOUNT_PATH TEXT("System\\State\\Hardware")
#define SN_CONNECTIONSNETWORKCOUNT_VALUE TEXT("WiFi")

#define SN_CELLSYSTEMCONNECTED_PATH TEXT("System\\State\\Phone")
#define SN_CELLSYSTEMCONNECTED_VALUE TEXT("Cellular System Connected")

typedef HRESULT (WINAPI* LPFN_REGISTRY_CLOSENOTIFICATION_T) (HREGNOTIFY);
typedef HRESULT (WINAPI* LPFN_REGISTRY_NOTIFYWINDOW_T) (HKEY, LPCTSTR, LPCTSTR, HWND, UINT, DWORD, NOTIFICATIONCONDITION*, HREGNOTIFY*);
typedef HRESULT (WINAPI* LPFN_REGISTRY_GETSTRING_T) (HKEY, LPCTSTR, LPCTSTR, LPTSTR, UINT);
typedef HRESULT (WINAPI* LPFN_REGISTRY_GETDWORD_T) (HKEY, LPCTSTR, LPCTSTR, DWORD*);
typedef HRESULT (WINAPI* LPFN_CAMERA_CAPTURE_T)(PSHCAMERACAPTURE);
typedef BOOL (WINAPI* LPFN_GETOPEN_FILEEX_T)(LPOPENFILENAMEEX);
typedef HRESULT (WINAPI* LPFN_SND_STOP_T)(__in SND_SCOPE, __reserved HSOUND);
typedef HRESULT (WINAPI* LPFN_SND_CLOSE_T)(__in HSOUND);
typedef HRESULT (WINAPI* LPFN_SND_PLAYASYNC_T)(__in HSOUND, DWORD);
typedef HRESULT (WINAPI* LPFN_SND_OPEN_T)(__in LPCTSTR,  __out HSOUND*);
typedef HRESULT (WINAPI* LPFN_SND_GETSOUNDFILELIST_T)(SND_EVENT, DWORD, SNDFILEINFO**, int*);


extern "C" LPFN_SND_STOP_T lpfn_snd_stop = NULL;
extern "C" LPFN_SND_CLOSE_T lpfn_snd_close = NULL;
extern "C" LPFN_SND_PLAYASYNC_T lpfn_snd_playasync = NULL;
extern "C" LPFN_SND_OPEN_T lpfn_snd_open = NULL;
extern "C" LPFN_SND_GETSOUNDFILELIST_T lpfn_snd_getsndflist = NULL;
LPFN_REGISTRY_CLOSENOTIFICATION_T	lpfn_Registry_CloseNotification = NULL;
LPFN_REGISTRY_NOTIFYWINDOW_T		lpfn_Registry_NotifyWindow = NULL;		
LPFN_REGISTRY_GETSTRING_T			lpfn_Registry_GetString = NULL;			
LPFN_REGISTRY_GETDWORD_T			lpfn_Registry_GetDWORD = NULL;
extern "C" LPFN_CAMERA_CAPTURE_T               lpfn_Camera_Capture = NULL;
extern "C" LPFN_GETOPEN_FILEEX_T               lpfn_GetOpen_FileEx = NULL;
HMODULE g_hAygShellDLL = NULL;	
extern "C" BOOL LoadAYGShell();
HMODULE g_hSndDLL = NULL;	
extern "C" BOOL LoadSoundDll();

#endif

//This is hack. MC4900 device failed to enable barcode after webkit initialization. So we enable it before.
#if defined(APP_BUILD_CAPABILITY_BARCODE) && defined(APP_BUILD_CAPABILITY_SYMBOL) && defined (OS_PLATFORM_MOTCE)
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
    rho::String m_strRootPath, m_strRhodesPath, m_logPort, m_strRuntimePath, m_strAppName;
	bool m_bRestarting;
    bool m_bMinimized;
	bool m_isRhoConnectPush;
    bool m_startAtBoot, m_bJSApplication;
    String m_strTabName;

	HANDLE m_hMutex;

public :
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

void parseHttpProxyURI(const rho::String &http_proxy);

static String g_strCmdLine;
HINSTANCE CRhodesModule::m_hInstance;
CRhodesModule _AtlModule;
bool g_restartOnExit = false;

bool CRhodesModule::ParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode ) throw()
{
	m_bRestarting      = false;
    m_bMinimized       = false;
    m_startAtBoot      = false;
#ifdef RHO_NO_RUBY
    m_bJSApplication   = true;        
#else
    m_bJSApplication   = false;
#endif

    m_logPort          = "";
    m_isRhoConnectPush = false;
    LPCTSTR lpszToken  = lpCmdLine;
	LPCTSTR nextToken;

    getRhoRootPath();

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
				m_bRestarting = true;
			}

            if (wcsncmp(lpszToken, _T("minimized"), 9)==0) {
				m_bMinimized = true;
			}

            if (wcsncmp(lpszToken, _T("tabname"), 7)==0) {
				m_strTabName = convertToStringA(value);
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
            else if (wcsnicmp(lpszToken, _T("approot"),7)==0 || wcsnicmp(lpszToken, _T("jsapproot"),9)==0)
			{
				if (value) 
                {
					m_strRootPath = convertToStringA(value);
					if (m_strRootPath.substr(0,7).compare("file://")==0)
						m_strRootPath.erase(0,7);
					String_replace(m_strRootPath, '\\', '/');
					if (m_strRootPath.at(m_strRootPath.length()-1)!='/')
						m_strRootPath.append("/");

#if defined(OS_WINCE)
					m_strRootPath.append("rho/");
#ifdef APP_BUILD_CAPABILITY_SHARED_RUNTIME
					rho_wmimpl_set_is_version2(m_strRootPath.c_str());
#endif
#endif
        		}

                if ( wcsnicmp(lpszToken, _T("jsapproot"),9)==0 )
                    m_bJSApplication = true;
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
    HRESULT hr;
	/*HRESULT hr = __super::PreMessageLoop(nShowCmd);
    if (FAILED(hr))
    {
        return hr;
    }*/
    // Note: In this sample, we don't respond differently to different hr success codes.

    //SetLastError(0);
//    HANDLE hEvent = CreateEvent( NULL, false, false, CMainWindow::GetWndClassInfo().m_wc.lpszClassName );

  //  if ( !m_bRestarting && hEvent != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
    //{
        // Rho Running so could bring to foreground
      //  HWND hWnd = FindWindow(CMainWindow::GetWndClassInfo().m_wc.lpszClassName, NULL);

        //if (hWnd)
        //{
          //  ShowWindow(hWnd, SW_SHOW);
            //SendMessage( hWnd, PB_WINDOW_RESTORE, NULL, TRUE);
            //SetForegroundWindow( hWnd );

            //COPYDATASTRUCT cds = {0};
            //cds.cbData = m_strTabName.length()+1;
            //cds.lpData = (char*)m_strTabName.c_str();
            //SendMessage( hWnd, WM_COPYDATA, (WPARAM)WM_WINDOW_SWITCHTAB, (LPARAM)(LPVOID)&cds);
        //}

        //return S_FALSE;
    //}

    if ( !rho_sys_check_rollback_bundle(rho_native_rhopath()) )
    {
        rho_sys_impl_exit_with_errormessage( "Bundle update", "Application is corrupted. Reinstall it, please.");
        return S_FALSE;
    }

#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    rho_logconf_Init((rho_wmimpl_get_logpath()[0]==0 ? m_strRootPath.c_str() : rho_wmimpl_get_logpath()), m_strRootPath.c_str(), m_logPort.c_str());
    if (rho_wmimpl_get_logurl()[0]!=0)
	{
        LOGCONF().setLogURL(rho_wmimpl_get_logurl());
		LOGCONF().setLogToSocket(true);
	}
    if (rho_wmimpl_get_logmaxsize())
        LOGCONF().setMaxLogFileSize(*rho_wmimpl_get_logmaxsize());
    if (rho_wmimpl_get_loglevel())
        LOGCONF().setMinSeverity(*rho_wmimpl_get_loglevel());
    if (rho_wmimpl_get_fullscreen())
        RHOCONF().setBool("full_screen", true, false);
    if (rho_wmimpl_get_logmemperiod())
        LOGCONF().setCollectMemoryInfoInterval(*rho_wmimpl_get_logmemperiod());
    //SR EMBPD00121468
    //Network api via proxy is not working with proxy enable using config.xml 
    //Sabir: The values were getting set from rhoconfig.txt from function rho_logconf_Init
    //fix: we have to override http_proxy_host using config.xml value
    rho::String szHttpProxy =  rho_wmimpl_get_httpproxy();
    parseHttpProxyURI(szHttpProxy);
#else
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());
#endif // APP_BUILD_CAPABILITY_SHARED_RUNTIME

 SetLastError(0);
 HANDLE hEvent = CreateEvent( NULL, false, false, CMainWindow::GetWndClassInfo().m_wc.lpszClassName );

    if ( !m_bRestarting && hEvent != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        // Rho Running so could bring to foreground
        HWND hWnd = FindWindow(CMainWindow::GetWndClassInfo().m_wc.lpszClassName, NULL);

        if (hWnd)
        {
            ShowWindow(hWnd, SW_SHOW);
            SendMessage( hWnd, PB_WINDOW_RESTORE, NULL, TRUE);
            SetForegroundWindow( hWnd );

            COPYDATASTRUCT cds = {0};
            cds.cbData = m_strTabName.length()+1;
            cds.lpData = (char*)m_strTabName.c_str();
            SendMessage( hWnd, WM_COPYDATA, (WPARAM)WM_WINDOW_SWITCHTAB, (LPARAM)(LPVOID)&cds);
        }

        return S_FALSE;
    }



    LOGCONF().setMemoryInfoCollector(CLogMemory::getInstance());

    if ( !rho_rhodesapp_canstartapp(g_strCmdLine.c_str(), " /-,") )
    {
		LOG(INFO) + "This is hidden app and can be started only with security key.";
		if (RHOCONF().getString("invalid_security_token_start_path").length() <= 0)
        {
			return S_FALSE;
        }
    }

	LOG(INFO) + "Rhodes started";
	if (RHOCONF().isExist("http_proxy_url")) {
		parseHttpProxyURI(RHOCONF().getString("http_proxy_url"));
	} else if (RHOCONF().isExist("http_proxy_uri")) {
		parseHttpProxyURI(RHOCONF().getString("http_proxy_uri"));
	}


	//Check for bundle directory is exists.
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	
	// rootpath + "rho/"
	if (m_strRootPath.at(m_strRootPath.length()-1) == '/') 
    {
		hFind = FindFirstFile(convertToStringW(m_strRootPath.substr(0, m_strRootPath.find_last_of('/'))).c_str(), &wfd);
	} 
    else if (m_strRootPath.at(m_strRootPath.length()-1) == '\\') 
    {
		//delete all '\' from the end of the pathname
		int i = m_strRootPath.length();
		for ( ; i != 1; i--) {
			if (m_strRootPath.at(i-1) != '\\')
				break;
		}

		hFind = FindFirstFile(convertToStringW(m_strRootPath.substr(0, i)).c_str(), &wfd);
	}

	if (INVALID_HANDLE_VALUE == hFind || !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
    {
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

    createPowerManagementThread();

    if (RHOCONF().getBool("Application.autoStart"))
        createAutoStartShortcut();

#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    if ((!rho_wmimpl_get_is_version2()) && (rho_wmimpl_get_startpage()[0] != 0)) {
        String spath = convertToStringA(rho_wmimpl_get_startpage());
        RHOCONF().setString("start_path", spath, false);
    }
#endif // APP_BUILD_CAPABILITY_SHARED_RUNTIME

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRuntimePath);

    set_bridge_direct_callback();

    bool bRE1App = false;

#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    if (!rho_wmimpl_get_is_version2())
        bRE1App = true;
#endif

    RHODESAPP().setJSApplication(bRE1App || _AtlModule.isJSApplication());

    DWORD dwStyle = m_bMinimized ? 0 : WS_VISIBLE;

#ifdef OS_WINCE
    m_appWindow.getTabbar().SetStartTabName(m_strTabName);
#else
    m_appWindow.setStartTabName(m_strTabName);
#endif

#if !defined(_WIN32_WCE)
    dwStyle |= WS_OVERLAPPEDWINDOW;
#endif
    // Create the main application window
    String strTitle = RHODESAPP().getAppTitle();
    m_appWindow.Create(NULL, CWindow::rcDefault, convertToStringW(strTitle).c_str(), dwStyle);

    if (NULL == m_appWindow.m_hWnd)
    {
        return S_FALSE;
    }

    m_appWindow.InvalidateRect(NULL, TRUE);
	m_appWindow.SetActiveWindow();	//  Enterprise Browser was failing to launch maximimized through a shortcut, this line ensures it launches maximised
    m_appWindow.UpdateWindow();
#if defined(APP_BUILD_CAPABILITY_SYMBOL)
	initialiseRhoElementsExt();
#endif
    m_appWindow.initBrowserWindow();

    if (m_bMinimized)
        m_appWindow.ShowWindow(SW_MINIMIZE);

/*
    if (bRE1App)
    {
#if defined(APP_BUILD_CAPABILITY_SYMBOL)
        registerRhoExtension();
#endif

#if !defined( APP_BUILD_CAPABILITY_WEBKIT_BROWSER ) && defined(OS_WINCE)
	    m_appWindow.Navigate2(_T("about:blank"), -1 );
#endif //!APP_BUILD_CAPABILITY_WEBKIT_BROWSER

        rho_webview_navigate(RHOCONF().getString("start_path").c_str(), 0 );
    }
    else
    { */
        RHODESAPP().startApp();

#if !defined( APP_BUILD_CAPABILITY_WEBKIT_BROWSER ) && defined(OS_WINCE)
        // Navigate to the "loading..." page
	    m_appWindow.Navigate2(_T("about:blank"), -1 );
#endif //APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    //}

#if defined(_WIN32_WCE)
    if(RHO_IS_WMDEVICE)
    {

        DWORD dwConnCount = 0;
        hr = lpfn_Registry_GetDWORD( SN_CONNECTIONSNETWORKCOUNT_ROOT,
		    SN_CONNECTIONSNETWORKCOUNT_PATH, 
		    SN_CONNECTIONSNETWORKCOUNT_VALUE, 
            &dwConnCount
        );
        rho_sysimpl_sethas_network((dwConnCount > 1) ? 1 : 0);

        DWORD dwCellConnected = 0;
        hr = lpfn_Registry_GetDWORD( SN_CONNECTIONSNETWORKCOUNT_ROOT,
		    SN_CELLSYSTEMCONNECTED_PATH, 
		    SN_CELLSYSTEMCONNECTED_VALUE, 
            &dwCellConnected
        );
        rho_sysimpl_sethas_cellnetwork(dwCellConnected);

	    // Register for changes in the number of network connections
	    hr = lpfn_Registry_NotifyWindow(SN_CONNECTIONSNETWORKCOUNT_ROOT,
		    SN_CONNECTIONSNETWORKCOUNT_PATH, 
		    SN_CONNECTIONSNETWORKCOUNT_VALUE, 
		    m_appWindow.m_hWnd, 
		    WM_CONNECTIONSNETWORKCOUNT, 
		    0, 
		    NULL, 
		    &g_hNotify);

	    hr = lpfn_Registry_NotifyWindow(SN_CONNECTIONSNETWORKCOUNT_ROOT,
		    SN_CELLSYSTEMCONNECTED_PATH, 
		    SN_CELLSYSTEMCONNECTED_VALUE, 
		    m_appWindow.m_hWnd, 
		    WM_CONNECTIONSNETWORKCELL, 
		    0, 
		    NULL, 
		    &g_hNotifyCell);
    }
#endif
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
	if(rho::BrowserFactory::getCurrentBrowserType() == eIE){
		if(RHO_IS_WMDEVICE)
		{
			Sleep(500);
		}
		else
		{
			Sleep(50);
		}
	}
#endif
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
	HWND hGetMainWnd = GetMainWindow();
	if(rho::BrowserFactory::getCurrentBrowserType() != eIE)
	{
		PostMessage( hGetMainWnd, WM_CREATE_SHORTCUT, NULL, NULL);
	}
	else
	{
		SendMessage( hGetMainWnd, WM_CREATE_SHORTCUT, NULL, NULL);
	}
#endif
    return S_OK;
}

void CRhodesModule::RunMessageLoop( ) throw( )
{
    m_appWindow.getWebKitEngine()->RunMessageLoop(m_appWindow);

#if defined(OS_WINCE)
	if(RHO_IS_WMDEVICE)
	{
		if (g_hNotify)
		  lpfn_Registry_CloseNotification(g_hNotify);

	 if ( g_hNotifyCell )
		 lpfn_Registry_CloseNotification(g_hNotifyCell);

		CGPSController* pGPS = CGPSController::Instance();
		pGPS->DeleteInstance();
	}
#endif
    rho_ringtone_manager_stop();

    rho::common::CRhodesApp::Destroy();

//	ReleaseMutex(m_hMutex);

    rho_platform_check_restart_application();
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
        m_strAppName = RHOCONF().getString("app_name");
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

void CRhodesModule::createAutoStartShortcut()
{
#ifdef OS_WINCE
    StringW strLnk = L"\\Windows\\StartUp\\";
    strLnk += convertToStringW(getAppName());
    strLnk += L".lnk";

    StringW strAppPath = L"\"";
    char rootpath[MAX_PATH];
    GetModuleFileNameA(NULL,rootpath,MAX_PATH);
    strAppPath += convertToStringW(rootpath);
    strAppPath += L"\" -minimized";

    DWORD dwRes = SHCreateShortcut( (LPTSTR)strLnk.c_str(), (LPTSTR)strAppPath.c_str());
#endif

}

HRESULT CRhodesModule::PostMessageLoop() throw()
{
	return 0;
}

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
                                LPTSTR lpCmdLine, int nShowCmd)
{
	INITCOMMONCONTROLSEX ctrl;

#if defined(OS_WINCE)
	/*if(RhoBluetoothManager::LoadBthUtil())
		winversion = 1;
	else
		winversion = 2;*/
	LoadAYGShell();
	LoadSoundDll();
#endif

	//Required to use datetime picker controls.
	ctrl.dwSize = sizeof(ctrl);
	ctrl.dwICC = ICC_DATE_CLASSES|ICC_BAR_CLASSES;
	InitCommonControlsEx(&ctrl);

    g_strCmdLine = convertToStringA(lpCmdLine);
    _AtlModule.SetModuleInstance(hInstance);

	return _AtlModule.WinMain(nShowCmd);
}

extern "C" void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask) 
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
static bool g_bIsRestartApplication = false;
void rho_platform_restart_application() 
{
    g_bIsRestartApplication = true;
}

static void rho_platform_check_restart_application() 
{
    if (!g_bIsRestartApplication)
        return;

	char module[MAX_PATH];
    ::GetModuleFileNameA(NULL,module,MAX_PATH);
                                       
    rho_wmsys_run_app(module, (g_strCmdLine + " -restarting").c_str());
}

typedef bool (WINAPI *PCSD)();

extern "C" void rho_wm_impl_SetApplicationLicenseObj(void* pAppLicenseObj);

typedef LPCWSTR (WINAPI *PCL)(HWND, LPCWSTR, LPCWSTR, LPCWSTR);
typedef int (WINAPI *FUNC_IsLicenseOK)();
typedef void* (WINAPI *FUNC_GetAppLicenseObj)();

extern "C" void rho_wm_impl_CheckLicense()
{   
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    int nRes = 0;
    LOG(INFO) + "Start license_rc.dll";
    HINSTANCE hLicenseInstance = LoadLibrary(L"license_rc.dll");
    LOG(INFO) + "Stop license_rc.dll";
    

    if(hLicenseInstance)
    {
        PCL pCheckLicense = (PCL) GetProcAddress(hLicenseInstance, L"CheckLicense");
        FUNC_IsLicenseOK pIsOK = (FUNC_IsLicenseOK) GetProcAddress(hLicenseInstance, L"IsLicenseOK");
        LPCWSTR szLogText = 0;
        if(pCheckLicense)
        {
            StringW strLicenseW;
            StringW strCompanyW;

        #if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
            LPCTSTR szLicense = rho_wmimpl_sharedconfig_getvalue( L"LicenseKey" );
            if ( szLicense )
                strLicenseW = szLicense;

            LPCTSTR szLicenseComp = rho_wmimpl_sharedconfig_getvalue( L"LicenseKeyCompany" );
            if ( szLicenseComp )
                strCompanyW = szLicenseComp;
        #endif

            StringW strAppNameW;
            strAppNameW = RHODESAPP().getAppNameW();
            szLogText = pCheckLicense( getMainWnd(), strAppNameW.c_str(), strLicenseW.c_str(), strCompanyW.c_str() );
        }

        if ( szLogText && *szLogText )
            LOGC(INFO, "License") + szLogText;

        nRes = pIsOK ? pIsOK() : 0;
    }

#ifdef APP_BUILD_CAPABILITY_SYMBOL
    if ( nRes == 0 )
    {
        rho::BrowserFactory::getInstance()->checkLicense(getMainWnd(), hLicenseInstance);
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
#endif
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

extern "C" void rho_title_change(const int tabIndex, const char* strTitle)
{
    PostMessage( rho_wmimpl_get_mainwnd(),WM_COMMAND, ID_TITLECHANGE, (LPARAM)_tcsdup(convertToStringW(strTitle).c_str()) );
}

extern "C" void rho_qt_unset_window_proxy()
{
}

extern "C" void rho_qt_set_window_proxy(const char* host, const char* port, const char* login, const char* password)
{
}

//Hook for ruby call to refresh web view

extern "C" void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
}

extern "C" void rho_map_location(char* query)
{
}

extern "C" void rho_appmanager_load( void* httpContext, char* szQuery)
{
}

extern "C" void Init_fcntl(void)
{
}
#include <aygshell.h>
extern "C" BOOL LoadAYGShell()
{
	bool bReturnValue = FALSE;
	winversion = 2;
	g_hAygShellDLL = LoadLibrary(L"aygshell.dll");
	if (!g_hAygShellDLL)
	{
		//  Error loading AygShell.dll (used for Retrieving values from the Registry)
		LOG(INFO) + "Failed to load AygShell.dll, WAN status event will not be available";
	}
	else
	{
		lpfn_Registry_CloseNotification = (LPFN_REGISTRY_CLOSENOTIFICATION_T)GetProcAddress(g_hAygShellDLL, _T("RegistryCloseNotification"));
		lpfn_Registry_NotifyWindow      = (LPFN_REGISTRY_NOTIFYWINDOW_T)GetProcAddress(g_hAygShellDLL, _T("RegistryNotifyWindow"));
		lpfn_Registry_GetString         = (LPFN_REGISTRY_GETSTRING_T)GetProcAddress(g_hAygShellDLL, _T("RegistryGetString"));
		lpfn_Registry_GetDWORD          = (LPFN_REGISTRY_GETDWORD_T)GetProcAddress(g_hAygShellDLL, _T("RegistryGetDWORD"));
		lpfn_Camera_Capture             = (LPFN_CAMERA_CAPTURE_T)GetProcAddress(g_hAygShellDLL, _T("SHCameraCapture"));
		lpfn_GetOpen_FileEx             = (LPFN_GETOPEN_FILEEX_T)GetProcAddress(g_hAygShellDLL, _T("GetOpenFileNameEx"));

		if (!lpfn_Registry_CloseNotification)
		{
			LOG(ERROR) + "Unable to load RegistryCloseNotification, WAN Status event will be unavailable";
			bReturnValue = FALSE;
		}
		else if (!lpfn_Registry_CloseNotification)
		{
			LOG(ERROR) + "Unable to load RegistryNotifyCallback, WAN Status event will be unavailable";
		}
		else if (!lpfn_Registry_GetString)
		{
			LOG(ERROR) + "Unable to load RegistryGetString, WAN Status event will be unavailable";
		}
		else if (!lpfn_Registry_GetDWORD)
		{
			LOG(ERROR) + "Unable to load RegistryGetDWORD, WAN Status event will be unavailable";
		}
		else
		{
			winversion = 1;
			bReturnValue = TRUE;
		}
	}

	return bReturnValue;	
}

extern "C" BOOL LoadSoundDll()
{
	bool bReturnValue = FALSE;
	g_hSndDLL = LoadLibrary(L"aygshell.dll");
	if (!g_hSndDLL)
	{
		//  Error loading AygShell.dll (used for Retrieving values from the Registry)
		LOG(INFO) + "Failed to load sound api";
	}
	else
	{
		lpfn_snd_stop = 
			(LPFN_SND_STOP_T)GetProcAddress(g_hSndDLL, _T("SndStop"));
		lpfn_snd_close = 
			(LPFN_SND_CLOSE_T)GetProcAddress(g_hSndDLL, _T("SndClose"));
		lpfn_snd_open = 
			(LPFN_SND_OPEN_T)GetProcAddress(g_hSndDLL, _T("SndOpen"));
		lpfn_snd_playasync = 
			(LPFN_SND_PLAYASYNC_T)GetProcAddress(g_hSndDLL, _T("SndPlayAsync"));
		lpfn_snd_getsndflist = (LPFN_SND_GETSOUNDFILELIST_T)GetProcAddress(g_hSndDLL, _T("SndGetSoundFileList"));

	
		bReturnValue = TRUE;
	}

	return bReturnValue;	
}

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

#if defined( _WIN32_WCE )

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

#elif //!_WIN32_WCE
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

#endif //!_WIN32_WCE

// see bridge_cb.h
//
typedef struct {
    unsigned size;
    char *data;
} BridgeCB_String;

typedef int (* BridgeCB_Callback)(const BridgeCB_String *request, BridgeCB_String *result, void *user_data);
typedef VOID (WINAPI *BridgeCB_SetCallback)(BridgeCB_Callback callback, void *user_data);
typedef BOOL (WINAPI *BridgeCB_Allocate)(BridgeCB_String *string, unsigned size);

extern "C" VOID WINAPI BridgeCB_set_callback(BridgeCB_Callback callback, void *user_data);
extern void BridgeCB_set_callback_if_none(BridgeCB_Callback callback, void *user_data);
extern "C" BOOL WINAPI BridgeCB_allocate(BridgeCB_String *string, unsigned size);
extern bool BridgeCB_call(const BridgeCB_String *request, BridgeCB_String *result);
//
// see bridge_cb.h

static BridgeCB_Allocate allocate;

static int bridge_direct_callback(const BridgeCB_String *request, BridgeCB_String *result, void *user_data)
{
    // js_entry_point requires null-terminated string. Lets make one.
    char *data = (char *)malloc(request->size + 1);
    if (data == 0)
    {
        RAWLOG_ERROR1("bridge_direct_callback: malloc(%u) returns 0.", request->size + 1);
        return 0;
    }
    memcpy(data, request->data, request->size);
    data[request->size] = '\0';

    LOG(TRACE) + "bridge_direct_callback: before rho::apiGenerator::js_entry_point.";

    rho::String answer = rho::apiGenerator::js_entry_point(data);

    LOG(TRACE) + "bridge_direct_callback: after rho::apiGenerator::js_entry_point.";

    free(data);

    if (!(*allocate)(result, answer.size()))
    {
        RAWLOG_ERROR1("bridge_direct_callback: (*allocate)(..., %u) fails.", answer.size());
        return 0;
    }
    memcpy(result->data, answer.c_str(), answer.size());
    return 1;
}

static void set_bridge_direct_callback()
{
    rho::String rePath = rho_native_reruntimepath();

    int last = rePath.find_last_of('/');
    int pre_last = rePath.substr(0, last).find_last_of('/');

    rho::StringW fullPath = rho::common::convertToStringW(rePath.substr(0, pre_last)) + + L"\\NPAPI\\bridge.dll";
    
    HINSTANCE hInstance = LoadLibrary(fullPath.c_str());
    if (hInstance == NULL)
    {
        RAWLOG_ERROR("set_bridge_direct_callback: LoadLibrary(L\"bridge.dll\") returns NULL.");
        return;
    }

    allocate = (BridgeCB_Allocate)GetProcAddress(hInstance, L"BridgeCB_allocate");
    if (allocate == NULL)
    {
        RAWLOG_ERROR("set_bridge_direct_callback: GetProcAddress(..., L\"BridgeCB_allocate\") returns NULL.");
        return;
    }

    BridgeCB_SetCallback set_callback = (BridgeCB_SetCallback)GetProcAddress(hInstance, L"BridgeCB_set_callback");
    if (set_callback == NULL)
    {
        RAWLOG_ERROR("set_bridge_direct_callback: GetProcAddress(..., L\"BridgeCB_set_callback\") returns NULL.");
        return;
    }

    LOG(TRACE) + "set_bridge_direct_callback: before set_callback.";

    (*set_callback)(bridge_direct_callback, 0);

    LOG(TRACE) + "set_bridge_direct_callback: after set_callback.";
}
