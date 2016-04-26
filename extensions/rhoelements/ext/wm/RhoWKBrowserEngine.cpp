#include "stdafx.h"

//#define USINGOUTPROCESSENGINE 1

//#include "common/app_build_capabilities.h"
#include "common/SplashScreen.h"
#include "common/RhodesApp.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "common/StringConverter.h"
#include "common/version.h"
#include "RhoWKBrowserEngine.h"
#include "RhoElementsMsgs.h"

#include "../../../../neon/Helium/HeliumForWindows/Implementation/Engine/OutProcessLauncher/EngOutProcWK.h"
#include "../../../../neon/Helium/HeliumForWindows/Implementation/Engine/InProcessLauncher/EngInProcWK.h"
 
#include "logging/RhoLog.h"
#include "common/app_build_configs.h"
#include "../../../../../../extensions/rhoelementsext/ext/rhoelementsext/platform/wm/src/gen/version.h"

#define CAMERAWND_CLASSNAME TEXT("Camera View")

extern "C" HWND rho_wmimpl_get_mainwnd();
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" LRESULT rho_wmimpl_draw_splash_screen(HWND hWnd);
extern "C" LRESULT rho_wm_appmanager_ProcessOnTopMostWnd( WPARAM wParam, LPARAM lParam );
extern "C" void rho_title_change(const int tabIndex, const char* strTitle);
extern "C" double rho_wmimpl_get_pagezoom();

//Used for getting the editable window handle
HWND m_hEditableWindow = NULL;
//Used for checking whether the focus is on editable field or not.
BOOL g_EditableControlHasFocus = FALSE; //Initially it will be initialised with FALSE

//Application Quitting is true or false
bool appQuitted = false;

//  DCC - For Testing Only
int REPLACE_WITH_ACTUAL_TAB_ID = 0;

class CSRCExtChecker
{
    LPTSTR mTokenValue;    
  public:
	CSRCExtChecker();
    LPVOID getTokenValue();
    void setTokenValue(LPVOID lpValue);   
};

CSRCExtChecker::CSRCExtChecker(){
        mTokenValue = L"";
}
LPVOID CSRCExtChecker::getTokenValue(){
	if(mTokenValue)
        return mTokenValue;
	return NULL;
}
void CSRCExtChecker::setTokenValue(LPVOID lpValue){
	if(lpValue)
        mTokenValue = (LPTSTR)lpValue;
}

