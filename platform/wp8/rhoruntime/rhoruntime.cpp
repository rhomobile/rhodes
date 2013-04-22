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

#include "pch_rhoruntime.h"
#include "rhoruntime.h"

//#include "../../shared/sqlite/sqlite3.h"
//#include "logging/RhoLogConf.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"
#include "common/RhoConvertWP8.h"

#include "api_generator\js_helpers.h"

using namespace rhoruntime;
using namespace Platform;

CRhoRuntime^ CRhoRuntime::m_instance = nullptr;

CRhoRuntime::CRhoRuntime(IMainPage^ mainPage):
	m_MainPage(mainPage)
{
}

CRhoRuntime^ CRhoRuntime::getInstance(IMainPage^ mainPage)
{
	if (m_instance == nullptr)
		m_instance = ref new CRhoRuntime(mainPage);
    return m_instance;
}

CRhoRuntime^ CRhoRuntime::getInstance()
{
    return m_instance;
}

IMainPage^ CRhoRuntime::getMainPage()
{
	return m_MainPage;
}

// rhodes executed in a separate thread
void CRhoRuntime::Execute()
{
	rho::String m_strRootPath = rho_native_rhopath(), m_logPort, m_strRuntimePath;
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRuntimePath);
    //RHODESAPP().setExtManager( &m_oExtManager );

	//Create Main window
	createCustomMenu();

	RHODESAPP().startApp();

	// wait for 8 seconds
	//m_MainPage->DoWait(8000);
	
	/* Toolbar test
	rho::String strImagePath;
	rho::StringW iconW;
	m_MainPage->toolbarRemoveAllButtons();
	strImagePath = rho::common::CFilePath::join( rho_native_reruntimepath(), "/public/images/bar/back_btn.png");
	m_MainPage->toolbarAddAction(rho::common::convertStringToWP8(strImagePath), ref new String(L"Back"), ref new String(L"back"));
	strImagePath = rho::common::CFilePath::join( rho_native_reruntimepath(), "public/images/bar/forward_btn.png");
	m_MainPage->toolbarAddAction(rho::common::convertStringToWP8(strImagePath), ref new String(L"Forward"), ref new String(L"forward"));
	strImagePath = rho::common::CFilePath::join( rho_native_reruntimepath(), "/public/images/bar/home_btn.png");
	m_MainPage->toolbarAddAction(rho::common::convertStringToWP8(strImagePath), ref new String(L"Home"), ref new String(L"home"));
	strImagePath = rho::common::CFilePath::join( rho_native_reruntimepath(), "public/images/bar/gears.png");
	m_MainPage->toolbarAddAction(rho::common::convertStringToWP8(strImagePath), ref new String(L"Options"), ref new String(L"options"));
	m_MainPage->toolbarAddAction(nullptr, ref new String(L"Refresh"), ref new String(L"refresh"));
	m_MainPage->toolbarShow();
	*/

	/* Tabbar test
	m_MainPage->tabbarInitialize();
	rho::String strIconPath1 = rho::common::CFilePath::join( RHODESAPP().getAppRootPath(), "/public/images/bar/gears.png");
	rho::StringW icon1 = rho::common::convertToStringW(strIconPath1);
	rho::String strIconPath2 = rho::common::CFilePath::join( RHODESAPP().getAppRootPath(), "public/images/bar/forward_btn.png");
	rho::StringW icon2 = rho::common::convertToStringW(strIconPath2);
	rho::String strIconPath3 = rho::common::CFilePath::join( RHODESAPP().getAppRootPath(), "/public/images/bar/home_btn.png");
	rho::StringW icon3 = rho::common::convertToStringW(strIconPath3);
	m_MainPage->tabbarAddTab(ref new String(L"Tab 1"), ref new String(icon1.c_str()), ref new String(L"/app"), false, ref new String(L""), ref new String(L""), false, false);
	m_MainPage->tabbarAddTab(ref new String(L"Tab 2"), ref new String(icon2.c_str()), ref new String(L"/app"), false, ref new String(L""), ref new String(L""), false, false);
	m_MainPage->tabbarAddTab(ref new String(L"Tab 3"), ref new String(icon3.c_str()), ref new String(L"/app"), false, ref new String(L""), ref new String(L""), false, false);
	m_MainPage->tabbarShow();
	*/

	//m_MainPage->fullscreenCommand(1);
	//m_MainPage->exitCommand();
}


// *** CALLBACKS from MainPage object ***

void CRhoRuntime::updateSizeProperties(int width, int height)
{
	// TODO: implement
}

void CRhoRuntime::onActivate(int active)
{
	RHODESAPP().callUiCreatedCallback();
}

