#include "stdafx.h"
#include "common/app_build_capabilities.h"

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER

#include "RhoWKBrowserEngine.h"
#include "MainWindow.h"

#include "rhoelements/PBCore/PBCore/Eng.h"

extern "C" HWND rho_wmimpl_get_mainwnd();

namespace rho
{
WNDPROC CRhoWKBrowserEngine::m_WebKitOwnerProc;
CConfig* CRhoWKBrowserEngine::g_pConfig = new CConfig();
WCHAR CRhoWKBrowserEngine::g_szConfigFilePath[MAX_PATH + 1];
WCHAR CRhoWKBrowserEngine::g_szInstallDirectory[MAX_PATH + 1];

LRESULT CALLBACK onTopmostWnd(EngineEventID eeID, LPARAM value, int tabIndex)
{
	// The engine needs to navigate to a page before it can provide the topmost window.
	// We cannot call preload until we have a valid window handle
	// If all of the instances have been loaded then the meta event and navigate event will be registered 
	
	return PostMessage(rho_wmimpl_get_mainwnd(),PB_ONTOPMOSTWINDOW,(LPARAM)tabIndex,(WPARAM)value);
}

CRhoWKBrowserEngine::CRhoWKBrowserEngine(HWND hParentWnd, HINSTANCE hInstance) : m_pEngine(NULL)
{
    if(GetModuleFileName(NULL, g_szInstallDirectory, MAX_PATH)){
        //trim the file name off the end
        WCHAR* pStr = wcsrchr(g_szInstallDirectory,L'\\');
        if(pStr) *(pStr+1) = NULL;
    }
 	wcscpy(g_szConfigFilePath,g_szInstallDirectory);
	wcscat(g_szConfigFilePath,L"Config/Config.xml");
	if((g_pConfig->Init(g_szConfigFilePath))==NULL)
	{
		WCHAR* szConfigErrorMsg = new WCHAR[MAX_PATH + 40];
		wsprintf(szConfigErrorMsg, L"Please check your config.xml (%s)", g_szConfigFilePath);
		MessageBox(NULL,szConfigErrorMsg, L"Config Error", MB_OK);
		delete[] szConfigErrorMsg;
	}
    m_pEngine = new CEng(hParentWnd, hInstance);
    if (m_pEngine->Init(L"PBEngine_WK.dll")) 
    {
        m_pEngine->InitEngine(0, &WK_HTMLWndProc, &m_WebKitOwnerProc, SETTING_OFF, &WK_GetEngineConfig);
        m_pEngine->RegisterForEvent(EEID_TOPMOSTHWNDAVAILABLE, onTopmostWnd);
    }
}

CRhoWKBrowserEngine::~CRhoWKBrowserEngine(void)
{
    m_pEngine->DeInitEngine();
    //TODO: delete engine - now it crash when delete
    //delete m_pEngine;
}

BOOL CRhoWKBrowserEngine::Navigate(LPCTSTR szURL)
{
    return m_pEngine->Navigate(szURL);
}

HWND CRhoWKBrowserEngine::GetHTMLWND()
{
    return m_pEngine->GetHTMLWND(0);
}

BOOL CRhoWKBrowserEngine::ResizeOnTab(int iInstID,RECT rcNewSize)
{
    return m_pEngine->ResizeOnTab(iInstID,rcNewSize);
}

BOOL CRhoWKBrowserEngine::BackOnTab(int iInstID,int iPagesBack/* = 1*/)
{
    return m_pEngine->BackOnTab(iInstID,iPagesBack);
}

BOOL CRhoWKBrowserEngine::ForwardOnTab(int iInstID)
{
    return m_pEngine->ForwardOnTab(iInstID);
}

BOOL CRhoWKBrowserEngine::ReloadOnTab(bool bFromCache, UINT iTab)
{
    return m_pEngine->ReloadOnTab(bFromCache, iTab);
}

BOOL CRhoWKBrowserEngine::NavigateToHtml(LPCTSTR szHtml)
{
    //return m_pEngine->NavigateToHtml(szHtml);
    //TODO: NavigateToHtml

    return FALSE;
}

LRESULT CALLBACK CRhoWKBrowserEngine::WK_HTMLWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	// Pass message to original window procedure
	result = CallWindowProc (m_WebKitOwnerProc, hwnd, message, wParam, lParam);
	return result;

    //return DefWindowProc(hwnd, message, wParam, lParam);
    //return (message == 0xF) ? 1 : 0;
}

LRESULT CALLBACK CRhoWKBrowserEngine::WK_GetEngineConfig(int iInstID, LPCTSTR tcSetting, TCHAR* tcValue)
{
	LPCTSTR tcValueRead;
	tcValueRead = g_pConfig->GetAppSettingPtr(iInstID, tcSetting, L"Value");
	if (tcValueRead != NULL)
		wcscpy(tcValue, tcValueRead);
	else
		tcValue = NULL;
	return S_OK;
} 

