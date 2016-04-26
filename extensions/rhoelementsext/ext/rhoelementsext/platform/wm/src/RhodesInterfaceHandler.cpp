#include "RhodesInterfaceHandler.h"

CExtManager* g_pExtManager = NULL;

//  Initialisation

BOOL InitialiseRhoElementsExtension(HINSTANCE hInstance, HWND hWnd, HWND hWndWebKit, const wchar_t* szConfigFile, 
									   CExtManager* pExtManager)
{
	g_pExtManager = pExtManager;
	//  Initialise the map of EMML tags which are processed by the Rhodes framework
	int iCommandLineLength = wcslen(szConfigFile) + 1 + strlen("/C file://");
	WCHAR* wCommandLine = new WCHAR[iCommandLineLength];
	WCHAR* szConfigFileCpy = new WCHAR[iCommandLineLength];
	wcscpy(szConfigFileCpy, szConfigFile);
	memset(wCommandLine, 0, iCommandLineLength * sizeof(WCHAR));
	wcscpy(wCommandLine, L"/C file://");
	wcscat(wCommandLine, szConfigFileCpy);
//	wsprintf(wCommandLine, L"/C file://%s", szConfigFile);

	BOOL bRetVal = InitialiseREAsExtension(hInstance, hWnd, hWndWebKit,
		wCommandLine, NavigateCallback, ExistsJavascriptCallback, 
		InvokeJavascriptCallback, LogCallback, GPSControlCallback,
		HistoryForwardCallback, HistoryBackCallback,
		RefreshCallback, QuitCallback, MinimizeCallback, RestoreCallback,
		ResizeBrowserWindowCallback, ZoomPageCallback, ZoomTextCallback,
		GetTextZoomCallback, GetTitleCallback, SetBrowserGesturingCallback,
		PassSipPositionToEngineCallback, RegisterForMessageCallback,
		DeRegisterForMessageCallback, RegisterForPrimaryMessageCallback,
		DeRegisterForPrimaryMessageCallback);

	delete[] wCommandLine;
	delete[] szConfigFileCpy;
	return bRetVal;
}

void ShutDown()
{
	DeInitialise();
}


//  Callbacks to Rhodes and WebKit, these are provided as function pointers to PBCore during initialisation,
//  this is the glue code to connect Rhodes with the RhoElements extension.
BOOL NavigateCallback(const wchar_t* szUrl)
{
	if (szUrl && wcslen(szUrl) >= 1 && szUrl[0] != '/')
	{
		//  If we are trying to navigate to a URL just call the navigate method
		if (g_pExtManager)
			g_pExtManager->navigate(szUrl);
	}
	else
	{
		//  if the user has specified a Rhodes method callback  then call it here
		//  with the json object as a parameter.
		int iLengthNavString = wcslen(szUrl);
		int iLengthJSONArray = wcslen(szUrl + wcslen(szUrl) + 1);
		int iLengthCallback = iLengthNavString + iLengthJSONArray + 1;
		char* szUrlMbs = new char[iLengthCallback];
		memset(szUrlMbs, 0, iLengthCallback);
		wcstombs(szUrlMbs, szUrl, iLengthCallback);
		wcstombs(szUrlMbs + strlen(szUrlMbs) + 1, szUrl + wcslen(szUrl) + 1, 
			iLengthCallback - strlen(szUrlMbs) - 1);
		LogCallback(PB_LOG_INFO, L"JSON Callback", szUrl + wcslen(szUrl) + 1, szUrl, 0);
		g_pExtManager->executeRubyCallbackWithJsonBody(szUrlMbs, szUrlMbs + strlen(szUrlMbs) + 1, "", false);
		delete[] szUrlMbs;
//		RHODESAPP().callCallbackWithData(szUrlMbs, szBody, szUrlMbs + strlen(szUrlMbs) + 1, false);
	}
	return TRUE;
}

BOOL ExistsJavascriptCallback(const wchar_t* szJavascript)
{
	if (g_pExtManager)
		g_pExtManager->existsJavascript(szJavascript);
	return TRUE;
}

BOOL InvokeJavascriptCallback(const wchar_t* szJavascript)
{
	if (g_pExtManager)
		g_pExtManager->executeJavascript(szJavascript);
	return TRUE;
}

