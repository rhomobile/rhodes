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

#include "common/ExtManager.h"
#include "rubyext/WebView.h"
#include "common/RhodesApp.h"

#ifndef RHODES_QT_PLATFORM
#include "MainWindow.h"
#include "Utils.h"
#endif

#include "common/app_build_capabilities.h"

#ifdef RHODES_QT_PLATFORM
#include "../../../../shared/qt/rhodes/impl/MainWindowImpl.h"
#endif

#ifndef RHODES_QT_PLATFORM
extern CMainWindow& getAppWindow();
//#define IDM_NAVIGATE                    40022
//#define IDM_EXECUTEJS                   40033
//#define IDM_STOPNAVIGATE                40034
//#define IDM_ZOOMPAGE                    40035
//#define IDM_ZOOMTEXT                    40036
extern "C" HWND getMainWnd();
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
#endif
extern "C" void rho_sys_app_exit();
#ifndef RHODES_QT_PLATFORM
extern "C" WCHAR* rho_wmimpl_get_configfilepath();
extern "C" const wchar_t* rho_wmimpl_sharedconfig_getvalue(const wchar_t* szName);
#endif

namespace rho {
namespace common {

IMPLEMENT_LOGCLASS(CExtManager, "ExtManager");

CZoomKeyDataType::CZoomKeyDataType()
{
#ifndef RHODES_QT_PLATFORM
	RetrieveZoomInZoomOutKeyInfo();
#endif
}

void CZoomKeyDataType::RetrieveZoomInZoomOutKeyInfo()
{
#ifndef RHODES_QT_PLATFORM
	LPCTSTR pZoomKeyInVal, pZoomKeyOutVal;
	//  By default, we will block the Zoom In or Zoom Out Function Key to process further.
	isKeyBlockingRequired = true;

	//Read the ZoomInKey value from config.xml
	pZoomKeyInVal = rho_wmimpl_sharedconfig_getvalue(L"ZoomKey\\ZoomInKey");
	if(pZoomKeyInVal)
	{
		iZoomIn = _httoi(pZoomKeyInVal);
		if( iZoomIn >= VK_F1 && iZoomIn <= VK_F24 )
		{
			LOG(INFO) + "Valid ZoomInKey set in config.xml.";
			isValidZoomKeyIn = true;
		}
		else
		{
			LOG(ERROR) + "Invalid ZoomInKey set in config.xml.";
			isValidZoomKeyIn = false;
		}
	}

	//Read the ZoomOutKey value from config.xml
	pZoomKeyOutVal = rho_wmimpl_sharedconfig_getvalue(L"ZoomKey\\ZoomOutKey");
	if(pZoomKeyOutVal)
	{
		iZoomOut = _httoi(pZoomKeyOutVal);
		if( iZoomOut >= VK_F1 && iZoomOut <= VK_F24 )
		{
			LOG(INFO) + "Valid ZoomOutKey set in config.xml.";
			isValidZoomKeyOut = true;
		}
		else
		{
			LOG(ERROR) + "Invalid ZoomOutKey set in config.xml.";
			isValidZoomKeyOut = false;
		}
	}
	if( iZoomIn == iZoomOut)
	{
		 LOG(ERROR) + "Same value set for ZoomInKey & ZoomOutKey in config.xml. Please set different function key value for ZoomInKey & ZoomOutKey in config.xml.";
		 isSameZoomValueSet = true;
		 // Since Keys for both Zoom In & Zoom Out is same.
		 // We will not do zoom in or zoom out but we will
		 // not block the key to process further.
		 isKeyBlockingRequired = false;
	}
	else
	{
		LOG(INFO) + "Different function key value is set for ZoomInKey & ZoomOutKey in config.xml.";
		isSameZoomValueSet = false;
	}
#endif
}


void CExtManager::registerExtension(const String& strName, IRhoExtension* pExt)
{
    m_hashExtensions.put(strName, pExt);
}

IRhoExtension* CExtManager::getExtByName(const String& strName)
{
    return m_hashExtensions.get(strName);
}

CRhoExtData CExtManager::makeExtData()
{
    CRhoExtData oData;
#ifndef RHODES_QT_PLATFORM
    oData.m_hWnd = getMainWnd();
    oData.m_hInstance = rho_wmimpl_get_appinstance();
#else
    oData.m_hWnd = 0;
    oData.m_hInstance = 0;
#endif
    oData.m_iTabIndex = rho_webview_active_tab();
#if !defined(OS_WINDOWS_DESKTOP)
	if(getAppWindow().getWebKitEngine())
		oData.m_hBrowserWnd = getAppWindow().getWebKitEngine()->GetHTMLWND(oData.m_iTabIndex);
#endif
    
    return oData;
}

void CExtManager::onSetPropertiesData( const wchar_t* pPropID, const wchar_t* pData)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->onSetPropertiesData( pPropID, pData, makeExtData() );
    }
}