void CRhoRuntime::logEvent(::Platform::String^ message)
{
	LOG(INFO) + rho::common::convertStringWFromWP8(message);
}

void CRhoRuntime::createCustomMenu()
{
	IMainPage^ mainPage = CRhoRuntime::getInstance()->getMainPage();
	rho::Vector<rho::common::CAppMenuItem> m_arAppMenuItems;
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);

    //update UI with custom menu items
	mainPage->menuClear();
	//RHODESAPP().setAppBackUrl("");
    for ( unsigned int i = 0; i < m_arAppMenuItems.size(); i++)
    {
        rho::common::CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
        if (oItem.m_eType == rho::common::CAppMenuItem::emtSeparator) 
            mainPage->menuAddSeparator();
        else {
			//if ((oItem.m_eType == rho::common::CAppMenuItem::emtBack) && (oItem.m_strLink.compare("back") != 0))
			//	RHODESAPP().setAppBackUrl(oItem.m_strLink);
			mainPage->menuAddAction((oItem.m_eType == rho::common::CAppMenuItem::emtClose ? "Exit" : rho::common::convertStringToWP8(oItem.m_strLabel)), i);
        }
    }
}

void CRhoRuntime::onCustomMenuItemCommand(int nItemPos)
{
	rho::Vector<rho::common::CAppMenuItem> m_arAppMenuItems;
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);

	if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return;

    rho::common::CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    oMenuItem.processCommand();
}

void CRhoRuntime::onToolbarAction(::Platform::String^ action)
{
	if ( action->Equals("forward") )
		rho_webview_navigate_forward();
	else
		RHODESAPP().loadUrl(rho::common::convertStringAFromWP8(action));
}

void CRhoRuntime::onTabbarCurrentChanged(int index, ::Platform::String^ action)
{
	// TODO: complete implementation

	//if (tbrp["on_change_tab_callback"].toString().length() > 0) {
	//	QString body = QString("&rho_callback=1&tab_index=") + QVariant(index).toString();
	//	rho::String* cbStr = new rho::String(tbrp["on_change_tab_callback"].toString().toStdString());
	//	rho::String* bStr = new rho::String(body.toStdString());
	//	const char* b = bStr->c_str();
	//	rho_net_request_with_data(RHODESAPP().canonicalizeRhoUrl(*cbStr).c_str(), b);
	//}

    //if (tbrp["reload"].toBool() || (ui->webView && (ui->webView->history()->count()==0))) {
	RHODESAPP().loadUrl(rho::common::convertStringAFromWP8(action));
    //}
}


void CRhoRuntime::onWindowClose(void)
{
	// TODO: implement
}

void CRhoRuntime::onWebViewUrlChanged(::Platform::String^ url)
{
	RHODESAPP().keepLastVisitedUrl(rho::common::convertStringAFromWP8(url));
}

bool CRhoRuntime::onBackKeyPress()
{
	// TODO: implement check for history size (return false if there's no history)
	RHODESAPP().navigateBack();
	return true;
}

::Platform::String^ CRhoRuntime::onJSInvoke(::Platform::String^ inJSON)
{
	rho::String ret = rho::apiGenerator::js_entry_point(rho::common::convertToStringA(inJSON->Data()).c_str());
	return ref new ::Platform::String(rho::common::convertToStringW(ret).c_str());
}

// *** PUBLIC METHODS ***

bool CRhoRuntime::Initialize(::Platform::String^ title)
{
	return true;
}

void CRhoRuntime::DestroyUi(void)
{
	rho_rhodesapp_callUiDestroyedCallback();
}

extern "C" void rho_sys_app_exit()
{
	rho_rhodesapp_callUiDestroyedCallback();

	//TODO: fix issue with stop thread - stopWait does not wakeup sleeping thread
	//rho::common::CRhodesApp::Destroy();

	CRhoRuntime::getInstance()->getMainPage()->exitCommand();
}

#ifdef OS_WP8
extern "C" void rho_net_impl_network_indicator(int active)
{
	 //TODO: rho_net_ping_network
    //RAWLOGC_INFO("rhoruntime.cpp", "rho_net_impl_network_indicator() has no implementation on WP8.");
}

extern "C" int rho_net_ping_network(const char* szHost)
{
    //TODO: rho_net_ping_network
    //RAWLOGC_INFO("rhoruntime.cpp", "rho_net_ping_network() has no implementation on WP8.");
    return 1;
}
#endif

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask)
{
	//TODO: rho_wm_impl_performOnUiThread
}

extern "C" const char* rho_native_rhopath()
{
    return "rho/";
}

extern "C" const char* rho_native_get_appname()
{
    return "Rhodes";
}

RHO_GLOBAL const char* rho_native_reruntimepath()
{
    return rho_native_rhopath();
}