LRESULT CALLBACK onNavEvent(EngineEventID eeID, LPARAM value, int tabIndex)
{
	BOOL bOk = FALSE;
	DWORD dwRes = WAIT_FAILED;
	
	switch(eeID)
	{
	case EEID_BEFORENAVIGATE:
        break;
    case EEID_NAVIGATECOMPLETE:
        break;
    case EEID_DOCUMENTCOMPLETE:
        SendMessage(rho_wmimpl_get_mainwnd(), WM_BROWSER_ONDOCUMENTCOMPLETE, 0, value);
        break;
    case EEID_NAVIGATIONERROR:
        break;
    case EEID_NAVIGATIONTIMEOUT:
        break;

    }

    return S_OK;
}

LRESULT CALLBACK onMeta(EngineEventID eeID, LPARAM value, int tabIndex)
{
    return S_OK;
}

LRESULT CALLBACK onFieldFocus(EngineEventID eeID, LPARAM value, int tabIndex)
{
    return S_OK;
}

LRESULT CALLBACK onConsoleMessage(EngineEventID eeID, LPARAM value, int tabIndex)
{
    return S_OK;
}

LRESULT CRhoWKBrowserEngine::ProcessOnTopMostWnd(HWND hWnd, int tabIndex)
{
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

	//Fired if there is an error navigating to the page, for example attempting to navigate to http://www.motorola.com if the device does not have a network connection.
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
/*			iTabID = (int)wParam;
			g_pTargetApp = g_pAppManager->GetApp(iTabID);
			if(g_pTargetApp){
				g_pTargetApp->RunDefaultMetaTags();
			}*/
			break;
		
		case PB_ONMETA:
/*			iTabID = (int)wParam;
			g_pTargetApp = g_pAppManager->GetApp(iTabID);
			if(g_pTargetApp){
				metaTag = (EngineMETATag*)lParam;
				g_pTargetApp->m_pbStruct.bInvoked = FALSE;
				g_pTargetApp->m_pEMML->ParseEMMLTag(metaTag->tcHTTPEquiv,metaTag->tcContents,TagProc);
				return S_OK; 
			}
			return S_FALSE; 
*/			

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
                   return m_pEngine->NavigateOnTab(pStr,iTabID);
			}
		
			break;
		
		case PB_GEN_ACTIVEX:
			// GD - don't attempt to call plugins while shutting down
/*			if (g_bShuttingDown)
				break;

			pEquiv = (LPWSTR) wParam;
			pContent = (LPWSTR)lParam;
			
			pApp = g_pAppManager->GetApp(0);
			if(pApp){
				pApp->m_pbStruct.bInvoked = TRUE;
				pApp->m_pEMML->ParseEMMLTag(pEquiv,pContent,TagProc);
			}*/
			
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
            return ProcessOnTopMostWnd((HWND)lParam,(int)wParam); 
			//LRESULT rtRes = g_pAppManager->ProcessOnTopMostWnd((HWND)lParam,(int)wParam); 
			//if(rtRes == S_FALSE){
			//	PostMessage(PB_GEN_QUIT,0,0);//we have not successfully processed the topMostWindow so shutdown
			//}
			//return rtRes;
            //break;
        }		
		case PB_WINDOW_RESTORE:
			//  The window has been restored 
			//BrowserRestore(0, NULL);
	        ShowWindow(m_pEngine->GetHTMLWND(0), SW_RESTORE);
	        //  Hide the Start Bar
	        //PBScreenMode(g_bFullScreen, FALSE);
	        SetForegroundWindow(m_pEngine->GetHTMLWND(0));
	        EnableWindow(m_pEngine->GetHTMLWND(0), TRUE);

			break;
		case PB_SCREEN_ORIENTATION_CHANGED:
			//g_bLeftRotated = wParam == TYPEHERE?TRUE:FALSE;//update the global
			//use a thread so that we don't hold up the message pump
			//CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) lParam, 0, 0));
			break;
		
		case PB_NEWGPSDATA:
			//Notifies the web engine when there are new location data available
			//g_pEngine->SendLocationDatoToEngine();
			break;
    }

    return 0;
}

void CRhoWKBrowserEngine::RunMessageLoop(CMainWindow& mainWnd)
{
	tPB_PreprocessMessage lpPreprocessMessage = m_pEngine->GetlpPreprocessMessage();

	LRESULT preProcRes;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
	    preProcRes = ACCELERATE_KEY_HANDLE;
//		    if (!g_bShuttingDown)
		    preProcRes = lpPreprocessMessage(msg);
//		    else
//			    continue;
	    //  Allow the browser to process the accelerator keys (most notably Tab)
	    //  Do not preProcess the message if we are shutting down as the 
	    //  Engine DLL will have been deinitialised
	    if (//CLicense::GetLicenseScreenVisible() || 
		    ACCELERATE_KEY_DONT_HANDLE != preProcRes)
	    {
		    if (preProcRes != ACCELERATE_KEY_DONT_TRANSLATE //|| 
			    //CLicense::GetLicenseScreenVisible()
                )
		    {
			    //if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
                if (!mainWnd.TranslateAccelerator(&msg))
			    {
				    TranslateMessage(&msg);
				    DispatchMessage(&msg);
			    }
		    }
		    else
		    {
			    DispatchMessage(&msg);
		    }
	    }

    }
}

CEng* CRhoWKBrowserEngine::getWebKitEngine() const
{
    return m_pEngine;
}

}

#endif //APP_BUILD_CAPABILITY_WEBKIT_BROWSER
