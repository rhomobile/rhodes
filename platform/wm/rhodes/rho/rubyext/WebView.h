#ifndef __WEB_VIEW__H__
#define __WEB_VIEW__H__

#include "ext/rho/rhoruby.h"

#include "logging/RhoLog.h"
#include "common/RhoMutexLock.h"

#define CUSTOM_MENU_ITEMS_MAX 32 //maximus number of items custom menu can contain

using namespace rho;
using namespace rho::common;
using namespace std;

class CWebView 
{
	DEFINE_LOGCLASS;

public:

	static enum 
	{
		MENU_TYPE_DEFAULT,
		MENU_TYPE_CUSTOM
	};
	
	class MenuItem {
	public:
		static enum 
		{
			TYPE_UNKNOWN          = 0,
			TYPE_SEPARATOR,
			TYPE_URL,
			TYPE_CMD_REFRESH,
			TYPE_CMD_HOME,
			TYPE_CMD_BACK,
			TYPE_CMD_SYNC,
			TYPE_CMD_OPTIONS,
			TYPE_CMD_LOG,
			TYPE_CMD_EXIT,
			TYPE_CMD_CLOSE
		};

		MenuItem () {};
		MenuItem (String label, String link);
		MenuItem (String label, String link, int id);
		MenuItem (const MenuItem &i) : m_label(i.m_label),  m_link(i.m_link), 
										m_type(i.m_type), m_id(i.m_id) {}
		
		~MenuItem () {}

		const String& getLabel ();
		const String& getLink  () { return m_link;  }
		int           getType  () { return m_type;  }
		int   		  getId    () { return m_id;    }
		void setId (int id) {m_id = id;}
				
	private:
		String m_label;
		String m_link;
		int m_type;
		int m_id;
	};

public:
	static CWebView& getCWebView();

	int  getMenuType (void);
	bool setMenuType (int type);
	
	void getMenuItems (vector<MenuItem> &items);
	void setMenuItems (vector<MenuItem> &items);
	
	bool getMenuItem (int id, MenuItem &tem);
	
private:
	CWebView();
	~CWebView ();
	
	CWebView (CWebView const&) {}
	CWebView& operator= (CWebView const&){}
    
	static void createCWebView();
	
	int  loadMenu (void);
	void unloadMenu (void);
private:
    static CWebView *m_pInstance;
    static CMutex m_mxLocker;

	vector<MenuItem> m_customMenuMenuItems;
	int m_menuType;
};

extern "C" void webview_refresh(int index);
extern "C" void perform_webview_refresh();
extern "C" void webview_navigate(char* url, int index);
extern "C" char* webview_execute_js(char* js, int index);
extern "C" void webview_set_menu_items(VALUE valMenu);
extern "C" int webview_active_tab();
extern "C" char* webview_current_location(int index);

#endif //__WEB_VIEW__H__