namespace rho
{
IMPLEMENT_LOGCLASS(CRhoWKBrowserEngine,"WKEngine");

WNDPROC CRhoWKBrowserEngine::m_WebKitOwnerProc = 0;
CRhoWKBrowserEngine* CRhoWKBrowserEngine::m_pInstance = 0;

LRESULT CALLBACK onTopmostWnd(EngineEventID eeID, LPARAM value, int tabIndex)
{
    LOG(INFO) + "onTopmostWnd";

	// The engine needs to navigate to a page before it can provide the topmost window.
	// We cannot call preload until we have a valid window handle
	// If all of the instances have been loaded then the meta event and navigate event will be registered 
	
	return PostMessage(rho_wmimpl_get_mainwnd(),PB_ONTOPMOSTWINDOW,(LPARAM)tabIndex,(WPARAM)value);
}

LRESULT CALLBACK WK_ExitHandler(EngineEventID eeID, LPARAM value, int tabIndex)
{
    RHODESAPP().getExtManager().quitApp();
    return (LRESULT)TRUE;
}

extern "C" CEng* rho_wmimpl_get_webkitbrowser(HWND hParentWnd, HINSTANCE hInstance)
{
    return CRhoWKBrowserEngine::getInstance(hParentWnd, hInstance)->getWebKitBrowser();
}

CRhoWKBrowserEngine* CRhoWKBrowserEngine::getInstance()
{
    return m_pInstance;
}

CRhoWKBrowserEngine* CRhoWKBrowserEngine::getInstance(HWND hParentWnd, HINSTANCE hInstance)
{
    if (!m_pInstance)
        m_pInstance = new CRhoWKBrowserEngine(hParentWnd, hInstance);
    return m_pInstance;
}

CEng* CRhoWKBrowserEngine::getWebKitBrowser()
{
    return m_pEngine;
}

CRhoWKBrowserEngine::CRhoWKBrowserEngine(HWND hParentWnd, HINSTANCE hInstance) : m_pEngine(NULL)
{	
	LOG(INFO) + RHO_VERSION;
	m_WKConfig = CRhoWKBrowserConfig::getInstance();
    
	
	m_hParentWnd = hParentWnd;
    //g_hBaseWnd = hParentWnd;
    m_bLoadingComplete = false;
    m_bLicenseChecked = false;
    m_pInstance = this;

	//webkit engine started, hence setting it to false
	appQuitted = false;
    
	if( RHODESAPP().isWebkitOutofProcess() )
    {
	    m_pEngine = new CEngOutProcWK(hParentWnd, CRhoWKBrowserConfig::getInstance()->getConfig());
		// Setting isWebkitInProcess to false as webkit out process is used.
		if(m_pEngine != NULL)
		{
			m_pEngine->isWebkitInProcess = false;
		}
		
	    HMODULE hModule = GetModuleHandleW(NULL);
	    WCHAR path[MAX_PATH];
	    GetModuleFileNameW(hModule, path, MAX_PATH);
	    WCHAR *pPath = wcsrchr(path, L'\\');
	    wsprintf(pPath, L"\\OutProcessWK.exe");
        LOG(INFO) + "START WebKit";
	    if (m_pEngine->Start(path))
	    {
            m_pEngine->RegisterForInitComplete(onTopmostWnd);
	    }
        else
        {
            RHODESAPP().getExtManager().quitApp();
	    }
    }
    else
    {
	    HMODULE hModule = GetModuleHandleW(NULL);
	    WCHAR path[MAX_PATH];
	    GetModuleFileNameW(hModule, path, MAX_PATH);
	    WCHAR *pPath = wcsrchr(path, L'\\');
	    wsprintf(pPath, L"\\WebkitPlatformDeliveryCompiledAsDLL.dll");
    	
	    m_pEngine = new CEngInProcWK(hParentWnd,hInstance,m_WKConfig->getConfig());
		// Setting isWebkitInProcess to true as webkit in process is used.
		if(m_pEngine != NULL)
		{
			m_pEngine->isWebkitInProcess = true;
		}
		
	    if (m_pEngine->Start(path))
	    {
		    m_pEngine->RegisterForEvent(EEID_TOPMOSTHWNDAVAILABLE, rho::onTopmostWnd);
	    }
        else
        {
            RHODESAPP().getExtManager().quitApp();
	    }
    }	
    LOG(INFO) + "END WebKit";
}

CRhoWKBrowserEngine::~CRhoWKBrowserEngine(void)
{
	//application quitting hence setting it to true
	appQuitted = true;
	m_pEngine->Shutdown();
    delete m_pEngine;
}

extern "C" bool isApplicationQuitting(){
	return appQuitted;
}

BOOL CRhoWKBrowserEngine::Navigate(LPCTSTR szURL, int iTabID)
{
    LOG(INFO) + "Navigate :" + szURL;

    BOOL res;

#ifdef _DEBUG
	//  DCC Testing the multi tab interface
	if (wcsstr(szURL, L"createTab1") != 0)
	{
		//  Design is to always specify bForeground as False
		int ret = NewTab();
		//  TODO - NewTab should be blocking.  Since it isn't currently this doesn't work.
		SwitchTab(1);
		return TRUE;
	}
	else if (wcsstr(szURL, L"navigateTab1") != 0)
	{
		REPLACE_WITH_ACTUAL_TAB_ID = 1;
		LRESULT ret2 = Navigate(L"file:///he/miscellaneous/alerts.htm", 0);
		return TRUE;
	}
	else if (wcsstr(szURL, L"createTab2") != 0)
	{
		int ret = NewTab();
		SwitchTab(2);
		return TRUE;
	}
	else if (wcsstr(szURL, L"navigateTab2") != 0)
	{
		REPLACE_WITH_ACTUAL_TAB_ID = 2;
		LRESULT ret2 = Navigate(L"file:///he/miscellaneous/jserrors.htm", 0);
		return TRUE;
	}
	else if (wcsstr(szURL, L"switch0") != 0)
	{
		REPLACE_WITH_ACTUAL_TAB_ID = 0;
		SwitchTab(0);
		return TRUE;
	}
	else if (wcsstr(szURL, L"switch1") != 0)
	{
		REPLACE_WITH_ACTUAL_TAB_ID = 1;
		SwitchTab(1);
		return TRUE;
	}
	else if (wcsstr(szURL, L"switch2") != 0)
	{
		REPLACE_WITH_ACTUAL_TAB_ID = 2;
		SwitchTab(2);
		return TRUE;
	}
	else if (wcsstr(szURL, L"close1") != 0)
	{
		CloseTab(1);
		return TRUE;
	}
	else if (wcsstr(szURL, L"close2") != 0)
	{
		CloseTab(2);
		return TRUE;
	}
#endif

    if (wcslen(szURL) >= wcslen(L"javascript:") && wcsnicmp(szURL, L"javascript:", 11) == 0) {
        res = m_pEngine->StopOnTab(iTabID);
        if (res)
            res = m_pEngine->JavaScriptInvoke(iTabID, szURL+11);
    } else {
        res = m_pEngine->NavigateOnTab(szURL, iTabID);
        //SetFocus(m_pEngine->GetHTMLWND(iTabID));
        InvalidateRect(m_pEngine->GetHTMLWND(iTabID), NULL, FALSE);
    }
    return res;
}

HWND CRhoWKBrowserEngine::GetHTMLWND(int iTabID)
{
	return m_pEngine->GetHTMLWND(iTabID);
}

//  Creates a new Tab and returns the newly created tab ID, will be -1 in case of error.
int CRhoWKBrowserEngine::NewTab()
{
	int iNewTab = m_pEngine->NewTab();
	ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iNewTab);
	return iNewTab;
}