void CExtManager::onUnhandledProperty( const wchar_t* pModuleName, const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData )
{
    rho::common::IRhoExtension* pExt = getExtByName( rho::common::convertToStringA(pModuleName) );
    if (pExt)
        pExt->onSetProperty( pName, pValue, oExtData );
}

void CExtManager::onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo)
{
    #ifdef OS_WINCE
        m_oEngineEvntMngr.onBeforeNavigate(szUrlBeingNavigatedTo);//sabir: priority for DOMInjector
    #endif

    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->onBeforeNavigate( szUrlBeingNavigatedTo, makeExtData() );
    }
}

void CExtManager::onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->onNavigateComplete( szUrlBeingNavigatedTo, makeExtData() );
    }
}

void CExtManager::onDocumentComplete(const wchar_t* szUrlOfDocument)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->onDocumentComplete( szUrlOfDocument, makeExtData() );
    }
}

void CExtManager::close()
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
        delete it->second;

   m_hashExtensions.clear();
}

void CExtManager::executeRubyCallback( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse)
{
    RHODESAPP().callCallbackWithData(szCallback, szCallbackBody, szCallbackData, bWaitForResponse );
}

void CExtManager::executeRubyCallbackWithJsonBody( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse)
{
    RHODESAPP().callCallbackWithJsonBody(szCallback, szCallbackBody, szCallbackData, bWaitForResponse );
}

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );
unsigned long CExtManager::parseJsonToRubyHash(const char* szJson)
{
    char* szError = 0;
    unsigned long valBody = rjson_tokener_parse(szJson, &szError);
    if ( valBody != 0 )
        return valBody;

    LOG(ERROR) + "Incorrect json body.Error:" + (szError ? szError : "");
    if ( szError )
        free(szError);

    return rho_ruby_get_NIL();
}

void CExtManager::navigate(const wchar_t* szUrl)
{
    rho_webview_navigate( convertToStringA(szUrl).c_str(), -1 );
}

bool CExtManager::existsJavascript(const wchar_t* szJSFunction)
{
#if !defined(OS_WINDOWS_DESKTOP)
    return getAppWindow().getWebKitEngine()->isExistJavascript(szJSFunction, rho_webview_active_tab());
#else
    return true;
#endif
}

void CExtManager::setBrowserGesturing(bool bEnableGesturing)
{
#if !defined(OS_WINDOWS_DESKTOP)
    getAppWindow().getWebKitEngine()->setBrowserGesturing(bEnableGesturing);
#endif

}

void CExtManager::passSipPositionToEngine()
{
#if !defined(OS_WINDOWS_DESKTOP)
    getAppWindow().getWebKitEngine()->NotifyEngineOfSipPosition();
#endif
}

bool CExtManager::RegisterForBrowserMessage(unsigned int iMsgId)
{
#if !defined(OS_WINDOWS_DESKTOP)
    return getAppWindow().getWebKitEngine()->RegisterForMessage(iMsgId);
#else
	return true;
#endif
}

bool CExtManager::DeRegisterForBrowserMessage(unsigned int iMsgId)
{
#if !defined(OS_WINDOWS_DESKTOP)
    rho::IBrowserEngine* pEng = getAppWindow().getWebKitEngine();
    if (pEng)
        return pEng->DeRegisterForMessage(iMsgId);
    return false;
#else
	return true;
#endif
}

bool CExtManager::RegisterForPrimaryBrowserMessage(unsigned int iMsgId)
{
#if !defined(OS_WINDOWS_DESKTOP)
    return getAppWindow().getWebKitEngine()->RegisterForPrimaryMessage(iMsgId);
#else
	return true;
#endif
}

bool CExtManager::DeRegisterForPrimaryBrowserMessage(unsigned int iMsgId)
{
#if !defined(OS_WINDOWS_DESKTOP)
    rho::IBrowserEngine* pEng = getAppWindow().getWebKitEngine();
    if (pEng)
        return pEng->DeRegisterForPrimaryMessage(iMsgId);
    return false;
#else
	return true;
#endif
}