void LogCallback(int nSeverity, const wchar_t* szModule, const wchar_t* szMsg, const wchar_t* szFunctionName, int nLine)
{
	int iFunctionNameLength;
	if (szFunctionName)
		iFunctionNameLength = wcslen(szFunctionName);
	else
		iFunctionNameLength = wcslen(L"UserLog");
	//  Convert parameters to mbs
	int iModuleLength = (wcslen(szModule) + 1) + (iFunctionNameLength + 3) * 2;
	int iMsgLength = (wcslen(szMsg) + 1) * 2;
	char* szModuleData = new char[iModuleLength];
	wchar_t* szModuleDataWide = new WCHAR[iModuleLength];
	if (szFunctionName)
		wsprintf(szModuleDataWide, L"%s::%s", szModule, szFunctionName);
	else
		wsprintf(szModuleDataWide, L"%s::UserLog", szModule);
	wcstombs(szModuleData, szModuleDataWide, iModuleLength);
	char* szMsgData = new char[iMsgLength];
	wcstombs(szMsgData, szMsg, iMsgLength);
    IRhoExtManager::ELogExtLevels iRhodesLogSeverity;
	switch (nSeverity)
	{
	case PB_LOG_ERROR:
		iRhodesLogSeverity = IRhoExtManager::eLogError;
		break;
	case PB_LOG_WARNING:
        iRhodesLogSeverity = IRhoExtManager::eLogWarning;
		break;
	case PB_LOG_INFO:
        iRhodesLogSeverity = IRhoExtManager::eLogInfo;
		break;
	case PB_LOG_DEBUG:
        iRhodesLogSeverity = IRhoExtManager::eLogDebug;
		break;
	case PB_LOG_USER:
        iRhodesLogSeverity = IRhoExtManager::eLogUser;
		break;

	default:
		iRhodesLogSeverity = IRhoExtManager::eLogInfo;
	}
	
	if (g_pExtManager)
		g_pExtManager->rhoLog(iRhodesLogSeverity, szModuleData, szMsgData, "RhoElements Extension", nLine);
	delete[] szModuleData;
	delete[] szModuleDataWide;
	delete[] szMsgData;
	return;
}

void GPSControlCallback(bool bLocationInfo)
{

	//  TBD  //  TODO
	return;
}

void StopCallback()
{
	if (g_pExtManager)
		g_pExtManager->stopNavigate();
}

void HistoryForwardCallback()
{
	if (g_pExtManager)
		g_pExtManager->historyForward();
}

void HistoryBackCallback()
{
	if (g_pExtManager)
		g_pExtManager->historyBack();
}

void RefreshCallback(BOOL bReloadFromCache)
{
	if (g_pExtManager)
	{
		if (bReloadFromCache)
			g_pExtManager->refreshPage(true);
		else
			g_pExtManager->refreshPage(false);
	}
}

void QuitCallback()
{
	if (g_pExtManager)
		g_pExtManager->quitApp();
}

void MinimizeCallback()
{
	if (g_pExtManager)
		g_pExtManager->minimizeApp();
}

void RestoreCallback()
{
	if (g_pExtManager)
		g_pExtManager->restoreApp();
}

void ResizeBrowserWindowCallback(RECT rcSize)
{
	if (g_pExtManager)
		g_pExtManager->resizeBrowserWindow(rcSize);
}

void ZoomPageCallback(float fZoom)
{
	if (g_pExtManager)
		g_pExtManager->zoomPage(fZoom);
}

void ZoomTextCallback(int iZoom)
{
	if (g_pExtManager)
		g_pExtManager->zoomText(iZoom);
}

void GetTitleCallback(WCHAR* szTitle, int iMaxLength)
{
	if (g_pExtManager)
	{
		wcsncpy(szTitle, g_pExtManager->getPageTitle(0).c_str(), iMaxLength - 1);
	}
	else
		szTitle = L"";
}

int GetTextZoomCallback()
{
	if (g_pExtManager)
		return g_pExtManager->getTextZoom();
	else
		return 3;
}

void SetBrowserGesturingCallback(BOOL bSetBrowserGesturing)
{
	if (g_pExtManager)
	{
		if (bSetBrowserGesturing)	
			g_pExtManager->setBrowserGesturing(true);
		else
			g_pExtManager->setBrowserGesturing(false);
	}
}

void PassSipPositionToEngineCallback()
{
	if (g_pExtManager)
	{
		//  Call method in Extension manager to notify Rhodes that
		//  the SIP button is clicked
		g_pExtManager->passSipPositionToEngine();
	}
}