//  Switches to the specified tab ID, returns the ID of the previous tab 
//  or -1 in case of error
int CRhoWKBrowserEngine::SwitchTab(int iTabID)
{
	if (iTabID >= 0){
		ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iTabID);
		return m_pEngine->SwitchTab(iTabID);
	}
	else
		return -1;
}

BOOL CRhoWKBrowserEngine::CloseTab(int iTabID)
{
	if (iTabID <= 0)
	{
		LOG(WARNING) + "ID of Tab to close must be positive and non-zero";
		return FALSE;
	}
	return m_pEngine->CloseTab(iTabID);
}

BOOL CRhoWKBrowserEngine::ResizeOnTab(int iInstID,RECT rcNewSize)
{
	ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iInstID);
    return m_pEngine->ResizeOnTab(iInstID,rcNewSize);
}

BOOL CRhoWKBrowserEngine::BackOnTab(int iInstID,int iPagesBack/* = 1*/)
{
	ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iInstID);   
	BOOL bRes = m_pEngine->BackOnTab(iInstID,iPagesBack);
    if ( bRes )
        InvalidateRect(m_pEngine->GetHTMLWND(iInstID), NULL, FALSE);

    return bRes;
}

BOOL CRhoWKBrowserEngine::ForwardOnTab(int iInstID)
{
	ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iInstID);
    BOOL bRes = m_pEngine->ForwardOnTab(iInstID);

    if ( bRes )
        InvalidateRect(m_pEngine->GetHTMLWND(iInstID), NULL, FALSE);

    return bRes;
}

BOOL CRhoWKBrowserEngine::ReloadOnTab(bool bFromCache, UINT iTab)
{
	ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iTab);
    BOOL bRes = m_pEngine->ReloadOnTab(bFromCache, iTab);
    if ( bRes )
        InvalidateRect(m_pEngine->GetHTMLWND(iTab), NULL, FALSE);

    return bRes;
}

BOOL CRhoWKBrowserEngine::StopOnTab(UINT iTab)
{
	ZoomPageOnTab((float)rho_wmimpl_get_pagezoom(),iTab);
    return m_pEngine->StopOnTab(iTab);
}

BOOL CRhoWKBrowserEngine::ZoomPageOnTab(float fZoom, UINT iTab)
{
    return m_pEngine->ZoomPageOnTab(fZoom, iTab);
}

BOOL CRhoWKBrowserEngine::ZoomTextOnTab(int nZoom, UINT iTab)
{
    return m_pEngine->ZoomTextOnTab((TextZoomValue)nZoom, iTab);
}

int CRhoWKBrowserEngine::GetTextZoomOnTab(UINT iTab)
{
    TextZoomValue eZoomLevel;
    m_pEngine->GetTextZoomOnTab(&eZoomLevel, iTab);
    return eZoomLevel;
}

BOOL CRhoWKBrowserEngine::NavigateToHtml(LPCTSTR szHtml)
{
    //return m_pEngine->NavigateToHtml(szHtml);
    //TODO: NavigateToHtml

    return FALSE;
}

