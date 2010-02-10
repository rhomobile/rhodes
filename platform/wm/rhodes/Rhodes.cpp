// Rhodes.cpp : Implementation of WinMain.
#include "stdafx.h"
#include "MainWindow.h"

#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "geolocation/LocationController.h"

//#include "ServerHost.h"
//#include "logging/RhoLog.h"

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H
/*
//void runAllLogTests();
extern "C" const char* RhoGetRootPath();

#if defined(OS_WINDOWS)
extern "C" void __setRootPath(const char* path);
char* parseToken( const char* start, int len );
#endif
//
extern "C" wchar_t* wce_mbtowc(const char* a);
extern "C" char* wce_wctomb(const wchar_t* w);
extern char* canonicalizeURL(const char* path);
*/

#if defined(OS_WINDOWS)
char* parseToken( const char* start, int len );
#endif

extern "C" char* wce_wctomb(const wchar_t* w);
extern "C" wchar_t* wce_mbtowc(const char* a);

#if defined(_WIN32_WCE)
#include <regext.h>

// Global Notification Handle
HREGNOTIFY g_hNotify = NULL;

// ConnectionsNetworkCount
// Gets a value indicating the number of network connections that are currently connected.
#define SN_CONNECTIONSNETWORKCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSNETWORKCOUNT_PATH TEXT("System\\State\\Connections\\Network")
#define SN_CONNECTIONSNETWORKCOUNT_VALUE TEXT("Count")

#endif

//BOOL EnumRhodesWindowsProc(HWND hwnd,LPARAM lParam);

class CRhodesModule : public CAtlExeModuleT< CRhodesModule >
{
public :
	bool ParseCommandLine(LPCTSTR lpCmdLine, HRESULT* pnRetCode ) throw( ) {
		m_nRestarting = 1;
		TCHAR szTokens[] = _T("-/");
		LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
        getRhoRootPath();
		while (lpszToken != NULL)
		{
			if (WordCmpI(lpszToken, _T("Restarting"))==0) {
				m_nRestarting = 10;
			}
#if defined(OS_WINDOWS)
			else if (wcsncmp(lpszToken, _T("approot"),7)==0) {
				char* token = wce_wctomb(lpszToken);
				//parseToken will allocate extra byte at the end of the returned token value
				char* path = parseToken( token, strlen(token) );
				if (path) {
					int len = strlen(path);
					if (!(path[len]=='\\' || path[len]=='/')) {
						path[len] = '\\';
						path[len+1]  = 0;
					}
					m_strRootPath = path;
					free(path);
				}
				free(token);
			}
#endif
			lpszToken = FindOneOf(lpszToken, szTokens);
		}

		return __super::ParseCommandLine(lpCmdLine, pnRetCode);
	}

    // This method is called immediately before entering the message loop.
    // It contains initialization code for the application.
    // Returns:
    // S_OK => Success. Continue with RunMessageLoop() and PostMessageLoop().
    // S_FALSE => Skip RunMessageLoop(), call PostMessageLoop().
    // error code => Failure. Skip both RunMessageLoop() and PostMessageLoop().
    HRESULT PreMessageLoop(int nShowCmd) throw()
    {
        HRESULT hr = __super::PreMessageLoop(nShowCmd);
        if (FAILED(hr))
        {
            return hr;
        }
        // Note: In this sample, we don't respond differently to different hr success codes.

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
			SetForegroundWindow( HWND( DWORD(hWnd) | 0x01 ) );
			return S_FALSE;
		}

        // Create the main application window
        m_appWindow.Create(NULL, CWindow::rcDefault, TEXT("Rhodes"));
        if (NULL == m_appWindow.m_hWnd)
        {
            return S_FALSE;
        }

        rho_logconf_Init(m_strRootPath.c_str());
        LOG(INFO) + "Rhodes started";
        rho::common::CRhodesApp::Create(m_strRootPath );
        RHODESAPP().startApp();