BOOL RegisterForMessageCallback(unsigned int iMsgId)
{
	if (g_pExtManager)
	{
		return g_pExtManager->RegisterForBrowserMessage(iMsgId);
	}
	return FALSE;
}

BOOL DeRegisterForMessageCallback(unsigned int iMsgId)
{
	if (g_pExtManager)
	{
		return g_pExtManager->DeRegisterForBrowserMessage(iMsgId);
	}
	return FALSE;
}

BOOL RegisterForPrimaryMessageCallback(unsigned int iMsgId)
{
	if (g_pExtManager)
	{
		return g_pExtManager->RegisterForPrimaryBrowserMessage(iMsgId);
	}
	return FALSE;
}

BOOL DeRegisterForPrimaryMessageCallback(unsigned int iMsgId)
{
	if (g_pExtManager)
	{
		return g_pExtManager->DeRegisterForPrimaryBrowserMessage(iMsgId);
	}
	return FALSE;
}

//  Direct calls coming from Rhodes, mimic the same inputs as were present in
//  RhoElements executable
BOOL InjectMetaTag(LPWSTR szHttpEquiv, LPWSTR szContents)
{
	EngineMETATag tag;
	tag.tcHTTPEquiv = szHttpEquiv;
	tag.tcContents = szContents;
	return SendRhoElementsOneMessage(PB_ONMETA, (WPARAM)0, (LPARAM)&tag, true);
}

BOOL BeforeNavigateHandler(const wchar_t* szURL)
{
//	LogCallback(PB_LOG_INFO, L"RhoElementsExtension", szURL, L"BeforeNavigate", __LINE__);
	return onNavEvent(EEID_BEFORENAVIGATE, (LPARAM)szURL, 0) == S_OK;
}

BOOL NavigateCompleteHandler(const wchar_t* szURL)
{
//	LogCallback(PB_LOG_INFO, L"RhoElementsExtension", szURL, L"NavigateComplete", __LINE__);
	return onNavEvent(EEID_NAVIGATECOMPLETE, (LPARAM)szURL, 0) == S_OK;
}

BOOL DocumentCompleteHandler(const wchar_t* szURL)
{
//	LogCallback(PB_LOG_INFO, L"RhoElementsExtension", szURL, L"DocumentComplete", __LINE__);
	return onNavEvent(EEID_DOCUMENTCOMPLETE, (LPARAM)szURL, 0) == S_OK;
}

LRESULT NavigationTimeoutHandler(const wchar_t* szURL)
{
//	LogCallback(PB_LOG_INFO, L"RhoElementsExtension", szURL, L"NavigationTimeout", __LINE__);
	return onNavEvent(EEID_NAVIGATIONTIMEOUT, (LPARAM)szURL, 0) == S_OK;
}	

LRESULT NavigationErrorHandler(const wchar_t* szURL)
{
//	LogCallback(PB_LOG_INFO, L"RhoElementsExtension", szURL, L"NavigationError", __LINE__);
	return onNavEvent(EEID_NAVIGATIONERROR, (LPARAM)szURL, 0) == S_OK;
}

LRESULT LicenseErrorHandler(const wchar_t* szURL)
{
	return onNavEvent(EEID_APPLICATIONUNLICENSED, (LPARAM)szURL, 0) == S_OK;
}

bool WindowsMessage(MSG& msg)
{
	return ProcessWindowsMessage(msg);
}

bool HTMLMessage(MSG& msg)
{
	return ProcessHTMLMessage(msg);
}

LRESULT SipState(BOOL bSIPState)
{
	return onFieldFocus(EEID_SETSIPSTATE, (LPARAM)bSIPState, 0);
}

LRESULT GeolocationData(int eeID, void* value)
{
	return onGeolocation((EngineEventID)eeID, (LPARAM)value, 0);
}

//LRESULT JavascriptPopup(int iPopupType, void* pPopupData)
//{
//	return onPopUp((EngineEventID)iPopupType, (LPARAM)pPopupData, 0);
//}

LRESULT AuthenticationRequest(int iEngineEvent, void* pAuthenticationData)
{
	return onAuthenticationRequest((EngineEventID)iEngineEvent, (LPARAM)pAuthenticationData, 0);
}

void WindowChangedHandler(LPVOID lparam)
{
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) lparam, 0, 0));
}

void AppActivateHandler(bool bActivate)
{
	ProcessAppActivate(bActivate);
}