bool CRhoWKBrowserEngine::isExistJavascript(const wchar_t* szJSFunction, int index)
{
    return m_pEngine->JavaScript_Exist(index, szJSFunction) == TRUE ? true : false;
}

void CRhoWKBrowserEngine::executeJavascript(const wchar_t* szJSFunction, int index)
{
    StringW strUrlW;
    if(_memicmp(szJSFunction,L"JavaScript:",11*2) != 0)
        strUrlW = L"javascript:";	

    strUrlW += szJSFunction;
	StringW find = L"\"";
	StringW replace = L"'";
    findAndReplace<StringW>(strUrlW, find, replace);
    m_pEngine->JavaScriptInvoke(index, strUrlW.c_str() );
}

void CRhoWKBrowserEngine::setBrowserGesturing(bool bEnableGesturing)
{
    m_pEngine->SetBrowserGesturing(bEnableGesturing);
}

void CRhoWKBrowserEngine::NotifyEngineOfSipPosition()
{
    m_pEngine->NotifyEngineOfSipPosition();
}

void CRhoWKBrowserEngine::setNavigationTimeout(unsigned int dwMilliseconds)
{
    m_pEngine->SetNavigationTimeout(dwMilliseconds);
}

bool CRhoWKBrowserEngine::RegisterForMessage(unsigned int iMsgId)
{
    return (m_pEngine->RegisterForMessage(iMsgId) == TRUE);
}

bool CRhoWKBrowserEngine::DeRegisterForMessage(unsigned int iMsgId)
{
    return (m_pEngine->DeRegisterForMessage(iMsgId) == TRUE);
}

bool CRhoWKBrowserEngine::RegisterForPrimaryMessage(unsigned int iMsgId)
{
    return (m_pEngine->RegisterForPrimaryMessage(iMsgId) == TRUE);
}

bool CRhoWKBrowserEngine::DeRegisterForPrimaryMessage(unsigned int iMsgId)
{
    return (m_pEngine->DeRegisterForPrimaryMessage(iMsgId) == TRUE);
}

BOOL CRhoWKBrowserEngine::GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab)
{
    return m_pEngine->GetTitleOnTab(szURL, iMaxLen, iTab);
}

LRESULT CALLBACK CRhoWKBrowserEngine::WK_HTMLWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( !getInstance()->m_bLoadingComplete && message == WM_PAINT)
    {
		if ( rho_wmimpl_draw_splash_screen(getInstance()->GetHTMLWND(RHODESAPP().getExtManager().getCurrentTabID())) )
            return 0;
    }
	
	if(RHODESAPP().m_cameraOpened)
	{
		HWND hwndCamera = FindWindow(CAMERAWND_CLASSNAME, NULL);
		if (NULL != hwndCamera)
			SetActiveWindow(hwndCamera);
	}

	// Pass message to original window procedure
	//return CallWindowProc (m_WebKitOwnerProc, hwnd, message, wParam, lParam);

    LRESULT result = CallWindowProc (m_WebKitOwnerProc, hwnd, message, wParam, lParam);
    MSG msg = {0};
    msg.hwnd = hwnd;
    msg.lParam = lParam;
    msg.message = message;
    msg.wParam = wParam;
    RHODESAPP().getExtManager().onHTMLWndMsg(msg);
    return result;
}

BOOL CALLBACK CRhoWKBrowserEngine::WK_WMMsgHandler(int iTabID,LPMSG pMsg)
{
	/******** THIS CANNOT STAY HERE, AS THE FUNCTION BELOW IT WOULD NEVER EXECUTE*********/ 
	MSG msg = *pMsg;
	if(pMsg->message == WM_PAINT)
	{
		return RHODESAPP().getExtManager().onHTMLWndMsg(msg);
	}

	/*
	if (pMsg->message == WM_KILLFOCUS)
	{
		if (CRhoWKBrowserConfig::getInstance()->getFullScreen())
		{
			HWND taskbarWnd = FindWindow(L"HHTaskBar", NULL);
			ShowWindow(taskbarWnd, SW_HIDE);
			return FALSE;
		}
	}
	*/
  
	if( !getInstance()->m_bLoadingComplete && pMsg->message == WM_PAINT)
	{
	   if ( rho_wmimpl_draw_splash_screen(getInstance()->GetHTMLWND(RHODESAPP().getExtManager().getCurrentTabID())) )
		  return FALSE;
	}
  
	if(RHODESAPP().m_cameraOpened)
	{
		HWND hwndCamera = FindWindow(CAMERAWND_CLASSNAME, NULL);
		if (NULL != hwndCamera)
			SetActiveWindow(hwndCamera);
	}

	if(RHODESAPP().isWebkitOutofProcess())
		return RHODESAPP().getExtManager().onWndMsg(msg);
	else
		return FALSE;
	
}