       // m_pServerHost = new CServerHost();
        // Starting local server
        //m_pServerHost->Start(m_appWindow.m_hWnd);
        // Navigate to the "loading..." page
		m_appWindow.Navigate2(_T("about:blank"));
        // Show the main application window
        m_appWindow.ShowWindow(nShowCmd);

#if defined(_WIN32_WCE)
		// Register for changes in the number of network connections
		hr = RegistryNotifyWindow(SN_CONNECTIONSNETWORKCOUNT_ROOT,
			SN_CONNECTIONSNETWORKCOUNT_PATH, 
			SN_CONNECTIONSNETWORKCOUNT_VALUE, 
			m_appWindow.m_hWnd, 
			WM_CONNECTIONSNETWORKCOUNT, 
			0, 
			NULL, 
			&g_hNotify);
#endif
        return S_OK;
    }

	void DoViewRefresh() {
		::PostMessage(m_appWindow.m_hWnd,WM_COMMAND,IDM_REFRESH,0);
	}
    
	void DoViewNavigate(char* url) 
    {
        rho::String strUrl = RHODESAPP().canonicalizeRhoUrl(url);
        ::PostMessage( m_appWindow.m_hWnd, WM_COMMAND, IDM_NAVIGATE, (LPARAM)wce_mbtowc(strUrl.c_str()) );
    }

    void DoShowLog()
    {
        ::PostMessage(m_appWindow.m_hWnd,WM_COMMAND,IDM_LOG,0);
    }

	//char* GetCurrentLocation() {
	//	return m_appWindow.GetCurrentLocation();
	//}

	HWND GetManWindow() {
		return m_appWindow.m_hWnd;
	}

    void RunMessageLoop( ) throw( )
    {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (!m_appWindow.TranslateAccelerator(&msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        // Stop local server
        //m_pServerHost->Stop();
        //delete m_pServerHost;
        //m_pServerHost = NULL;

#if defined(OS_WINCE)
        CGPSController* pGPS = CGPSController::Instance();
        pGPS->DeleteInstance();
#endif

        rho::common::CRhodesApp::Destroy();
    }

    const rho::String& getRhoRootPath()
    {
        if ( m_strRootPath.length() == 0 )
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

            m_strRootPath = rootpath;
            m_strRootPath += "rho/";
        }

        return m_strRootPath; 
    }

    const char* getCurrentLocale()
    {
        wchar_t szLang[20];
        int nRes = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SABBREVLANGNAME , szLang, 20);
        szLang[2] = 0;
        wcslwr(szLang);

        m_strLocale = rho::common::convertToStringA(szLang);
        return m_strLocale.c_str();
    }

private:
    CMainWindow m_appWindow;
    //CServerHost* m_pServerHost;
    rho::String m_strRootPath, m_strLocale;
	int m_nRestarting;
};

CRhodesModule _AtlModule;
//
bool g_restartOnExit = false;
//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

extern "C" HWND getMainWnd() {
	return _AtlModule.GetManWindow();
}

extern "C" const char* rho_native_rhopath() 
{
    return _AtlModule.getRhoRootPath().c_str();
}

extern "C" void rho_conf_show_log()
{
    _AtlModule.DoShowLog();
}

extern "C" char* rho_sys_get_locale()
{
    return const_cast<char*>(_AtlModule.getCurrentLocale());
}

extern "C" int rho_sys_get_screen_width()
{
#ifdef _WIN32_WCE
	return GetSystemMetrics(SM_CXSCREEN);
#else
	return CMainWindow::getScreenWidth();
#endif
}

extern "C" int rho_sys_get_screen_height()
{
#ifdef _WIN32_WCE
	return GetSystemMetrics(SM_CYSCREEN);
#else
	return CMainWindow::getScreenHeight();
#endif
}

//Hook for ruby call to refresh web view