void CExtManager::executeJavascript(const wchar_t* szJSFunction)
{
    TNavigateData* nd = new TNavigateData();
    nd->index = rho_webview_active_tab();
#ifndef RHODES_QT_PLATFORM
    nd->url = _tcsdup(szJSFunction);
    ::SendMessage( getMainWnd(), WM_COMMAND, IDM_EXECUTEJS, (LPARAM)nd );
#else
    nd->url = wcsdup(szJSFunction);
    CMainWindow::getInstance()->executeJavaScriptCommand(nd);
#endif
}

StringW CExtManager::getCurrentUrl()
{
    return convertToStringW(RHODESAPP().getCurrentUrl(rho_webview_active_tab()));
}

void CExtManager::historyForward()
{
    rho_webview_navigate_forward();
}

void CExtManager::historyBack()
{
    rho_webview_navigate_back();
}

int CExtManager::getCurrentTabID()
{
    return rho_webview_active_tab();    
}

void CExtManager::refreshPage(bool bFromCache)
{
    rho_webview_refresh(rho_webview_active_tab());
}

void CExtManager::stopNavigate()
{
#ifndef RHODES_QT_PLATFORM
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_STOPNAVIGATE, (LPARAM)rho_webview_active_tab() );
#endif
}

void CExtManager::quitApp()
{
    rho_sys_app_exit();
}

#ifndef RHODES_QT_PLATFORM
static void __minimize_restoreApp(int nParam)
{
    ::ShowWindow(getMainWnd(), nParam );
    SetForegroundWindow(getMainWnd());
}
#endif

void CExtManager::minimizeApp()
{
#ifndef RHODES_QT_PLATFORM
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
    rho_callInUIThread(__minimize_restoreApp, SW_MINIMIZE);
#else
    ::PostMessage( getMainWnd(), WM_WINDOW_MINIMIZE, 0, 0 );
#endif
#else
    CMainWindow::getInstance()->minimizeWindowCommand();
#endif
}

void CExtManager::restoreApp()
{
#ifndef RHODES_QT_PLATFORM
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
    rho_callInUIThread(__minimize_restoreApp, SW_RESTORE);
#else
    rho_callInUIThread(__minimize_restoreApp, SW_SHOW);
#endif
#else
    CMainWindow::getInstance()->restoreWindowCommand();
#endif
}

void CExtManager::resizeBrowserWindow(RECT rc)
{
#ifndef RHODES_QT_PLATFORM
    //::MoveWindow( getMainWnd(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE );
#else
    CMainWindow::getInstance()->setFrame(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
#endif
}

void CExtManager::zoomPage(float fZoom)
{
#ifndef RHODES_QT_PLATFORM
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_ZOOMPAGE, (LPARAM)new CRhoFloatData(fZoom) );
#endif
}

INT CExtManager::onZoomTextWndMsg(MSG& oMsg)
{
	// Creating zoomKeyVal object of type CZoomKeyDataType
	// This object is used for Zoom In & Zoom Out Operation
#ifndef RHODES_QT_PLATFORM
	static CZoomKeyDataType zoomKeyVal;
#endif
	BOOL retVal = TRUE;
#ifndef RHODES_QT_PLATFORM
	if( ( ( oMsg.message == WM_KEYDOWN )&& (oMsg.wParam >= VK_F1) && ( oMsg.wParam <= VK_F24 ) ) && !zoomKeyVal.isSameZoomValueSet )
	{
		int getZoomVal = getTextZoom(); // Get the last text zoom value of an html page
		int setZoomVal;
		
		// Check & set whether to block the key.
		// However if the  function key is not valid the 
		// function key will not be blocked.
		if( zoomKeyVal.isKeyBlockingRequired )
		{
			retVal = FALSE;
		}

		if ( ( zoomKeyVal.isValidZoomKeyIn ) && (oMsg.wParam == zoomKeyVal.iZoomIn ) )
		{
			// Zoom In the text of an html page
			if( getZoomVal < 4 ) //Zoom Value range is in between 0 to 4
			{
				setZoomVal = getZoomVal + 1;
				zoomText(setZoomVal);
			}
		}
		else if( ( zoomKeyVal.isValidZoomKeyOut ) && (oMsg.wParam == zoomKeyVal.iZoomOut ) )
		{
			// Zoom Out the text of an html page
			if( getZoomVal > 0 ) //Zoom Value range is in between 0 to 4
			{
				setZoomVal = getZoomVal - 1;
				zoomText(setZoomVal);
			}
		}
		else
		{
			// It will reach here only if the clicked key is not valid function key
			// We will not block the key as any one of the functionality
			// i.e. Zoom In or Zoom Out is invalid function key
			retVal = TRUE;
		}
	}
#endif
	return retVal;
}