LRESULT CALLBACK CRhoWKBrowserEngine::WK_GetEngineConfig(int iInstID, LPCTSTR tcSetting, TCHAR* tcValue)
{
	LPCTSTR tcValueRead;

    tcValueRead = CRhoWKBrowserConfig::getInstance()->getConfig()->GetAppSettingPtr(iInstID, tcSetting, L"Value");
	if (tcValueRead != NULL)
		wcscpy(tcValue, tcValueRead);
	else
		tcValue = NULL;

    LOG(INFO) + "WebKit Settings: " + tcSetting + "=" + (tcValue ? tcValue : L"NULL");

	return S_OK;
} 

LRESULT CALLBACK onNavEvent(EngineEventID eeID, LPARAM value, int tabIndex)
{
	BOOL bOk = FALSE;
	DWORD dwRes = WAIT_FAILED;
    HWND parentHWND = CRhoWKBrowserEngine::getInstance()->GetParentHWND();
	switch(eeID)
	{
    case EEID_TITLECHANGE:
        rho_title_change(tabIndex, rho::common::convertToStringA((LPCTSTR)value).c_str());
        break;
	case EEID_BEFORENAVIGATE:		
	g_EditableControlHasFocus = FALSE;
        if ( !CRhoWKBrowserEngine::getInstance()->getWKConfig()->isHybridApp() )
            PostMessage(parentHWND, WM_BROWSER_ONBEFORENAVIGATE, (WPARAM)tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value) );
        else
        { 
            if ( CRhoWKBrowserEngine::getInstance()->getWebKitEngine()->Navigable((LPCTSTR)value) )
                PostMessage(parentHWND, WM_BROWSER_ONBEFORENAVIGATE, (WPARAM)tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value));
            else
                RHODESAPP().getExtManager().OnLicenseError((LPCTSTR)value);
        }
        break;
    case EEID_NAVIGATECOMPLETE:
        PostMessage(parentHWND, WM_BROWSER_ONNAVIGATECOMPLETE, (WPARAM)tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value) );
        break;
	case EEID_DOCUMENTCOMPLETE:
		{
			LOG(INFO) + "before calling dominjector ";
			RHODESAPP().getExtManager().getEngineEventMngr().injectDOMElements();
			PostMessage(parentHWND, WM_BROWSER_ONDOCUMENTCOMPLETE, (WPARAM)tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value));

            if ( !CRhoWKBrowserEngine::getInstance()->m_bLoadingComplete )
            {
                CRhoWKBrowserEngine::getInstance()->m_bLoadingComplete = true;
                InvalidateRect(CRhoWKBrowserEngine::getInstance()->GetHTMLWND(RHODESAPP().getExtManager().getCurrentTabID()), NULL, FALSE);
            }

			break;
		}
    case EEID_NAVIGATIONERROR:
		//Check for common protocols
		if (
			(_tcsncmp((LPCTSTR)value,TEXT("tel:"),4) == 0) ||
			(_tcsncmp((LPCTSTR)value,TEXT("sms:"),4) == 0) ||
			(_tcsncmp((LPCTSTR)value,TEXT("mailto:"),7) == 0))
			{
				SHELLEXECUTEINFO	shInfo;
	
				//Run the Program
				ZeroMemory(&shInfo,sizeof(shInfo));
				shInfo.cbSize = sizeof(shInfo);
				shInfo.fMask  = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
				shInfo.lpVerb   = L"open";
				shInfo.lpFile = (LPCTSTR)value;
				shInfo.nShow  = SW_SHOWNORMAL;
				ShellExecuteEx(&shInfo);
				CloseHandle(shInfo.hProcess);
				PostMessage(parentHWND, WM_BROWSER_ONDOCUMENTCOMPLETE, tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value) );
				return S_OK;
			}

        LOG(ERROR) + "Navigation error: " + (LPCTSTR)value;

        PostMessage(parentHWND, WM_BROWSER_ONNAVIGATIONERROR, (WPARAM)tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value));
        return S_OK;
        break;
    case EEID_NAVIGATIONTIMEOUT:
        PostMessage(parentHWND, WM_BROWSER_ONNAVIGATIONTIMEOUT, (WPARAM)tabIndex, (LPARAM)_tcsdup( (LPCTSTR)value));
        return S_OK;
        break;	
    }

    return S_OK;
}

