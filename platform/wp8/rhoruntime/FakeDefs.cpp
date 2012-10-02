
#include <windows.h>
//#include "../../shared/ruby/include/ruby.h"
#include "../../shared/common/RhoDefs.h"

extern "C" HINSTANCE rho_wmimpl_get_appinstance()
{
    return 0;//_AtlModule.GetModuleInstance();
}

extern "C" int rho_webview_active_tab() 
{
    return 0;// CMainWindow::getInstance()->tabbarGetCurrent();
}

extern "C" void rho_webview_navigate_back()
{
    //CMainWindow::getInstance()->navigateBackCommand();
}

extern "C" void rho_webview_navigate_forward()
{
    //CMainWindow::getInstance()->navigateForwardCommand();
}

extern "C" const char* rho_webview_execute_js(const char* js, int index) 
{
/*    String strJS = "javascript:";
    strJS += js;
    rho_webview_navigate(strJS.c_str(), index)*/;
    return "";
}

extern "C" const char* rho_webview_current_location(int index) 
{
    return "";//RHODESAPP().getCurrentUrl(index).c_str();
}

//void rho_webview_set_menu_items(VALUE valMenu) 
//{
//    //rho_rhodesapp_setViewMenu(valMenu); 
//}

extern "C" void rho_webview_full_screen_mode(int enable)
{
    //CMainWindow::getInstance()->fullscreenCommand(enable);
}

extern "C" void rho_webview_set_cookie(const char *url, const char *cookie)
{
    //CMainWindow::getInstance()->setCookie(url, cookie);
}

RHO_GLOBAL void rho_map_location(char* query)
{
    //rho::String url = "http://maps.google.com/?";
    //url += query;
    //rho_webview_navigate(url.c_str(), 0);
}

RHO_GLOBAL void rho_appmanager_load( void* /*httpContext*/, const char* /*szQuery*/)
{
    // Nothing
}

RHO_GLOBAL void rho_sys_app_exit()
{
}

RHO_GLOBAL void rho_sys_report_app_started()
{
}

RHO_GLOBAL void rho_conf_show_log()
{
}

RHO_GLOBAL const char* rho_native_rhopath()
{
    return "";//rho_root_path().c_str();
}

extern "C" void rho_webview_navigate(const char* url, int index)
{ 
}

RHO_GLOBAL void rho_webview_refresh(int index)
{
}

RHO_GLOBAL const char* rho_native_reruntimepath()
{
    return "";//rho_root_path().c_str();
}

_Success_(return != 0)
_Ret_range_(1, nSize)
DWORD
WINAPI
GetModuleFileNameA(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPSTR lpFilename,
    _In_ DWORD nSize
    )
{
	return 0;
}

_Success_(return != 0)
_Ret_range_(1, nSize)
DWORD
WINAPI
GetModuleFileNameW(
    _In_opt_ HMODULE hModule,
    _Out_writes_to_(nSize, ((return < nSize) ? (return + 1) : nSize)) LPWSTR lpFilename,
    _In_ DWORD nSize
    )
{
	return 0;
}

#ifdef UNICODE
#define GetModuleFileName  GetModuleFileNameW
#else
#define GetModuleFileName  GetModuleFileNameA
#endif // !UNICODE

RHO_GLOBAL VOID
WINAPI
Sleep(
    _In_ DWORD dwMilliseconds
    )
{
}

BOOL
WINAPI
DeleteFileW(
    _In_ LPCWSTR lpFileName
    )
{
	return FALSE;
}

UINT
WINAPI
GetConsoleCP(
    VOID
    )
{
	return 0;
}

DWORD
WINAPI
WaitForSingleObject(
    _In_ HANDLE hHandle,
    _In_ DWORD dwMilliseconds
    )
{
	return 0;
}