void CExtManager::zoomText(int nZoom)
{
#ifndef RHODES_QT_PLATFORM
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_ZOOMTEXT, (LPARAM)nZoom );
#endif
}

int CExtManager::getTextZoom() //Enum (0 to 4)
{
#if !defined(OS_WINDOWS_DESKTOP)
    return getAppWindow().getWebKitEngine()->GetTextZoomOnTab(rho_webview_active_tab());
#else
    return 2;
#endif
}

StringW CExtManager::getConfigPath()
{
#if defined(APP_BUILD_CAPABILITY_SYMBOL)
    return rho_wmimpl_get_configfilepath();
#else
    return L"";
#endif
}

StringW CExtManager::getPageTitle(UINT iTab)
{
#if !defined(OS_WINDOWS_DESKTOP)
    wchar_t szBuf[1025];
    szBuf[0]=0;
    getAppWindow().getWebKitEngine()->GetTitleOnTab( szBuf, 1024, iTab );    
    return szBuf ? szBuf : L"";
#else
    return L"";
#endif
}

extern "C" int rho_wmimpl_is_loglevel_enabled(int nLogLevel);
extern "C" bool rho_wmimpl_get_is_version2();
void CExtManager::rhoLog(ELogExtLevels eLogLevel, const char* szModule, const char* szMsg, const char* szFile, int nLine)
{
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
    bool bRE1App = false;

    if (!rho_wmimpl_get_is_version2())
        bRE1App = true;

    if (bRE1App && !rho_wmimpl_is_loglevel_enabled(eLogLevel))
        return;
#endif


    int nSeverity = L_INFO;
    switch (eLogLevel)
    {
    case eLogError:
	    nSeverity = L_ERROR;
	    break;
    case eLogWarning:
        nSeverity = L_WARNING;
	    break;
    case eLogInfo:
        nSeverity = L_INFO;
	    break;
    case eLogUser:
        nSeverity = L_INFO;
	    break;
    case eLogDebug:
        nSeverity = L_TRACE;
	    break;
    }

    rhoPlainLog(szFile, nLine, nSeverity, szModule, szMsg);
    
}

bool CExtManager::onWndMsg(MSG& oMsg)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        if ( (it->second)->onWndMsg( oMsg ) )
            return true;
    }

    return false;
}

bool CExtManager::onHTMLWndMsg(MSG& oMsg)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        if ( (it->second)->onHTMLWndMsg( oMsg ) )
            return true;
    }

    return false;
}

long CExtManager::OnNavigateTimeout(const wchar_t* szUrlBeingNavigatedTo)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnNavigateTimeout( szUrlBeingNavigatedTo, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}

long CExtManager::OnSIPState(bool bSIPState)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnSIPState( bSIPState, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}

long CExtManager::OnAlertPopup(int nEnum, void* pData)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnAlertPopup( nEnum, pData, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}

long CExtManager::OnAuthenticationRequest(int nEnum, void* pData)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnAuthenticationRequest( nEnum, pData, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}

long CExtManager::OnGeolocationData(int nEnum, void* pData)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnGeolocationData( nEnum, pData, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}

long CExtManager::OnNavigateError(const wchar_t* szUrlBeingNavigatedTo)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnNavigateError( szUrlBeingNavigatedTo, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}

long CExtManager::OnLicenseError(const wchar_t* szUrlBeingNavigatedTo)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        long lRes = (it->second)->OnLicenseError( szUrlBeingNavigatedTo, makeExtData() );
        if ( lRes )
            return lRes;
    }

    return 0;
}
void CExtManager::OnLicenseScreen(bool bActivate)
{
	for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
	{
		(it->second)->OnLicenseScreen( bActivate, makeExtData() );
	}
}
void CExtManager::OnCreateShortcutViaXML(bool bIsWMDevice)
{
	for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
	{
		(it->second)->OnCreateShortcutViaXML(bIsWMDevice);
	}
}
void CExtManager::OnQuittingTheApplication()
{
	for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
	{
		(it->second)->OnQuittingTheApplication();
	}
}
void CExtManager::OnAppActivate(bool bActivate)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->OnAppActivate( bActivate, makeExtData() );
    }
}
void CExtManager::OnPowerButton(bool bPowerOn)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->OnPowerButton( bPowerOn, makeExtData() );
    }
}

void CExtManager::OnWindowChanged(LPVOID lparam)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->OnWindowChanged( lparam );
    }
}

} //namespace common
} //namespace rho