LRESULT CALLBACK onMeta(EngineEventID eeID, LPARAM value, int tabIndex)
{
	EngineMETATag metaTag,*pMeta = (EngineMETATag*)value;
	LRESULT lRet;
	 
	if(pMeta->tcHTTPEquiv && *pMeta->tcHTTPEquiv){
		metaTag.tcHTTPEquiv = new WCHAR[wcslen(pMeta->tcHTTPEquiv)+1];
		wcscpy(metaTag.tcHTTPEquiv,pMeta->tcHTTPEquiv);
	}
	if(pMeta->tcContents && *pMeta->tcContents){
		metaTag.tcContents = new WCHAR[wcslen(pMeta->tcContents)+1];
		wcscpy(metaTag.tcContents,pMeta->tcContents);
	}
	
	EngineMETATag* metaTag2 = new EngineMETATag;
	metaTag2->tcHTTPEquiv = _tcsdup(metaTag.tcHTTPEquiv);
	metaTag2->tcContents = _tcsdup(metaTag.tcContents);
	lRet = PostMessage(rho_wmimpl_get_mainwnd(),PB_ONMETA,(WPARAM)tabIndex, (LPARAM)metaTag2);
	delete []metaTag.tcHTTPEquiv;
	delete []metaTag.tcContents;
	return lRet;


}

LRESULT CALLBACK onFieldFocus(EngineEventID eeID, LPARAM value, int tabIndex)
{
    g_EditableControlHasFocus = (BOOL)value;
	m_hEditableWindow = GetFocus();
    return PostMessage(rho_wmimpl_get_mainwnd(), WM_BROWSER_ONSETSIPSTATE, (WPARAM)tabIndex, value);
}

LRESULT CALLBACK onConsoleMessage(EngineEventID eeID, LPARAM value, int tabIndex)
{
    LOG(INFO) + "JS Log: " + (LPCTSTR)value;

    return S_OK;
}

LRESULT CALLBACK OnGeolocationData(EngineEventID eeID, LPARAM value, int tabIndex)
{
	if (eeID == EEID_GPS_POSITIONRECEIVED)
		PostMessage(rho_wmimpl_get_mainwnd(), PB_NEWGPSDATA, 0, 0);
	else if (eeID == EEID_GPS_LOCATIONERROR)
		PostMessage(rho_wmimpl_get_mainwnd(), PB_GEOLOCERROR, 0, 0);
	return S_OK;

//    return SendMessage(rho_wmimpl_get_mainwnd(), WM_BROWSER_ONGPSDATA, (WPARAM)eeID, value);
}

LRESULT CRhoWKBrowserEngine::ProcessOnTopMostWnd(HWND hWnd, int tabIndex)
{
    LOG(INFO) + "ProcessOnTopMostWnd";

	
	if(tabIndex)return S_OK;//only register on the first tab created, which will be 0 
		
	
	if(!m_pEngine->RegisterMessageHandler(&WK_WMMsgHandler)){
		return S_FALSE;
	}
	if(!m_pEngine->RegisterPrimaryMessageHandler(&WK_WMMsgHandler)){
		return S_FALSE;
	}
	if(!m_pEngine->RegisterForEvent(EEID_ENGINE_EXIT,WK_ExitHandler)){
		return S_FALSE;
	}
	
	if(!m_pEngine->RegisterForEvent(EEID_METATAG,onMeta)){
		return S_FALSE;
	}
	if(!m_pEngine->RegisterForEvent(EEID_DOCUMENTCOMPLETE,onNavEvent)){
		return S_FALSE;
	}
	if(!m_pEngine->RegisterForEvent(EEID_NAVIGATECOMPLETE,onNavEvent)){
		return S_FALSE;
	}
	if(!m_pEngine->RegisterForEvent(EEID_BEFORENAVIGATE,onNavEvent)){
		return S_FALSE;
	}	

	if(!m_pEngine->RegisterForEvent(EEID_TITLECHANGE,onNavEvent)){
		return S_FALSE;
	}

	//Fired when the page load takes longer than the value specified by 'NavTimeout' in the config file
	if(!m_pEngine->RegisterForEvent(EEID_NAVIGATIONTIMEOUT,onNavEvent)){
		return S_FALSE;
	}
	//  Fired when the engine indicates an editable field has been clicked on
	if (!m_pEngine->RegisterForEvent(EEID_SETSIPSTATE, onFieldFocus))
	{
	}
	//  Fired when the engine gives us a console log message (JS error or window.console.log)
	if (!m_pEngine->RegisterForEvent(EEID_CONSOLEMESSAGE, onConsoleMessage))
	{
	}

	if (!(m_pEngine->RegisterForEvent(EEID_GPS_POSITIONRECEIVED, OnGeolocationData) && 
		m_pEngine->RegisterForEvent(EEID_GPS_LOCATIONERROR, OnGeolocationData)))
	{
		return S_FALSE;
	}

	//Fired if there is an error navigating to the page, for example attempting to navigate to http://www.symbol.com if the device does not have a network connection.
	if(!m_pEngine->RegisterForEvent(EEID_NAVIGATIONERROR,onNavEvent)){
		return S_FALSE;
	}

    return S_OK;
}