extern "C" void webview_refresh(int index) {
	_AtlModule.DoViewRefresh();
}

//Sync hook to refresh the web view
extern "C" void perform_webview_refresh() {
	webview_refresh(0);
}

extern "C" void webview_navigate(char* url, int index) {
	_AtlModule.DoViewNavigate(url);
}

extern "C" char* webview_execute_js(char* js, int index) {
//TODO: webview_execute_js
    return "";
}

extern "C" void webview_set_menu_items(VALUE valMenu) 
{
    RHODESAPP().setViewMenu(valMenu);
}

extern "C" int webview_active_tab() {
	return 0;
}

//extern "C" char* get_current_location() {
//	return _AtlModule.GetCurrentLocation();
//}

extern "C" char* webview_current_location(int index) {
    return const_cast<char*>(RHODESAPP().getCurrentUrl(index).c_str());
}

extern "C" VALUE rho_syscall(const char* callname, int nparams, char** param_names, char** param_values) {
	return 0;
}

extern "C" void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
}

extern "C" void choose_datetime(char* callback, char* title, long initial_time, int format, char* data) {
	//TODO: Implement me!
}

extern "C" void create_nativebar(int bar_type, int nparams, char** params) {
	//TODO: Implement me!
}

extern "C" void remove_nativebar() {
	//TODO: Implement me!
}

extern "C" void nativebar_switch_tab(int index) {
	//TODO: Implement me!
}

extern "C" void mapview_create(int nparams, char** params, int nannotations, char** annotation) {
    //TODO: mapview_create
}

extern "C" void rho_map_location(char* query)
{
}

extern "C" void rho_appmanager_load( void* httpContext, char* szQuery)
{
}

extern "C" void Init_RingtoneManager()
{
}

extern "C" void Init_openssl(void)
{
}

extern "C" void Init_digest(void)
{
}

extern "C" void Init_fcntl(void)
{
}

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

#if defined(OS_WINDOWS)
//parseToken will allocate extra byte at the end of the 
//returned token value
char* parseToken( const char* start, int len ) {
    int nNameLen = 0;
    while(*start==' '){ start++; len--;}

    int i = 0;
    for( i = 0; i < len; i++ ){
        if ( start[i] == '=' ){
            if ( i > 0 ){
                int s = i-1;
                for(; s >= 0 && start[s]==' '; s-- );

                nNameLen = s+1;
                break;
            }else 
                break;
        }
    }

    if ( nNameLen == 0 )
        return NULL;

    const char* szValue = start + i+1;
    int nValueLen = len - (i+1);

    while(*szValue==' ' || *szValue=='\'' || *szValue=='"' && nValueLen >= 0 ){ szValue++; nValueLen--;}
    while(nValueLen > 0 && (szValue[nValueLen-1]==' ' || szValue[nValueLen-1]=='\'' || szValue[nValueLen-1]=='"')) nValueLen--;

	char* value = (char*) malloc(nValueLen+2);
	strncpy(value, szValue, nValueLen);
	value[nValueLen] = '\0';

	return value;
}

// char -> wchar_t 
wchar_t* wce_mbtowc(const char* a)
{
	int length;
	wchar_t *wbuf;

	length = MultiByteToWideChar(CP_ACP, 0, 
		a, -1, NULL, 0);
	wbuf = (wchar_t*)malloc( (length+1)*sizeof(wchar_t) );
	MultiByteToWideChar(CP_ACP, 0,
		a, -1, wbuf, length);

	return wbuf;
}

// wchar_t -> char
char* wce_wctomb(const wchar_t* w)
{
	DWORD charlength;
	char* pChar;

	charlength = WideCharToMultiByte(CP_ACP, 0, w,
					-1, NULL, 0, NULL, NULL);
	pChar = (char*)malloc(charlength+1);
	WideCharToMultiByte(CP_ACP, 0, w,
		-1, pChar, charlength, NULL, NULL);

	return pChar;
}

#endif