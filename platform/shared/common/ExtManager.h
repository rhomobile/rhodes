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

#ifndef __EXT_MANAGER__H__
#define __EXT_MANAGER__H__

#ifdef __cplusplus

#include "logging/RhoLog.h"

#if defined(WINDOWS_PLATFORM)

namespace rho {
namespace common {

struct CRhoExtData
{
	HWND	    m_hWnd;
	void*		m_hInstance;
	int			m_iTabIndex;
    HWND       m_hBrowserWnd;
};

struct IRhoExtension 
{
    virtual ~IRhoExtension(){}
    virtual void onSetPropertiesData( const wchar_t* pPropID, const wchar_t* pData, const CRhoExtData& oExtData ){}
    virtual void onSetProperty( const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData ){}
    virtual void onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData){}
    virtual void onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData){}
    virtual void onDocumentComplete(const wchar_t* szUrlOfDocument, const CRhoExtData& oExtData){}
    virtual bool onWndMsg(MSG& oMsg){return false;}
    virtual long OnNavigateTimeout(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData){return 0;}
    virtual long OnSIPState(bool bSIPState, const CRhoExtData& oExtData){return 0;}
    virtual long OnAlertPopup(int nEnum, void* pData, const CRhoExtData& oExtData){return 0;}
    virtual long OnAuthenticationRequest(int nEnum, void* pData, const CRhoExtData& oExtData){return 0;}
    virtual long OnGeolocationData(int nEnum, void* pData, const CRhoExtData& oExtData){return 0;}
    virtual long OnNavigateError(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData){return 0;}
    virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData){}
};

struct IRhoExtManager
{
    virtual ~IRhoExtManager(){}

    virtual void onUnhandledProperty( const wchar_t* pModuleName, const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData ) = 0;
    virtual void executeRubyCallback( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse) = 0;
    virtual void executeRubyCallbackWithJsonBody( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse) = 0;

    virtual void requireRubyFile( const char* szFilePath ) = 0;
    virtual unsigned long parseJsonToRubyHash(const char* szJson)=0;

    virtual void navigate(const wchar_t* szUrl) = 0;
    virtual bool existsJavascript(const wchar_t* szJSFunction) = 0;
    virtual void executeJavascript(const wchar_t* szJSFunction) = 0;

    virtual void rhoLog(int nSeverity, const char* szModule, const char* szMsg, const char* szFile, int nLine) = 0;
    virtual StringW getCurrentUrl() = 0;
    virtual void stopNavigate() = 0;
    virtual void historyForward() = 0;
    virtual void historyBack() = 0;
    virtual void refreshPage(bool bFromCache) = 0;
    virtual void quitApp() = 0;
    virtual void minimizeApp() = 0;
    virtual void restoreApp() = 0;
    virtual void resizeBrowserWindow(RECT rc) = 0;
    virtual void zoomPage(float fZoom) = 0;
    virtual void zoomText(int nZoom) = 0;
    virtual int getTextZoom() = 0; //Enum (0 to 4)
    virtual StringW getPageTitle(UINT iTab) = 0;

    virtual StringW getConfigPath() = 0;
};

class CExtManager : public IRhoExtManager
{
	DEFINE_LOGCLASS;

	HashtablePtr<String, IRhoExtension*> m_hashExtensions;

public:
    void registerExtension(const String& strName, IRhoExtension* pExt);

    IRhoExtension* getExtByName(const String& strName);

    void onSetPropertiesData( const wchar_t* pPropID, const wchar_t* pData);
    void onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo);
    void onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo);
    void onDocumentComplete(const wchar_t* szUrlOfDocument);
    bool onWndMsg(MSG& oMsg);
    long OnNavigateTimeout(const wchar_t* szUrlBeingNavigatedTo);
    long OnSIPState(bool bSIPState);
    long OnAlertPopup(int nEnum, void* pData);
    long OnAuthenticationRequest(int nEnum, void* pData);
    long OnGeolocationData(int nEnum, void* pData);
    long OnNavigateError(const wchar_t* szUrlBeingNavigatedTo);
    void OnAppActivate(bool bActivate);

    CRhoExtData makeExtData();
    void close();

    //IRhoExtManager
    virtual void onUnhandledProperty( const wchar_t* pModuleName, const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData );
    virtual void executeRubyCallback( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse);
    virtual void executeRubyCallbackWithJsonBody( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse);

    virtual void navigate(const wchar_t* szUrl);
    virtual void requireRubyFile( const char* szFilePath );
    virtual unsigned long parseJsonToRubyHash(const char* szJson);

    virtual void executeJavascript(const wchar_t* szJSFunction);
    virtual bool existsJavascript(const wchar_t* szJSFunction);
    virtual void rhoLog(int nSeverity, const char* szModule, const char* szMsg, const char* szFile, int nLine);
    virtual StringW getCurrentUrl();
    virtual void stopNavigate();
    virtual void historyForward();
    virtual void historyBack();
    virtual void refreshPage(bool bFromCache);
    virtual void quitApp();
    virtual void minimizeApp();
    virtual void restoreApp();
    virtual void resizeBrowserWindow(RECT rc);
    virtual void zoomPage(float fZoom);
    virtual void zoomText(int nZoom);
    virtual int getTextZoom(); //Enum (0 to 4)
    virtual StringW getPageTitle(UINT iTab);
    virtual StringW getConfigPath();
};

} //namespace common
} //namespace rho

#else //WINDOWS_PLATFORM
class CExtManager
{
public:
    void close(){}
};

#endif

#endif //__cplusplus

#endif //__EXT_MANAGER__H__