LRESULT CRhoWKBrowserEngine::OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int iTabID;
	LPTSTR pStr;

    bHandled = TRUE;

    switch (uMsg) 
    {
		case PB_DEFTAGS:
            //EV: do you need PB_DEFTAGS notification?
            //Darryn Campbell: No, if we're reading the configuration file we'll fire the PB_DEFTAGS after the page has loaded ourselves
			break;
		
		case PB_ONMETA:
			{
				RHODESAPP().getExtManager().onSetPropertiesData( (LPCWSTR)wParam, (LPCWSTR)lParam );
				EngineMETATag* metaTag2 = (EngineMETATag*)lParam;
				if (metaTag2 && wcscmp(metaTag2->tcHTTPEquiv, L"initialiseRhoElementsExtension") != 0)
				{
					free(metaTag2->tcHTTPEquiv);
					free(metaTag2->tcContents);
					delete metaTag2;
				}
			}
			break;
		case PB_NAVIGATETAB:
			
			pStr = (LPTSTR)lParam;
			iTabID = (int)wParam;
			
			//  Remove spaces after 'JavaScript:'
			if (_memicmp(pStr, L"JavaScript: ", 12 * sizeof(TCHAR)) == 0 ||
				_memicmp(pStr, L"JavaScript:\t", 12 * sizeof(TCHAR)) == 0)
			{
				//  There are spaces to remove
				int iJSLen = _tcslen(L"JavaScript:");
				while(*(pStr + iJSLen) == L' ' || *(pStr + iJSLen) == L'\t')
				{
					//  +1 for space, +1 for NULL terminator
					wmemmove(pStr + iJSLen, pStr + iJSLen + 1, wcslen(pStr + iJSLen + 1) + 1);
				}
			}

			if(_memicmp(pStr,L"JavaScript:",11*2) == 0){
				if(m_pEngine->JavaScript_Exist(iTabID,pStr)){
					return m_pEngine->JavaScriptInvoke(iTabID,pStr);
				}
			}
			else{
                   LOG(INFO) + "PB_NAVIGATETAB :" + pStr;
                   return m_pEngine->NavigateOnTab(pStr,iTabID);
			}
		
			break;
		
		case PB_GEN_ACTIVEX:
            RHODESAPP().getExtManager().onSetPropertiesData( (LPCWSTR)wParam, (LPCWSTR)lParam );
			break;
		case PB_GEN_QUIT://currently quits all instances of PB
			//DeInitialise();
			break;
		case PB_SETPLG_PROPERTY:
			break;
		case PB_CALLPLG_METHOD:
			break;
		case PB_ONTOPMOSTWINDOW:
        {
            LOG(INFO) + "START PB_ONTOPMOSTWINDOW";
			/*return*/ CRhoWKBrowserEngine::ProcessOnTopMostWnd((HWND)lParam,(int)wParam); 
			//LRESULT rtRes = g_pAppManager->ProcessOnTopMostWnd((HWND)lParam,(int)wParam); 
            LRESULT rtRes = rho_wm_appmanager_ProcessOnTopMostWnd(wParam, lParam);
			//if(rtRes == S_FALSE){
			//	PostMessage(PB_GEN_QUIT,0,0);//we have not successfully processed the topMostWindow so shutdown
			//}
            LOG(INFO) + "END PB_ONTOPMOSTWINDOW";
			return rtRes;
            break;
        }		
		case PB_WINDOW_RESTORE:
			//  The window has been restored 
			//BrowserRestore(0, NULL);
	        ShowWindow(m_pEngine->GetHTMLWND(RHODESAPP().getExtManager().getCurrentTabID()), SW_RESTORE);
	        //  Hide the Start Bar
	        //PBScreenMode(g_bFullScreen, FALSE);
	        SetForegroundWindow(m_pEngine->GetHTMLWND(RHODESAPP().getExtManager().getCurrentTabID()));
	        EnableWindow(m_pEngine->GetHTMLWND(RHODESAPP().getExtManager().getCurrentTabID()), TRUE);

            RHODESAPP().getExtManager().OnWindowChanged((LPVOID) PB_WINRESTORED);
            
			break;
		case PB_SCREEN_ORIENTATION_CHANGED:
            RHODESAPP().getExtManager().OnWindowChanged((LPVOID) lParam);
			//g_bLeftRotated = wParam == TYPEHERE?TRUE:FALSE;//update the global
			//use a thread so that we don't hold up the message pump
			//CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) lParam, 0, 0));
			break;
		
		case PB_NEWGPSDATA:
			//Notifies the web engine when there are new location data available
			m_pEngine->SendLocationDatoToEngine();
			break;

		case PB_GEOLOCERROR:
			m_pEngine->SendLocationErrorToEngine();
			break;

		case PB_REGISTER_MESSAGE:
			m_pEngine->RegisterForMessage((UINT)lParam);
			break;

		case PB_DEREGISTER_MESSAGE:
			m_pEngine->DeRegisterForMessage((UINT)lParam);
			break;

    }

    return 0;
}

