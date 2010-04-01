#ifndef __WEB_VIEW__H__
#define __WEB_VIEW__H__

#include "ext/rho/rhoruby.h"

#include "logging/RhoLog.h"
#include "common/RhoMutexLock.h"

using namespace rho;
using namespace rho::common;

class CWebView 
{
	DEFINE_LOGCLASS;
public:
	static CWebView& getCWebView();

private:
	CWebView() {};
	~CWebView () {};
	
	CWebView (CWebView const&) {}
	CWebView& operator= (CWebView const&){}
    
    static void createCWebView();
    
    static CWebView *m_pInstance;
    static CMutex m_mxLocker;

public:
	bool m_bCustomMenu;
};

extern "C" void webview_refresh(int index);
extern "C" void perform_webview_refresh();
extern "C" void webview_navigate(char* url, int index);
extern "C" char* webview_execute_js(char* js, int index);
extern "C" void webview_set_menu_items(VALUE valMenu);
extern "C" int webview_active_tab();
extern "C" char* webview_current_location(int index);

#endif //__WEB_VIEW__H__
