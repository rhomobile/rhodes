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

#include "common/ExtManager.h"
#include "rubyext/WebView.h"
#include "common/RhodesApp.h"
#include "common/app_build_capabilities.h"
#include "MainWindowImpl.h"

//extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" void rho_sys_app_exit();
//extern "C" WCHAR* rho_wmimpl_get_configfilepath();

namespace rho {
namespace common {

IMPLEMENT_LOGCLASS(CExtManager, "ExtManager");

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
    oData.m_hWnd = 0;
    oData.m_hInstance = 0; // rho_wmimpl_get_appinstance()
    oData.m_iTabIndex = rho_webview_active_tab();
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
    return true;
}

void CExtManager::setBrowserGesturing(bool)
{
    // not implemented
}

void CExtManager::passSipPositionToEngine()
{
    // not implemented
}

bool CExtManager::RegisterForBrowserMessage(unsigned int iMsgId)
{
	return true;
}

bool CExtManager::DeRegisterForBrowserMessage(unsigned int iMsgId)
{
	return true;
}

bool CExtManager::RegisterForPrimaryBrowserMessage(unsigned int iMsgId)
{
	return true;
}

bool CExtManager::DeRegisterForPrimaryBrowserMessage(unsigned int iMsgId)
{
	return true;
}

void CExtManager::executeJavascript(const wchar_t* szJSFunction)
{
    TNavigateData* nd = new TNavigateData();
    nd->index = rho_webview_active_tab();
    nd->url = wcsdup(szJSFunction);
    CMainWindow::getInstance()->doNavigateCommand(nd);
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
    // not implemented
}

void CExtManager::quitApp()
{
    rho_sys_app_exit();
}

void CExtManager::minimizeApp()
{
    // not implemented
}

void CExtManager::restoreApp()
{
    // not implemented
}

void CExtManager::resizeBrowserWindow(RECT rc)
{
    CMainWindow::getInstance()->setFrame(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
}

void CExtManager::zoomPage(float)
{
    // not implemented
}

void CExtManager::zoomText(int)
{
    // not implemented
}

int CExtManager::getTextZoom() //Enum (0 to 4)
{
    return 2;
}

StringW CExtManager::getConfigPath()
{
    // rho_wmimpl_get_configfilepath()
    return L"";
}

StringW CExtManager::getPageTitle(UINT iTab)
{
    return L"";
}

void CExtManager::rhoLog(ELogExtLevels eLogLevel, const char* szModule, const char* szMsg, const char* szFile, int nLine)
{
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

void CExtManager::OnAppActivate(bool bActivate)
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->OnAppActivate( bActivate, makeExtData() );
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