void CRhoWKBrowserEngine::RunMessageLoop(CMainWindow& mainWnd)
{
	//tPB_PreprocessMessage lpPreprocessMessage = m_pEngine->GetlpPreprocessMessage();

	//LRESULT preProcRes;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        //In the case of webkit inprocess, On keydown, it invokes RunMessageLoop followed by  WK_WMMsgHandler.     
        //In the case of webkit outprocess, whenever Ehkio window has the focus, it invokes only WK_WMMsgHandler.
        //But whenever webkit loses focus and MainWindow gets the focus, WM_KEYDOWN message will invoke RunMessageLoop and 
        //it won't invoke WK_WMMsgHandler
        //We have lots of logic sucha as camera logic inside WK_WMMsgHandler, so we have to explicitly 
        //invoke this method if focus is with MainWindow
        if(RHODESAPP().isWebkitOutofProcess())
        {
            //we reach here for outproc, WK_WMMsgHandler internally handover the message to extention manager
            WK_WMMsgHandler(0,&msg);
        }
        else
        {
        	if((WM_KEYDOWN == msg.message)||((WM_KEYUP == msg.message))||((WM_CHAR == msg.message)))
			{
				if(msg.wParam==VK_BACK)
				{
					HWND checkCurrentWindow = GetActiveWindow();
					HWND checkIsParentHWND = CRhoWKBrowserEngine::getInstance()->GetParentHWND();
					if(!g_EditableControlHasFocus && (checkCurrentWindow == checkIsParentHWND))
					{
						continue;	
					}
				}
				if(g_EditableControlHasFocus)
				{
					HWND checkFocusWindow = GetFocus();
					if(m_hEditableWindow)
					{
						if(m_hEditableWindow != checkFocusWindow)
						{
							SetFocus(m_hEditableWindow);
						}
					}
				}
			}
				
			//we reach here, only in the case of inproc
			if( RHODESAPP().getExtManager().onWndMsg(msg) )
				continue;
        }


	    if (!TranslateAccelerator(msg.hwnd, 0, &msg))
        //if (!mainWnd.TranslateAccelerator(&msg))
	    {
		    TranslateMessage(&msg);
		    DispatchMessage(&msg);
	    }		
    }
}

void CRhoWKBrowserEngine::SetCookie(char* url, char* cookie)
{
    // can't implement: no webkit method exposed!
}

CEng* CRhoWKBrowserEngine::getWebKitEngine() const
{
    return m_pEngine;
}

void CRhoWKBrowserEngine::OnDocumentComplete(LPCTSTR url)
{
}

}

rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance)
{
    return rho::CRhoWKBrowserEngine::getInstance(hwndParent, rhoAppInstance);
}

