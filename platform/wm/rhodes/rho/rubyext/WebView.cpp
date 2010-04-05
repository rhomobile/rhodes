#include "stdafx.h"

#include "common/RhodesApp.h"
#include "MainWindow.h"

#include "WebView.h"

extern "C" HWND getMainWnd();

IMPLEMENT_LOGCLASS(CWebView, "WebView");

CWebView *CWebView::m_pInstance = NULL;
CMutex CWebView::m_mxLocker;

CWebView::MenuItem::MenuItem (String label, String link)
{
	m_label = label;
	m_link  = link;
	m_id    = -1;
	
	//TODO: table
	if (label == "separator")
		m_type = TYPE_SEPARATOR;
	else if (link == "home")
		m_type = TYPE_CMD_HOME;
	else if (link == "refresh")
		m_type = TYPE_CMD_REFRESH;
	else if (link == "options")
		m_type = TYPE_CMD_OPTIONS;
	else if (link == "log")
		m_type = TYPE_CMD_LOG;
	else if (link == "sync")
		m_type = TYPE_CMD_SYNC;
	else if (link == "close")
		m_type = TYPE_CMD_CLOSE;
	else
		m_type = TYPE_URL;
}

CWebView::MenuItem::MenuItem (String label, String link, int id)
{
	m_label = label;
	m_link  = link;
	m_id    = id;
	
	//TODO: table
	if (label == "separator")
		m_type = TYPE_SEPARATOR;
	else if (link == "home")
		m_type = TYPE_CMD_HOME;
	else if (link == "refresh")
		m_type = TYPE_CMD_REFRESH;
	else if (link == "options")
		m_type = TYPE_CMD_OPTIONS;
	else if (link == "log")
		m_type = TYPE_CMD_LOG;
	else if (link == "sync")
		m_type = TYPE_CMD_SYNC;
	else if (link == "close")
		m_type = TYPE_CMD_CLOSE;
	else
		m_type = TYPE_URL;
}

const String& CWebView::MenuItem::getLabel () 
{ 
	return m_label; 
}

CWebView::CWebView() 
{
	m_menuType = MENU_TYPE_DEFAULT;
}

CWebView::~CWebView () {}

void CWebView::createCWebView()
{
    static CWebView instance;
    m_pInstance = &instance;
}

CWebView &CWebView::getCWebView()
{
    m_mxLocker.Lock();

    if (!m_pInstance)
        createCWebView();
    
    m_mxLocker.Unlock();
    
    return *m_pInstance;
}

int  CWebView::getMenuType (void)
{
	return m_menuType;
}

bool CWebView::setMenuType (int type)
{	
	bool ret = false;

	if (type == MENU_TYPE_DEFAULT){
		m_menuType = MENU_TYPE_DEFAULT;
		unloadMenu();
		ret = true;
	} else if (type == MENU_TYPE_CUSTOM) {
		unloadMenu();
		if (loadMenu() == 0) {
			ret = false;
		} else {
			m_menuType = MENU_TYPE_CUSTOM;
			ret = true;
		}
	} else {
		ret = false;
	}

	return ret;
}	

int  CWebView::loadMenu (void)
{	
	Hashtable<String, String> hash = RHODESAPP().getViewMenu();
	
	for (Hashtable<String, String>::iterator itr = hash.begin(); itr != hash.end(); ++itr) {
		MenuItem menuItem = MenuItem(itr->first.c_str(), itr->second.c_str());
		m_customMenuMenuItems.push_back(menuItem) ;
	}
	
	return m_customMenuMenuItems.size();
}

void CWebView::unloadMenu (void)
{
	m_customMenuMenuItems.clear();
}

void CWebView::getMenuItems (vector<MenuItem> &items)
{
	items = m_customMenuMenuItems;
}

void CWebView::setMenuItems (vector<MenuItem> &items)
{
	m_customMenuMenuItems = items;
}

bool CWebView::getMenuItem (int id, MenuItem &item) 
{
	for (vector<CWebView::MenuItem>::iterator itr = m_customMenuMenuItems.begin(); 
		itr != m_customMenuMenuItems.end(); ++itr) 
	{
		if ((itr)->getId() == id) {
			item = *itr;
			return true;
		}
	}
	return false;
}

/**
 * C interface
 */

//Sync hook to refresh the web view
void perform_webview_refresh() {
	webview_refresh(0);
}


void webview_set_menu_items(VALUE valMenu) 
{
	LOG(INFO) + __FUNCTION__;
    RHODESAPP().setViewMenu(valMenu); 
	CWebView::getCWebView().setMenuType(CWebView::MENU_TYPE_CUSTOM);
	::PostMessage(getMainWnd(), WM_SET_CUSTOM_MENU, 0, 0);
}

 int webview_active_tab() {
	return 0;
}

 char* webview_current_location(int index) {
    return const_cast<char*>(RHODESAPP().getCurrentUrl(index).c_str());
}
