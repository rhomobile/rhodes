#pragma once

// PBCore.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "PBCore.h"
#include <pmpolicy.h>
#include <pm.h>
#ifndef BUILD_AS_RHODES_EXTENSION
#include "../../gen/buildDate.h"
#include "sipapi.h"
#endif
#include "JSONObject.h"

#include "..\..\Engine\common\JSPopup.h"
extern "C" const char* rho_wmimpl_get_webengine();

#ifdef BUILD_AS_RHODES_EXTENSION
#include "../../../../../../extensions/rhoelementsext/ext/rhoelementsext/platform/wm/src/gen/version.h"
#include <common/RhodesApp.h>
#endif

//#define PB_DEBUGLOG 1
#define MAX_LOADSTRING 100
// Use to convert bytes to KB
#define DIV 1024


#ifdef SHELL_AYGSHELL
HWND				g_hWndMenuBar;		// menu bar handle
#else // SHELL_AYGSHELL
HWND				g_hWndCommandBar;	// command bar handle
#endif // SHELL_AYGSHELL

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE, LPTSTR);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK	HTMLWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE			g_hInst;// current instance
HWND			g_hBaseWnd; 
HWND				g_hBrowserWnd;
BOOL				g_bNewPage;	//used by onMeta to run default tags before a new page's tags are run
IMORec				*g_pIMOHead; ///< the head of the linked list for IMO records
//WCHAR				g_szCurrentURL	[MAX_URL + 1];//supplied by 'EEID_BEFORENAVIGATE' in 'onNavEvent'
WCHAR				g_szConfigFilePath[MAX_PATH + 1];//<
LPCTSTR				g_pRegExPath;//
LPCTSTR				g_pPluginPath;
CConfig				*g_pConfig;		//pointer to our global config object
CApp				*g_pTargetApp = NULL;
BOOL				bPlgMsgRegistered = 0;
BOOL				g_bLeftRotated = FALSE;
BOOL				m_bMinimized = FALSE;
CSIP*				m_pSIP;
bool				m_bDialogShown = false;
HANDLE				g_hVolumeSliderWindow;
HANDLE				g_hBacklightSliderWindow;
BOOL	g_bLogOptions[6]; 
DWORD	g_dwLogMaxSize;
BOOL	g_bMaxReached;
DWORD   g_dwMemoryLogFreq;
HMODULE g_hLegacy;
HMODULE g_hJSObjects;
PB_ScreenMode g_bFullScreen = PB_NO_TASKBAR;
#ifdef BUILD_AS_RHODES_EXTENSION
HWND							g_hRhodesWnd;
NAVIGATECALLBACK				*g_pNavigateCB;
EXISTSJSCALLBACK				*g_pExistsJSCB;
INVOKEJSCALLBACK				*g_pInvokeJSCB;
LOGCALLBACK						*g_pLogCB;
GEOLOCATIONCALLBACK				*g_pGeolocationCB;
HISTORYFORWARDCALLBACK			*g_pHistoryForwardCB;
HISTORYBACKCALLBACK				*g_pHistoryBackCB;
REFRESHCALLBACK					*g_pRefreshCB;
QUITCALLBACK					*g_pQuitCB;
MINIMIZECALLBACK				*g_pMinimizeCB;
RESTORECALLBACK					*g_pRestoreCB;
RESIZEBROWSERWINDOWCALLBACK		*g_pResizeBrowserWindowCB;
ZOOMPAGECALLBACK				*g_pZoomPageCB;
ZOOMTEXTCALLBACK				*g_pZoomTextCB;
GETTEXTZOOMCALLBACK				*g_pGetTextZoomCB;
GETTITLECALLBACK				*g_pGetTitleCB;
SETBROWSERGESTURINGCALLBACK		*g_pSetBrowserGesturingCB;
SIPPOSITIONCALLBACK				*g_pPassSipPositionToEngineCB;
REGISTERFORMESSAGECALLBACK		*g_pRegisterForMessageCB;
DEREGISTERFORMESSAGECALLBACK	*g_pDeRegisterForMessageCB;
REGISTERFORPRIMARYMESSAGECALLBACK *g_pRegisterForPrimaryMessageCB;
DEREGISTERFORPRIMARYMESSAGECALLBACK *g_pDeRegisterForPrimaryMessageCB;
#endif


#pragma region PB_Globals

/**********************************************************************************************
* Variables & Objects
**********************************************************************************************/

CRITICAL_SECTION	g_PBLoggingCriticalSection;
#ifndef BUILD_AS_RHODES_EXTENSION
CEng				*g_pEngine					= NULL;
#endif
PPBCORESTRUCT		g_pPBCore					= NULL;					
PPBEVENTSTRUCT		g_pPBCoreEvents				= NULL;
CMeta				*g_pPlgManager				= NULL;//< pointer to the pluginManager
CAppManager			*g_pAppManager				= NULL;
CSync				*g_pEventSync				= NULL;		
CSyncMsg			*pSyncPrimMsg				= NULL;
CWebServer			*g_pWebServer				= NULL;
//CXMLReader			*g_pXMLSetting				= NULL;

int					g_iStartApp					= 0;	//<the default startup application is ID = 0


CMessageManager		*g_pMessageManager			= NULL;
CPaintManager		*g_pPaintManager			= NULL;
CGenericManager		*g_pGenericManager			= NULL;
BOOL				g_bShuttingDown				= FALSE;	///< Used to ensure Translate Accelerator is not called during shutdown
BOOL				g_bEditableFieldFocused		= FALSE;	///<  Whether or not a text field has focus, used in deciding to action the back key or not.


#ifndef BUILD_AS_RHODES_EXTENSION
std::queue<stLog>	*g_pLogQueue;
HANDLE				g_hLogThread;
LPCTSTR				g_pLogFile					= NULL;
bool				g_bFileProtocol				= true;
#endif
SIP_CONTROL_VALUES	g_sipControlMode			= SIP_CONTROL_MANUAL;

#pragma endregion

#pragma region PB_IMO


IMOREF	CreateIMO(PPBSTRUCT pPBStructure,LPCTSTR pTargetModuleName,LPCTSTR pCallingModName)
{
	
	IMORec *pRetIMORec;
		
	if(!pPBStructure || !pTargetModuleName){
		return NULL;
	}
	
	int iOffSet = 0, iInstID = pPBStructure->iTabID;

	if(!g_pIMOHead){
		pRetIMORec = g_pIMOHead = new IMORec;
		memset(pRetIMORec,0,sizeof(IMORec));
		iOffSet = 0;
	}
	else{//add to the existing list
		IMORec *pIMORec = g_pIMOHead;
		if(pIMORec->iParentInstance == iInstID) iOffSet++;
		while(pIMORec->pNext)
		{ 
			if(pIMORec->pNext->iParentInstance == iInstID) iOffSet++;
			pIMORec = pIMORec->pNext;
		}
		pIMORec->pNext = new IMORec;
		pIMORec->pNext->pNext = NULL;
		pRetIMORec = pIMORec->pNext;
	}
	
	if(pRetIMORec){
		pRetIMORec->iParentInstance		= pPBStructure->iTabID;
		
		pRetIMORec->PBStructure.hWnd	= pPBStructure->hWnd;
		pRetIMORec->PBStructure.bInvoked= TRUE;
		pRetIMORec->PBStructure.hInstance= pPBStructure->hInstance;
		pRetIMORec->PBStructure.bLaunchingAppHasFocus = TRUE; // If we're just launching g_pTargetApp may be null, so assume we have focus
		if (g_pTargetApp){
			pRetIMORec->PBStructure.bLaunchingAppHasFocus = g_pTargetApp->m_pbStruct.bLaunchingAppHasFocus;
		}
		if(pPBStructure->iTabID < 0){
			pRetIMORec->PBStructure.iTabID = (pPBStructure->iTabID*IMOS_PER_APP)- PB_APP_ARRSIZE - iOffSet;
		}
		else{
			pRetIMORec->PBStructure.iTabID = (pPBStructure->iTabID*IMOS_PER_APP)+ PB_APP_ARRSIZE + iOffSet;
		}
		
		if(PreloadModule(&pRetIMORec->PBStructure,pTargetModuleName)){
			pRetIMORec->pMod = (void *)g_pPlgManager->GetModule(pTargetModuleName);
			return (IMOREF)pRetIMORec;
		}
	}
	return NULL;
}

BOOL DeleteIMO(IMOREF IMORef)
{
	IMORec *pIMORec = (IMORec *) IMORef;
	if(pIMORec){
		CModRec* pMod = (CModRec *)pIMORec->pMod;
		if(pMod){
			pMod->Dispose(&pIMORec->PBStructure);
		}

		// Check there's something in the IMO list
		// When PB is closing the IMO list may be already freed
		if (!g_pIMOHead)
		{
			return TRUE;
		}

		IMORec *pprevious;

		// Is this IMO the first in the list?
		if (g_pIMOHead == pIMORec)
		{
			// Yes, set the head to point to the next in the list (could be null)
			g_pIMOHead = pIMORec->pNext;
		}
		else
		{
			// No, find previous IMO in list
			for (pprevious = g_pIMOHead; pprevious && pprevious->pNext != pIMORec; pprevious = pprevious->pNext)
				;

			// Check we found it, problem if not
			if (!pprevious)
			{
				Log(PB_LOG_ERROR,L"Previous IMO entry not found!",L"DeleteIMO",__LINE__,L"Core");
				return FALSE;
			}

			// Remove this item from list linkage
			pprevious->pNext = pIMORec->pNext;
		}

		delete pIMORec;
	}

	return TRUE;
}



BOOL	SetIMOProperty	(IMOREF IMORef,LPCTSTR pParam,LPCTSTR pValue)
{
	if(pParam && pValue){
		IMORec *pIMORec = (IMORec *) IMORef;
		if(pIMORec){
			CModRec* pMod = (CModRec *)pIMORec->pMod;
			if(pMod){
				pMod->Preload(&pIMORec->PBStructure);
				return pMod->SetProperty(&pIMORec->PBStructure,pParam,pValue);
			}
		}
	}
	return FALSE;
}

BOOL	CallIMOMethod	(IMOREF IMORef,LPCTSTR pMethod)
{
	if(pMethod){
		IMORec *pIMORec = (IMORec *) IMORef;
		
		if(pIMORec){
			CModRec* pMod = (CModRec *)pIMORec->pMod;
			//return TRUE;
			if(pMod){
				pMod->Preload(&pIMORec->PBStructure);
				return pMod->CallMethod(&pIMORec->PBStructure,pMethod);
			}
			
		}
	}
	return FALSE;
}



BOOL SetIMOCallBack(IMOREF IMORef,IMOEVENTPROC IMOEventProc,LPARAM lParam)
{
	if(IMOEventProc){
		IMORec *pIMORec = (IMORec *) IMORef;
		if(pIMORec){
			pIMORec->pCBFunc = IMOEventProc;
			pIMORec->lParam = lParam;
			return TRUE;
		}
		
	}
	return FALSE;
}

IMORec *GetIMOFromID(int iTabID)
{
	IMORec *pRetIMORec = g_pIMOHead;
	for(pRetIMORec= g_pIMOHead;pRetIMORec;pRetIMORec = pRetIMORec->pNext)
	{
		if(pRetIMORec->PBStructure.iTabID == iTabID){
			return pRetIMORec;
		}
	}
	return NULL;
}

IMORec *DeleteIMOList(IMORec * pIMORec)
{
	if(pIMORec){
		if(pIMORec->pNext){
			DeleteIMOList(pIMORec->pNext);
		}
	}
	delete pIMORec;
	pIMORec = NULL;
	return pIMORec;
}

#pragma endregion

#pragma region PB_BrowserCommands

BOOL BrowserReload(int iAppID,BOOL bCacheLoad,LPCTSTR pCallingModule)
{
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
#ifndef BUILD_AS_RHODES_EXTENSION
		BOOL bRetVal = g_pEngine->ReloadOnTab(bCacheLoad,iAppID);
#else
		g_pRefreshCB(bCacheLoad);
		BOOL bRetVal = TRUE;
#endif
//		SetFocus(g_hBrowserWnd);
		return bRetVal;
	}
} 

BOOL BrowserStop(int iAppID,LPCTSTR pCallingModule)
{
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
		CApp* pApp = g_pAppManager->GetApp(iAppID);
		if(pApp)
		{
			if(pApp->m_bBadlink)
			{
				//  Bad link needs to navigate in a separate thread.
				CloseHandle(CreateThread(NULL, 0, 
					(LPTHREAD_START_ROUTINE) BadLinkNavigateThread,
					(LPVOID) pApp->GetBadlink(pApp->m_szCurrentURL, BADLINK_STOPPED), 
					0, 0));
				//SetFocus(g_hBrowserWnd);
			}
		}
		//  Work around, we do not tell them to Stop() as that gives a NavError, 
		//  we just navigate to the bad link page
		return TRUE;
	}
}

BOOL BrowserSipClick(int iAppID,LPCTSTR pCallingModule)
{
	//  The SIP button has been clicked, notify the engine
	if (!g_bShuttingDown)
		PostMessage(g_hBaseWnd,PB_ENGINESIPCHANGE,(WPARAM)0,(LPARAM)NULL);
	return TRUE;
}

BOOL BrowserBack(int iAppID,LPCTSTR pCallingModule)
{
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
#ifndef BUILD_AS_RHODES_EXTENSION
		BOOL bRetVal = g_pEngine->BackOnTab(iAppID);
#else
		g_pHistoryBackCB();
		BOOL bRetVal = TRUE;
#endif
		//SetFocus(g_hBrowserWnd);
		return bRetVal;
	}
}
BOOL BrowserBack(int iAppID,LPCTSTR pCallingModule,int iPagesBack)
{
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
#ifndef BUILD_AS_RHODES_EXTENSION
		BOOL bRetVal = g_pEngine->BackOnTab(iAppID,iPagesBack);
#else
		g_pHistoryBackCB();  //  We have not defined a Rhodes API for Back(iPages)
		BOOL bRetVal = TRUE;
#endif
		//SetFocus(g_hBrowserWnd);
		return bRetVal;
	}
}

BOOL BrowserForward(int iAddID, LPCTSTR pCallingModule)
{
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
#ifndef BUILD_AS_RHODES_EXTENSION
		BOOL bRetVal = g_pEngine->ForwardOnTab(iAddID);
#else
		g_pHistoryForwardCB();
		BOOL bRetVal = TRUE;
#endif
		//SetFocus(g_hBrowserWnd);
		return bRetVal;
	}
}

BOOL BrowserQuit(int iAppID,LPCTSTR pCallingModule)
{
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
#ifndef BUILD_AS_RHODES_EXTENSION
		//SetFocus(g_hBrowserWnd);
		PostMessage(g_hBaseWnd,PB_GEN_QUIT,0,0);
#else
		g_pQuitCB();
#endif
		return TRUE;
	}
}

BOOL BrowserHome(int iAppID,LPCTSTR pCallingModule)
{
	
	if (DetermineJavaScriptAlertShown())
		return FALSE;
	else
	{
		CApp *pApp =  g_pAppManager->GetApp(iAppID);
		if(pApp){
			BOOL bRetVal = pApp->Home();
			//SetFocus(g_hBrowserWnd);
			return bRetVal;
		}
		return FALSE;
	}
}

BOOL BrowserResize (int id, LPCTSTR module, int left, int top, int width, int height)
{
	if(m_bMinimized)
	{
		PBScreenMode(PB_NOT_SHOWN, false);
		return TRUE;
	}
	RECT rect;

	rect.left = left;
	rect.top = top;
	rect.right = left + width;
	rect.bottom = top + height;
	SetBrowserSize(rect);
	if (g_hBaseWnd)
		MoveWindow(g_hBaseWnd, left, top, width, height, true);

#ifndef BUILD_AS_RHODES_EXTENSION
	return g_pEngine->ResizeOnTab (id, rect);
#else
	g_pResizeBrowserWindowCB(rect);
	return TRUE;
#endif
}

BOOL BrowserMinimize(int iAppID,LPCTSTR pCallingModule)
{

	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) PB_WINMINIMIZED, 0, 0));

#ifndef BUILD_AS_RHODES_EXTENSION
    ShowWindow(g_hBaseWnd,SW_MINIMIZE);
#else
    //SetFocus(g_hBaseWnd);
    // Application in minimized mode gets restored when device is rotated
    //Sabir, Debaraj
    //As per our analysis SetForegroundWindow during minimize should be called only when webkit is running as out process
    //hence we are moving SetForegroundWindow inside a condition
    if(RHODESAPP().isWebkitOutofProcess())
    {
        SetForegroundWindow(g_hBaseWnd);
    }
    g_pMinimizeCB();
#endif
	//  Show the Start Bar
	PBScreenMode(PB_NOT_SHOWN, FALSE);
	m_bMinimized = true;
	m_pSIP->ShowTitle(true);
	return TRUE;
}


BOOL BrowserRestore	(int iAppID,LPCTSTR pCallingModule)
{
	if (g_bShuttingDown)
		return FALSE;
	m_bMinimized = false;
	// Restore event is not getting fired fully in WM and CE devices.
        CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) PB_WINRESTORED, 0, 0));
#ifndef BUILD_AS_RHODES_EXTENSION
	ShowWindow(g_hBaseWnd,SW_RESTORE);
#else
	g_pRestoreCB();
	return TRUE;
#endif
	//  Hide the Start Bar
	PBScreenMode(g_bFullScreen, FALSE);
	SetForegroundWindow(g_hBaseWnd);
	EnableWindow(g_hBaseWnd, TRUE);

	//  Read the page to navigate to from the registry if one is specified.
	//  Lifted directly from PB2.2, needs modifying for RhoElements devices.
	TCHAR newURL[MAX_URL];
	newURL[0] = NULL;
	DWORD RetSize = 2048;
	DWORD Type;
	HKEY hRegKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Symbol\\SymbolPB\\Temp", 0, 0, &hRegKey))
	{
		RegQueryValueEx(hRegKey, _T("cmdline"), NULL, &Type, (BYTE *) newURL, &RetSize);
		if(newURL!=NULL)
		{
			PageNavigate(0, newURL);
		}
		RegCloseKey(hRegKey);
		RegDeleteKey(HKEY_CURRENT_USER, L"Software\\Symbol\\SymbolPB\\Temp");
	}
	
	//CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) PB_WINRESTORED, 0, 0));
	m_pSIP->ShowTitle(false);
	return TRUE;
}

BOOL BrowserSetPageZoom(int iAppID,double fZoomFactor,LPCTSTR pCallingModule)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	return g_pEngine->ZoomPageOnTab(fZoomFactor,iAppID);
#else
	g_pZoomPageCB((float)fZoomFactor);
	return TRUE;
#endif
}
double BrowserGetPageZoom(int iAppID,LPCTSTR pCallingModule)
{
	return 1.1;
}

//returns the text zoom factor
DWORD BrowserGetTxtZoom(int iAppID,LPCTSTR pCallingModule)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	TextZoomValue Val;
	if(g_pEngine->GetTextZoomOnTab(&Val,iAppID)){
		return (DWORD)Val;
	}
#else
	return g_pGetTextZoomCB();
#endif
	return -1;//failed
}

BOOL BrowserSetTxtZoom(int iAppID, int pZoomFactor, LPCTSTR pCallingModule)
{
	//TextZoomValue txtZoom = (TextZoomValue)8;
#ifndef BUILD_AS_RHODES_EXTENSION
	return g_pEngine->ZoomTextOnTab((TextZoomValue)pZoomFactor,iAppID);
#else
	g_pZoomTextCB(pZoomFactor);
	return TRUE;
#endif
}

#ifndef BUILD_AS_RHODES_EXTENSION

BOOL BrowserGetSize (int appid, int *pwidth, int *pheight)
{
	return g_pEngine->GetSizeOnTab (pwidth, pheight, appid);
}

BOOL BrowserSetVScroll (int appid, int scroll)
{
	return g_pEngine->SetVScrollOnTab (scroll, appid);
}

BOOL BrowserSetHScroll (int appid, int scroll)
{
	return g_pEngine->SetHScrollOnTab (scroll, appid);
}

BOOL BrowserSetAccelerator(AcceleratorValue eAcceleratorValue)
{
	return g_pEngine->SetAccelerator(eAcceleratorValue);
}
#endif

#pragma endregion

#pragma region PB_Config_ReadWrite 

BOOL SetAppValue(int iAppID,LPCTSTR pXmlPath,LPCTSTR pValue)
{
	return g_pConfig->SetAppValue(iAppID,pXmlPath,pValue); 
}
BOOL SetGlobalValue(LPCTSTR pXmlPath, LPCTSTR pValue)
{
	return g_pConfig->SetGlobalValue(pXmlPath,pValue); 
}

BOOL AddPreload(int iAppID,LPCTSTR pPreloadStr)
{
	return g_pConfig->AddPreload(iAppID,pPreloadStr); 
}

BOOL AddDefaultMeta(int iAppID,LPCTSTR metaTag)
{
	return g_pConfig->AddDefaultMeta(iAppID,metaTag); 
}

LPCTSTR GetSettingPtr(LPCTSTR pXmlPath,LPTSTR pAttName)
{
	return g_pConfig->GetSettingPtr(pXmlPath,pAttName); 
}

LPCTSTR GetAppSettingPtr(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName)
{
	return g_pConfig->GetAppSettingPtr(iAppID,pXmlPath,pAttName); 
}
LPCTSTR GetDefaultMetaTagsPtr(int iAppID,int iMetaIndex)
{
	return g_pConfig->GetDefaultMetaTagsPtr(iAppID,iMetaIndex); 
}
LPCTSTR GetPreLoadPtr(int iAppID,int iPreLoadIndex)
{
	return g_pConfig->GetPreLoadPtr(iAppID,iPreLoadIndex); 
}

#pragma endregion

LPCTSTR GetCurrURL(int iAppID)
{
	Lock AutoLock(g_pAppManager->GetCriticalSect()); 
	
	
	//is this a reserved ID?
	if(iAppID < 0){
		return NULL;//a negative IMO which has no alliance with an application, so just return NULL;
	}

	int iUpper,iLower;
	
	//is this an IMO? if so,get the application ID first 
	int iID = iAppID;
	if(iAppID >= PB_APP_ARRSIZE ){
		for(iID=0;iID < PB_APP_ARRSIZE;iID++)
		{
			iUpper = (((iID+1)*IMOS_PER_APP)+PB_APP_ARRSIZE);
			iLower = ((iID*IMOS_PER_APP)+PB_APP_ARRSIZE);
			if(iAppID >= ((iID*IMOS_PER_APP)+PB_APP_ARRSIZE)&& iAppID < (((iID+1)*IMOS_PER_APP)+PB_APP_ARRSIZE)){
				return g_pAppManager->GetCurrURL(iID);
			}
		}
		//error
		return NULL;
	}
	
	return g_pAppManager->GetCurrURL(iID);	
}




/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	March 2011, DCC, Initial Creation.
*/
int GetComponentVersions(TCHAR* tcFileVersion, CVersion* listVersions)
{
	//  Firstly calculate the product version number, this is the first four digits of the
	//  File Version.
	//  If the user passes NULL they are asking for the size of the product version array, 
	//  cheat by giving them the maximum value immediately.

	//  In the context of Windows:
	//   FileVersion --> v.w.x.y format numeric version (where y is our release candidate)
	//   ProductVersion --> v, w, x, y, z format string version (where z is our hot fix number)
	if (tcFileVersion == NULL)
		return wcslen(L"vvv.www.xxx.yyy");

#ifndef BUILD_AS_RHODES_EXTENSION
	DWORD dwLen, dwUseless = 0;
    LPTSTR lpVI;
    DWORD dwVersion = 0;
	VS_FIXEDFILEINFO* vsApplicationVersion;
	wchar_t wcFilename[256];
	GetModuleFileName(NULL, wcFilename, 255);
    dwLen = GetFileVersionInfoSize(wcFilename, &dwUseless);
	int returnValue = 0;
    if (dwLen==0) 
	{
		dwLen = GetLastError();
        return returnValue;
	}
    lpVI = (LPTSTR) GlobalAlloc(GPTR, dwLen);
    if (lpVI)
    {
        DWORD dwBufSize;
        GetFileVersionInfo(wcFilename, NULL, dwLen, lpVI);
        if (VerQueryValue(lpVI, _T("\\"), (LPVOID *) &vsApplicationVersion, (UINT *) &dwBufSize))
		{
			memset(tcFileVersion, 0, sizeof(TCHAR) * wcslen(L"vvv.www.xxx.yyy"));
			wsprintf(tcFileVersion, L"%d.%d.%d.%d", 
				HIWORD(vsApplicationVersion->dwFileVersionMS), 
				LOWORD(vsApplicationVersion->dwFileVersionMS),
				HIWORD(vsApplicationVersion->dwFileVersionLS),
				LOWORD(vsApplicationVersion->dwFileVersionLS));
			returnValue = wcslen(tcFileVersion);
		}
		else
			returnValue = 0;

        GlobalFree((HGLOBAL)lpVI);
	}

	//  Retrieve the product version of the main executable (PBCore)
	TCHAR tcCoreFileName[256];
	GetModuleFileName(NULL, tcCoreFileName, 255);
	GetProductVersionString(tcCoreFileName, listVersions);

#else
	CVersion* listVersionsTemp = listVersions;
	wsprintf(tcFileVersion, L"%d.%d.%d.%d", RE_MAJOR_VER, RE_MINOR_VER_1, RE_MINOR_VER_2, RE_MINOR_VER_3);
	CVersion* newVersion = new CVersion();
	newVersion->iMajorVer = RE_MAJOR_VER;
	newVersion->iFeatureVer = RE_MINOR_VER_1;
	newVersion->iMaintenanceVer = RE_MINOR_VER_2;
	newVersion->iReleaseCandidateVer = RE_MINOR_VER_3;
	newVersion->iHotFixVer = RE_MINOR_VER_4;
	newVersion->tcComponentName = new WCHAR[wcslen(L"RhoElements Runtime") + 1];
	wcscpy(newVersion->tcComponentName, L"RhoElements Runtime");
	while (listVersionsTemp->pNext != NULL)
		listVersionsTemp = listVersionsTemp->pNext;
	listVersionsTemp->pNext = newVersion;
#endif

/*
	//  DCC (23/01/2013): Deleting this code as we move to semantic versioning and 
	//  common API set we are no longer supporting individual component versions in the 
	//  Version API
	//  Populate the Plugin Versions array
	g_pPlgManager->RetrieveProductVersion(listVersions);

	//  Populate the Engine version
	GetProductVersionString(g_pAppManager->m_pEngDLLFilePath, listVersions);

	//  Allocate enough space for any of the ActiveX / NPAPI
	int iLen = wcslen(g_pPBCore->szInstallDirectory) + 
			wcslen(PB_BIN_INSTDIR) + 30;
	WCHAR* tcActiveXFileLocation = new WCHAR[iLen];
	WCHAR* tcGenericActiveX = new WCHAR[iLen];
	WCHAR* tcODAXActiveX = new WCHAR[iLen];
	WCHAR* tcNoSIPActiveX = new WCHAR[iLen];
	WCHAR* tcLegacyNPAPI = new WCHAR[iLen];
	WCHAR* tcJSObjectsNPAPI = new WCHAR[iLen];

	if(!tcActiveXFileLocation || !tcGenericActiveX || !tcODAXActiveX || !tcNoSIPActiveX 
		|| !tcLegacyNPAPI)
		return FALSE;

	wcscpy(tcActiveXFileLocation, g_pPBCore->szInstallDirectory);
	wcscat(tcActiveXFileLocation, PB_BIN_INSTDIR);
	wcscpy(tcGenericActiveX, tcActiveXFileLocation);
	wcscat(tcGenericActiveX, L"PocketBrowser.dll");
	wcscpy(tcODAXActiveX, tcActiveXFileLocation);
	wcscat(tcODAXActiveX, L"CeODAX.dll");
	wcscpy(tcNoSIPActiveX, tcActiveXFileLocation);
	wcscat(tcNoSIPActiveX, L"NoSIP.dll");
	wcscpy(tcLegacyNPAPI, g_pPBCore->szInstallDirectory);
	wcscpy(tcJSObjectsNPAPI, g_pPBCore->szInstallDirectory);
	wcscat(tcLegacyNPAPI, PB_NPAPI_INSTDIR);
	wcscat(tcJSObjectsNPAPI, PB_NPAPI_INSTDIR);
	wcscat(tcLegacyNPAPI, L"npwtg_legacy.dll");
	wcscat(tcJSObjectsNPAPI, L"npwtg_jsobjects.dll");

	//  Populate the versions of the ActiveX components, Browser did not support
	//  moving these on the device
	GetProductVersionString(tcGenericActiveX, listVersions);
	GetProductVersionString(tcODAXActiveX, listVersions);
	GetProductVersionString(tcNoSIPActiveX, listVersions);

	//  Populate the version of the NPAPI components
	GetProductVersionString(tcLegacyNPAPI, listVersions);
	GetProductVersionString(tcJSObjectsNPAPI, listVersions);

	g_hLegacy = LoadLibrary(tcLegacyNPAPI);
	g_hJSObjects = LoadLibrary(tcJSObjectsNPAPI);

	if (!g_hLegacy)
		Log(PB_LOG_WARNING, L"Failed to load legacy NPAPI, this functionality will not be available",
		_T(__FUNCTION__), __LINE__, L"Core");
	if (!g_hJSObjects)
		Log(PB_LOG_WARNING, L"Failed to load JSObjects NPAPI, this functionality will not be available",
		_T(__FUNCTION__), __LINE__, L"Core");


	delete[] tcGenericActiveX;
	delete[] tcODAXActiveX;
	delete[] tcNoSIPActiveX;
	delete[] tcActiveXFileLocation;
	delete[] tcLegacyNPAPI;
*/
	return TRUE;
}


/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	March 2011, Initial Creation (DCC)
*/
BOOL GetProductVersionString(TCHAR* tcFileName, CVersion* listVersions)
{
	CVersion* listVersionsTemp = listVersions;

	DWORD temp;
	DWORD size = GetFileVersionInfoSize(tcFileName, &temp);
	
	if (size == 0)
		return FALSE;	//  Unable to find file

	TCHAR* fvBuf = new TCHAR[size];
	if (!fvBuf)
		return FALSE;

	GetFileVersionInfo(tcFileName, 0, size, fvBuf);

	DWORD* langCodeArray;
	UINT aLen;
	VerQueryValue(fvBuf, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&langCodeArray, &aLen);

	TCHAR subBlock[40];
	wsprintf(subBlock, TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"),
	LOWORD(langCodeArray[0]), HIWORD(langCodeArray[0]));

	UINT bufLen;
	TCHAR* tempBuf = new TCHAR[size];
	TCHAR* tempBuffForDelete = tempBuf;
	if (!tempBuf)
		return FALSE;
	VerQueryValue(fvBuf, subBlock, (LPVOID*)(&tempBuf), &bufLen);

	//  tempBuf now contains the product version as a string, convert this to a version object.
	//  Format of tempBuf is v, w, x, y, z (with spaces)
	CVersion* newVersion = new CVersion();
	if (wcsrchr(tcFileName, L'\\'))
	{
		newVersion->tcComponentName = new TCHAR[wcslen(wcsrchr(tcFileName, L'\\')) + 1];
		if (!newVersion)
			return FALSE;
		memset(newVersion->tcComponentName, 0, sizeof(TCHAR) * (wcslen(wcsrchr(tcFileName, L'\\')) + 1));
		TCHAR* fileName = wcsrchr(tcFileName, L'\\');
		wcscpy(newVersion->tcComponentName, ++fileName);
	}
	if (tempBuf && wcslen(tempBuf) > 1)
	{
		newVersion->iMajorVer = _wtoi(tempBuf);
		tempBuf = wcschr(tempBuf, L',');
	}
	if (tempBuf && wcslen(tempBuf) > 1)
	{
		newVersion->iFeatureVer = _wtoi(++tempBuf);
		tempBuf = wcschr(tempBuf, L',');
	}
	if (tempBuf && wcslen(tempBuf) > 1)
	{
		newVersion->iMaintenanceVer = _wtoi(++tempBuf);
		tempBuf = wcschr(tempBuf, L',');
	}
	if (tempBuf && wcslen(tempBuf) > 1)
	{
		newVersion->iReleaseCandidateVer = _wtoi(++tempBuf);
		tempBuf = wcschr(tempBuf, L',');
	}
	if (tempBuf)
	{
		newVersion->iHotFixVer = _wtoi(++tempBuf);
	}

	while (listVersionsTemp->pNext != NULL)
		listVersionsTemp = listVersionsTemp->pNext;
	listVersionsTemp->pNext = newVersion;
	delete[] fvBuf;
	delete[] tempBuffForDelete;
	return TRUE;
}

BOOL SipControlChange(SIP_CONTROL_VALUES newVal)
{
	g_sipControlMode = newVal;
	return !g_pAppManager->m_bUsingLegacyEngine;
}

BOOL DisableScrolling(BOOL bDisableScrolling)
{
	//  The ability to disable scrolling applies specifically to 
	//  finger scrolling and we only want to override the configured value
	//  if it was configured as 'fingerscroll'.  Gestures only want to be able
	//  to disable fingerscrolling.
	if (!g_bShuttingDown && g_pAppManager && g_pAppManager->GetApp(0)->m_iScrollTechnique == SCROLLING_FINGERSCROLL)
	{
		g_pAppManager->GetApp(0)->m_bDisableScrolling = bDisableScrolling;
	#ifndef BUILD_AS_RHODES_EXTENSION
		g_pEngine->SetBrowserGesturing(!bDisableScrolling);
	#else
		g_pSetBrowserGesturingCB(!bDisableScrolling);
	#endif
	}
	return TRUE;
}

CEMML	*GetEMMLObj(int iTabID)
{
	if(g_pAppManager){
		return g_pAppManager->GetApp(iTabID)->m_pEMML;

	}
	return NULL;
}

/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	March 2009, Initial Creation (DCC)
*/
BOOL ResizePB(BOOL bAccountForSIP, BOOL bAccountForIgnoreSettingChange)
{
	//  Rhodes are handling WM_SETTINGCHANGE also, appears to be 
	//  conflicting with our SIP handling.  Raised with EV but removing
	//  resize on SIP functionality for now.
#ifndef BUILD_AS_RHODES_EXTENSION
	if (!m_bMinimized)
	{
		PBScreenMode(g_bFullScreen, bAccountForSIP, bAccountForIgnoreSettingChange);
	}
	else
		PBScreenMode(PB_NOT_SHOWN, false);
#endif
	return TRUE;
}


#pragma region PB_STARTUP




/****************************************************************************************************/

BOOL Initialise(LPTSTR lpCmdLine)
{
	//  Display the load cursor
#ifndef BUILD_AS_RHODES_EXTENSION
	SetCursor(LoadCursor(g_hInst, IDC_WAIT));
#endif
	//  This needs to be the first CSIP instance as this instance is
	//  responsible for hiding / showing the title bar as appropriate
	m_pSIP = new CSIP();
	LPCTSTR pAppName,pStartPage;
	LPCTSTR  pTemp,pPlgFilePath;
	pAppName = pStartPage = pTemp = pPlgFilePath = NULL;
	int appLoop;//,PreloadLoop;
	g_pIMOHead = NULL; //set the head of the IMO linked list to NULL
	
	g_bNewPage = TRUE;

	

	
	//create the CSyncMsg object that synchronises primary messages with registered plug-ins
	if((pSyncPrimMsg = new CSyncMsg())==NULL){
		goto _cleanup;
	}

	//Create the sync object that handles 
	if((g_pEventSync = new CSync())==NULL){
		goto _cleanup;
	}
	
	if((g_pConfig = new CConfig()) == NULL){
		goto _cleanup;
	}
	
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// InitCoreStruct Creates the core structure and event structure
	/// It also initialises the structure function pointers
	/// And sets up the global events	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if((InitCoreStruct())!=SUCCESS){
		goto _cleanup;
	}
	
		
	
	
		
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// open the config.xml file, which contains application settings
	// this will write the XML file to memory structures held in g_pXMLSetting object
	//////////////////////////////////////////////////////////////////////////////////////////////////	
	//  User is able to specify a configuration file on the command line as /C:
	TCHAR tcCommandLineStartPage[MAX_URL];
	memset(tcCommandLineStartPage, 0, MAX_URL * sizeof(TCHAR));
	wcscpy(g_szConfigFilePath, L"");
	ParseCommandLine(lpCmdLine, tcCommandLineStartPage, g_szConfigFilePath);

	//  Check to see if a configuration file was defined on the command line
	if (!g_szConfigFilePath[0])
	{
		//  No configuration file was defined on the command line, use the default path
		wcscpy(g_szConfigFilePath, g_pPBCore->szInstallDirectory);
		wcscat(g_szConfigFilePath, PB_CONFIG_INSTDIR);
		wcscat(g_szConfigFilePath, CONF_FILE);
	}

	//  Check to see if the specified config file exists
	HANDLE hFile = CreateFile (g_szConfigFilePath,GENERIC_READ,
	FILE_SHARE_READ ,NULL,OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//  A Valid Config.xml has been found
		CloseHandle(hFile);
		WCHAR* szConfigMsg = new WCHAR[MAX_PATH + 40];
		wsprintf(szConfigMsg, L"RhoElements is using Config file: %s", g_szConfigFilePath);
		Log(PB_LOG_INFO, szConfigMsg, _T(__FUNCTION__), __LINE__, L"Core");
		delete[] szConfigMsg;
	}
	else
	{
		WCHAR* szConfigMsg = new WCHAR[MAX_PATH + 40];
		wsprintf(szConfigMsg, L"RhoElements could not find Config.xml (%s)", g_szConfigFilePath);
		MessageBox(NULL, szConfigMsg, L"Config Error", MB_OK | MB_TOPMOST);
		delete[] szConfigMsg;
		goto _cleanup;
	}
	
	//  Now try parsing the Config file
	if((g_pConfig->Init(g_szConfigFilePath))==NULL)
	{
		WCHAR* szConfigErrorMsg = new WCHAR[MAX_PATH + 40];
		wsprintf(szConfigErrorMsg, L"Please check your config.xml (%s)", g_szConfigFilePath);
		MessageBox(NULL,szConfigErrorMsg, L"Config Error", MB_OK | MB_TOPMOST);
		delete[] szConfigErrorMsg;
		goto _cleanup;
	}
		
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the configuration settings from the config object
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the logger settings and start the logger if it is specified in the configuration file
	//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef BUILD_AS_RHODES_EXTENSION
	if((g_pLogFile = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogURI",L"Value"))==NULL){
		// the logger has failed to start so the log function will output this as a message box
		Log(PB_LOG_ERROR,L"The log file location cannot be found", _T(__FUNCTION__),__LINE__,L"Core");
		goto _cleanup;
	}
	
	if (g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogProtocol",L"Value"))
		g_bFileProtocol = !_memicmp(g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogProtocol",L"Value"),L"FILE",8);

	if(!_memicmp(g_pLogFile,L"FILE://",14)){//if the protocol is part of the string it will supersede the 'LogProtocol' setting
		g_pLogFile = g_pLogFile+=7;
		g_bFileProtocol = true;
	}
	else if(!_memicmp(g_pLogFile,L"HTTP://",14)){//if the protocol is part of the string it will supersede the 'LogProtocol' setting
		g_pLogFile = g_pLogFile+=7;
		g_bFileProtocol = false;
	}
	//set up the logger 
	if(StartLogger()!=SUCCESS){
		// the logger has failed to start so the log function will output this as a message box
		Log(PB_LOG_ERROR,L"Logger has failed", _T(__FUNCTION__),__LINE__,L"Core");
		goto _cleanup;
	}	

	if (g_bLogOptions[PB_LOG_MEMORY])
	{
		//  Only start the memory logging thread if the user has specified to log memory
		LogMemory();

		//Get handle to main RhoElements window
		HWND hwnd = FindWindow(PB_WINDOW_NAME, NULL);
		
		// Set the timer.  
		SetTimer(hwnd,						// handle to main window 
				PB_MEMLOGTIMER,				// timer identifier 
				g_dwMemoryLogFreq,			// 5-second interval(default) 
				(TIMERPROC) OnMemoryLogTimeOut); // timer callback
	}		
	
#endif

	//  Log our version number and build time / date
	TCHAR versionString[1024];

#ifndef BUILD_AS_RHODES_EXTENSION
	wsprintf(versionString, L"RhoElements Build Date: %s", WTG_BUILD_DATE);
	Log(PB_LOG_INFO, versionString, _T(__FUNCTION__), __LINE__, L"Core");	
	wsprintf(versionString, L"RhoElements Build Time: %s", WTG_BUILD_TIME);
	Log(PB_LOG_INFO, versionString, _T(__FUNCTION__), __LINE__, L"Core");
#ifdef RHOELEMENTS_LIGHT
	Log(PB_LOG_INFO, L"Running RhoElements Light", _T(__FUNCTION__), __LINE__, L"Core");
#endif  //  RHOELEMENTS_LIGHT
#endif	//  BUILD_AS_RHODES_EXTENSION

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// get the plugin file and load the plugin data
	//////////////////////////////////////////////////////////////////////////////////////////////////	
	
	if((g_pPluginPath = g_pConfig->GetSettingPtr(L"Configuration\\FileLocations\\PluginFile",L"Value"))==NULL){//first get the length
		Log(PB_LOG_ERROR,L"Plugin file not specified", _T(__FUNCTION__),__LINE__,L"Core");
		goto _cleanup;
	}
	
	if((g_pRegExPath = g_pConfig->GetSettingPtr(L"Configuration\\FileLocations\\RegEXFile",L"Value"))==NULL){
		Log(PB_LOG_ERROR,L"RegEx file not specified", _T(__FUNCTION__),__LINE__,L"Core");
		goto _cleanup;
	}
	

	//set the fullscreen mode
#ifndef BUILD_AS_RHODES_EXTENSION
	pTemp = g_pConfig->GetSettingPtr(L"Configuration\\Screen\\FullScreen",L"Value");
	if(pTemp){
		if (*pTemp==L'1')
			g_bFullScreen = PB_NO_TASKBAR;
		else
			g_bFullScreen = PB_WITH_TASKBAR;
	}
#else
	//  EV : Rhodes will handle full screen mode
	g_bFullScreen = PB_WITH_TASKBAR;
#endif
	// initialise and set up the plug-in manager
	if(LoadPlugs(g_pPluginPath)!=SUCCESS){ 
		goto _cleanup;
	}

	//  Disable the Volume and Backlight sliders specific to Nano (for now)
	BlockNanoSliders(TRUE);
	
	
	int iIndex = PB_INVALID_APPID;
	CApp* pApp = NULL;

	//LPCTSTR pPreload;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// for each application tag found under configuration	
	// add the application to the global app manager 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	for(appLoop = 0;;appLoop++)							
	{
		
		
		pAppName	= g_pConfig->GetAppSettingPtr(appLoop,L"General\\Name",L"Value");
		if (!tcCommandLineStartPage[0])
			pStartPage	= g_pConfig->GetAppSettingPtr(appLoop,L"General\\StartPage",L"Value");
		else
			pStartPage = tcCommandLineStartPage;

		if(!pAppName || !pStartPage){
			if(!appLoop){
				goto _cleanup;
			}
			break;
		}
		
		
		
		if(appLoop == 0){//if this is the first application
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// This is the first application found. 
			// So create the engine and application manager
			// 
			// g_pAppManager is a pointer to the application manager. It handles each Browser application.
			// g_pEngine is a pointer to the engine class wrapper
			// 
			//////////////////////////////////////////////////////////////////////////////////////////////////			
#ifndef BUILD_AS_RHODES_EXTENSION
			if(!(g_pEngine = new CEng(g_hBaseWnd,g_hInst))){
				Log(PB_LOG_ERROR,L"Cannot create the engine object", _T(__FUNCTION__),__LINE__,L"Core");
				goto _cleanup;
			}
#endif

			if(!(g_pAppManager = new CAppManager())){
				goto _cleanup;
			}
			
			// initialise the Application manager
#ifndef BUILD_AS_RHODES_EXTENSION
			if(!g_pAppManager->Init(g_hInst,g_hBaseWnd,g_pPBCore,g_pEngine,pAppName,pStartPage)){
				goto _cleanup;
			}
#else
			if(!g_pAppManager->Init(g_hInst,g_hBaseWnd,g_pPBCore,pAppName,pStartPage)){
				goto _cleanup;
			}
#endif

			g_pPBCore->iCurrentInstID = 0;
			iIndex = 0;

			//  Log the Versions of all the components in the system			
			CVersion* componentVersionsHead = new CVersion();
			int iRetVal = GetComponentVersions(versionString, componentVersionsHead);
			if (iRetVal > 0)
			{
				CVersion* tempVer = componentVersionsHead;
				while (tempVer != NULL && tempVer->pNext!= NULL)
				{
					//  Head element is empty
					tempVer = tempVer->pNext;
					wsprintf(versionString, L"RhoElements Component Version: %s = %i.%i.%i.%i.%i", 
						tempVer->tcComponentName, tempVer->iMajorVer, tempVer->iFeatureVer,
						tempVer->iMaintenanceVer, tempVer->iReleaseCandidateVer, tempVer->iHotFixVer);
					Log(PB_LOG_INFO, versionString, _T(__FUNCTION__), __LINE__, L"Core");
				}
			}

			//  Delete memory associated with component versions list
			CVersion* nextNode = componentVersionsHead->pNext;
			CVersion* currentNode = componentVersionsHead;
			while (nextNode)
			{
				//  The Head of the list has no version data, all data is contained in subsequent nodes
				delete currentNode;
				currentNode = nextNode;
				nextNode = currentNode->pNext;
			}
			//  End Component logging

		}
		else{
			///<add the application and start page to the global manager
			iIndex = g_pAppManager->AddApp(pAppName,pStartPage);///<inform the application manager
			if(iIndex == PB_INVALID_APPID){
				Log(PB_LOG_ERROR,L"App manager could not add application", _T(__FUNCTION__),__LINE__,L"Core");
				goto _cleanup;
			}
			Log(PB_LOG_INFO,L"Application Added: ",pAppName,__LINE__,L"Core");
		}
		
		//reset the pointers ready for the next iteration
		pAppName = pStartPage = NULL;
		
	}
	
	PBScreenMode(g_bFullScreen, FALSE);
	
	g_pAppManager->LoadDummy();
	g_pAppManager->AddLicenceApp();

	//initialise internal webserver
	g_pWebServer = new CWebServer();

	//Add MimeTypes
	g_pWebServer->AddMimeType(L"html","text/html");
	g_pWebServer->AddMimeType(L"htm","text/html");
	g_pWebServer->AddMimeType(L"png","image/png");
	g_pWebServer->AddMimeType(L"jpg","image/jpeg");
	g_pWebServer->AddMimeType(L"gif","image/gif");
	g_pWebServer->AddMimeType(L"css","text/css");
	g_pWebServer->AddMimeType(L"js","text/javascript");
	g_pWebServer->AddMimeType(L"json","application/json");

	//Read Webserver Port Number
	pTemp = g_pConfig->GetSettingPtr(L"Configuration\\WebServer\\Port",L"Value");
	USHORT usPort = 80;
	if(pTemp) usPort = (USHORT) _wtol(pTemp);
	g_pWebServer->SetPortNumber(usPort);

	//Read Public Access
	pTemp = g_pConfig->GetSettingPtr(L"Configuration\\WebServer\\Public",L"Value");
	if (pTemp && *pTemp==L'1') g_pWebServer->SetAllowPublic(TRUE);
	
	//Read Website Folder
	pTemp = g_pConfig->GetSettingPtr(L"Configuration\\WebServer\\WebFolder",L"Value");
	if (pTemp) g_pWebServer->SetWebFolder(pTemp);

	//Enable the WebServer
	pTemp = g_pConfig->GetSettingPtr(L"Configuration\\WebServer\\Enabled",L"Value");
	if (pTemp && *pTemp==L'1') g_pWebServer->Initialise();
	//  Hide the load cursor
#ifndef BUILD_AS_RHODES_EXTENSION
	SetCursor(LoadCursor(g_hInst, IDC_ARROW));
#endif
	return TRUE;

_cleanup:
#ifndef BUILD_AS_RHODES_EXTENSION
	SetCursor(LoadCursor(g_hInst, IDC_ARROW));
#endif
	delete g_pMessageManager;
	delete g_pPaintManager;
	delete g_pGenericManager;
	delete g_pAppManager;
#ifndef BUILD_AS_RHODES_EXTENSION
	delete g_pEngine;
#endif
	delete g_pEventSync;
	delete g_pConfig;
	delete g_pWebServer;
	g_pMessageManager	= NULL;
	g_pPaintManager		= NULL;
	g_pGenericManager	= NULL;
	g_pAppManager		= NULL;
#ifndef BUILD_AS_RHODES_EXTENSION
	g_pEngine			= NULL;
#endif
	//g_pLogFile			= NULL;
	//g_pPluginPath		= NULL;
	//g_pRegExPath		= NULL;
	g_pEventSync		= NULL;
	g_pConfig			= NULL;
	g_pWebServer		= NULL;
	return FALSE;
}


void BlockNanoSliders(BOOL bBlockSliders)
{
	SetLastError(0);
	g_hBacklightSliderWindow = CreateEvent(NULL, TRUE, FALSE, L"Keybd/RhoElements/DisableBacklightApp");
	g_hVolumeSliderWindow = CreateEvent(NULL, TRUE, FALSE, L"Keybd/RhoElements/DisableVolumeApp");

	DWORD dwError = GetLastError();
	if (dwError == ERROR_ALREADY_EXISTS)
	{
		//  We are running on a device which supports backlight / volume sliders (i.e. Nano)
		if (bBlockSliders)
		{
			bool bBacklightSlider = true;
			bool bVolumeSlider = true;
			//  Read from the Configuration whether or not we want to show them
			LPCTSTR pBacklightSlider;
			LPCTSTR pVolumeSlider;
			pBacklightSlider = g_pConfig->GetAppSettingPtr(0,L"DeviceKeys\\EnableBacklightSlider",L"Value");	
			if(pBacklightSlider){
				bBacklightSlider = *pBacklightSlider==L'0'?TRUE:FALSE;
			}
			pVolumeSlider = g_pConfig->GetAppSettingPtr(0,L"DeviceKeys\\EnableVolumeSlider",L"Value");	
			if(pVolumeSlider){
				bVolumeSlider = *pVolumeSlider==L'0'?TRUE:FALSE;
			}
			if (bBacklightSlider)
				SetEvent(g_hBacklightSliderWindow);
			if (bVolumeSlider)
				SetEvent(g_hVolumeSliderWindow);

		}
		else
		{
			//  We want to reenable the sliders regardless of whether the user disabled them
			//  or not, this is different from something like IST Enable which is user
			//  configurable anyway.
			if (g_hBacklightSliderWindow)
				ResetEvent(g_hBacklightSliderWindow);
			if (g_hVolumeSliderWindow)
				ResetEvent(g_hVolumeSliderWindow);
		}
	}
	else
	{
		g_hBacklightSliderWindow = NULL;
		g_hVolumeSliderWindow = NULL;
	}
}


/*
ReagConfigSettings(LPCTSTR pFilePath)
{
	
}
*/

/*
int LoadEMMLManager()
{
	g_pEMML = new CEMML();


	int iNoRegExps = dim(RegExPatternsHE);
	//add the regex patterns
	for(int i=0; i<iNoRegExps; i++)
		g_pEMML->AddRegExPair_HE(RegExPatternsHE[i][0], RegExPatternsHE[i][1]);
		
	iNoRegExps = dim(RegExPatternsCT);

	for(int i=0; i<iNoRegExps; i++)
		g_pEMML->AddRegExPair_CT(RegExPatternsCT[i][0], RegExPatternsCT[i][1]);


}
*/

void DeInitialise()
{
	//  Check to see if the 'PowerKey Action' Dialog is visible (MPA2.0 devices)
	//  as PB will crash if it shuts down whilst this window is visible.
	
	DEBUGMSG(true, (L"Deinitialising RhoElements"));
#ifndef BUILD_AS_RHODES_EXTENSION
	HWND hPowerWindow = FindWindow(L"Dialog", POWERKEY_ACTION);
	if (hPowerWindow)
		return;
#endif

	if (m_bDialogShown)
		return;

	DEBUGMSG(true, (L"Hiding SIP"));
	HideSipIfShown();
	m_pSIP->ShowTitle(true);

	//JMS 5/5/2010
	//adding unnattended mode to avoid crashing after suspend during closedown process
	PowerPolicyNotify (PPN_UNATTENDEDMODE, TRUE);
	SetCursor(LoadCursor(g_hInst, IDC_WAIT));
	BOOL bClosed = FALSE;
	int iIndex = 0;
	bPlgMsgRegistered = FALSE;
	
	// Show the taskbar and reset the screen to non fullscreen	
	PBScreenMode(PB_NOT_SHOWN, FALSE);//set to non fullscreen
	BlockNanoSliders(FALSE);

	g_bShuttingDown = TRUE;
	FreeLibrary(g_hLegacy);
	FreeLibrary(g_hJSObjects);
	
	SetEvent(g_pPBCoreEvents->PBQuitEvent);

    if ( g_pAppManager )
    {
	    for(iIndex = 0;g_pAppManager->m_pApp[iIndex];iIndex++)
	    {
		    g_pAppManager->RemoveApp(iIndex);
	    }
    }

#ifndef BUILD_AS_RHODES_EXTENSION
    if ( g_pEngine )
    {
	    if(g_pEngine->DeInitEngine()){
			    bClosed = TRUE;
	    }
	    DEBUGMSG(1,(L"\nEngine Deinitialised"));	
    }
#endif

	delete pSyncPrimMsg;
	pSyncPrimMsg = NULL;
	
	delete g_pAppManager;
	g_pAppManager = NULL;
	

	delete g_pMessageManager;
	g_pMessageManager = NULL;
	
	delete g_pPaintManager;
	g_pPaintManager = NULL;
	
	delete g_pGenericManager;
	g_pGenericManager = NULL;

//	delete g_pEngine;
//	g_pEngine = NULL;

	delete g_pEventSync;
	g_pEventSync = NULL;
	
#ifndef BUILD_AS_RHODES_EXTENSION
	delete g_pConfig;
	g_pConfig = NULL;
#endif

	delete g_pWebServer;
	g_pWebServer = NULL;

	//    Commenting out the plugin manager shutdown code resolves this SR.  The
	//  repo steps are not 100% reliable as the issue seems related to timing.  Reproduction is
	//  with auto_common_spec and 
//	delete g_pPlgManager;
//	g_pPlgManager = NULL;
	
	// IMO list can now be freed as the licence module and plug-ins have now finished with IMO
	g_pIMOHead = DeleteIMOList(g_pIMOHead);

	// Close log thread
#ifndef BUILD_AS_RHODES_EXTENSION
	DWORD dwRet = WaitForSingleObject(g_hLogThread,1000);
	if(dwRet == WAIT_TIMEOUT){
		TerminateThread(g_hLogThread,1);
	}
	CloseHandle(g_hLogThread);
	g_hLogThread = NULL;
	DEBUGMSG(1,(L"\nLog Thread closed"));
#endif
	DeleteCriticalSection(&g_PBLoggingCriticalSection);

	SetCursor(LoadCursor(g_hInst, IDC_ARROW));

	//JMS 5/5/2010
	//adding unnattended mode to avoid crashing after suspend during closedown process
	PowerPolicyNotify (PPN_UNATTENDEDMODE, FALSE);


#ifndef BUILD_AS_RHODES_EXTENSION
	//finally destroy the base window
	PostMessage(g_hBaseWnd,WM_QUIT,0,0);
#endif
}



PBERR InitCoreStruct()
{
	LPWSTR pStr;
	
	g_pPBCore = new PBCORESTRUCT;///< allocate space for the core structure
	if(g_pPBCore){
		memset(g_pPBCore,0,sizeof(PBCORESTRUCT));
		
		g_pPBCoreEvents = g_pPBCore->pEventStructure = new PBEVENTSTRUCT;///< allocate space for the event structure
		if(g_pPBCoreEvents){
			memset(g_pPBCoreEvents,0,sizeof(PBEVENTSTRUCT));
	
			//Build the core structure; This is available to each plugin DLL - callback functions
			
			g_pPBCore->pNavigateFunc		= Navigate;			///<the function pointer to the navigate function
			g_pPBCore->pSetPlugProp			= SetPlugProperty;
			g_pPBCore->pCallPlgMethod		= CallPlugMethod;
			g_pPBCore->pRegMessFunc			= RegisterForMessage;
			g_pPBCore->pDeRegMessFunc		= DeRegisterForMessage;
			g_pPBCore->pRegPrimaryMessFunc	= RegisterForPrimaryMessage;
			g_pPBCore->pDeRegPrimaryMessFunc = DeRegisterForPrimaryMessage;
			g_pPBCore->pRegPaintFunc		= RegisterForPaint;
			g_pPBCore->pLoggingFunc			= Log;
			g_pPBCore->pKillApplFunc		= KillApp;
			g_pPBCore->pRegisterForEvent	= RegisterForEvent;
			g_pPBCore->pUnRegisterForEvent	= UnRegisterForEvent;
			g_pPBCore->pBrowserReload		= BrowserReload;
			g_pPBCore->pBrowserStop			= BrowserStop;
			g_pPBCore->pBrowserBack			= BrowserBack;
			g_pPBCore->pBrowserForward		= BrowserForward;
			g_pPBCore->pBrowserQuit			= BrowserQuit;
			g_pPBCore->pBrowserHome			= BrowserHome;
			g_pPBCore->pBrowserResize		= BrowserResize;
			g_pPBCore->pBrowserMininmize	= BrowserMinimize;
			g_pPBCore->pBrowserRestore		= BrowserRestore;
			g_pPBCore->pSipButtonClick		= BrowserSipClick;
			g_pPBCore->pBrowserSetPageZoom	= BrowserSetPageZoom;
			g_pPBCore->pBrowserGetTxtZoom	= BrowserGetTxtZoom;
			g_pPBCore->pBrowserSetTxtZoom	= BrowserSetTxtZoom;
#ifndef BUILD_AS_RHODES_EXTENSION
			g_pPBCore->pBrowserSetAccelerator= BrowserSetAccelerator;
#endif
			g_pPBCore->pCreateImo			= CreateIMO;
			g_pPBCore->pDeleteImo			= DeleteIMO;
			g_pPBCore->pCallImoMethod		= CallIMOMethod;
			g_pPBCore->pSetImoProperty		= SetIMOProperty;
			g_pPBCore->pSetImoCallBack		= SetIMOCallBack;
			g_pPBCore->pGetEmmlObj			= GetEMMLObj;
#ifndef BUILD_AS_RHODES_EXTENSION
			g_pPBCore->pBrowserGetSize		= BrowserGetSize;
			g_pPBCore->pBrowserSetVScroll	= BrowserSetVScroll;
			g_pPBCore->pBrowserSetHScroll	= BrowserSetHScroll;
#endif
			g_pPBCore->pGetComponentVersions	= GetComponentVersions;
			g_pPBCore->pSipControlChange    = SipControlChange;
			g_pPBCore->pDisableScrolling	= DisableScrolling;
			if(!g_pConfig){
				return ERR;
			}
			
			//read/write config.xml settings
			g_pPBCore->pGetGlobalSettingPtr		= GetSettingPtr;
			g_pPBCore->pGetAppSettingPtr		= GetAppSettingPtr;
			
			g_pPBCore->pSetGlobalSetting		= SetGlobalValue;
			g_pPBCore->pSetAppSetting			= SetAppValue;
			
			g_pPBCore->pGetDefaultMetaTagsPtr	= GetDefaultMetaTagsPtr;
			g_pPBCore->pGetAppPreloadsPtr		= GetPreLoadPtr;	
			g_pPBCore->pAddPreLoad				= AddPreload;
			g_pPBCore->pAddDefaultMetaTag       = AddDefaultMeta;

			g_pPBCore->pGetAppCurrURL			= GetCurrURL;
			g_pPBCore->pResizePB				= ResizePB;
			g_pPBCore->hParentWnd				= g_hBaseWnd;
#ifndef BUILD_AS_RHODES_EXTENSION
			g_pPBCore->bBuiltAsRhodesExtension	= false;
#else
			g_pPBCore->bBuiltAsRhodesExtension	= true;
#endif
			
			
			//Create the events needed for synchronisation.
			//this event will trigger when all plug-ins have handled their onAppFocus notification
			//g_pPBCoreEvents->PBAppGotFocusEvent		= CreateEvent(NULL,TRUE,FALSE,NULL);///< signalled when a PB app gains focus

		/*	
			///<signalled when a Browser application loses focus 
			pbCoreEvents.PBAppLostFocusEvent	= CreateEvent(NULL,TRUE,TRUE,NULL);
		*/	
			// signalled when PB is navigating
			//this event will trigger when all plug-ins have handled their onAppFocus notification
			//g_pPBCoreEvents->PBBrowserNavEvent		= CreateEvent(NULL,TRUE,FALSE,NULL);
			
			// signalled when PB is exiting
			g_pPBCoreEvents->PBQuitEvent			= CreateEvent(NULL,TRUE,FALSE,NULL);

			// signalled when PB is logging; this will be created in StartLogger, only if the config file specifies
			g_pPBCoreEvents->PBLogEvent				= NULL;

			
			//this event is created by the syncro object, it is signalled, by the sync object
			//when all registered plugins have handled onAppFocus
			
			
			g_pPBCoreEvents->PBBrowserBeforeNavHandledEvent			= g_pEventSync->CreateSyncEvent(PB_BROWSER_BEFORE_NAV_EVENT);
			if(g_pPBCoreEvents->PBBrowserBeforeNavHandledEvent == NULL){
				return ERR;
			}
			g_pPBCoreEvents->PBBrowserNavCompleteHandledEvent		= g_pEventSync->CreateSyncEvent(PB_BROWSER_NAV_COMPLETE_EVENT);
			if(g_pPBCoreEvents->PBBrowserNavCompleteHandledEvent == NULL){
				return ERR;
			}
			g_pPBCoreEvents->PBBrowserDocumentCompleteHandledEvent	= g_pEventSync->CreateSyncEvent(PB_BROWSER_DOC_COMPLETE_EVENT);
			if(g_pPBCoreEvents->PBBrowserDocumentCompleteHandledEvent == NULL){
				return ERR;
			}
			g_pPBCoreEvents->PBBrowserNavErrorHandledEvent			= g_pEventSync->CreateSyncEvent(PB_BROWSER_NAV_ERROR_EVENT);
			if(g_pPBCoreEvents->PBBrowserNavErrorHandledEvent == NULL){
				return ERR;
			}	
			
			g_pPBCoreEvents->PBAppFocusHandledEvent					= g_pEventSync->CreateSyncEvent(PB_APPFOCUSEVENT);
			if(g_pPBCoreEvents->PBAppFocusHandledEvent == NULL){
				return ERR;
			}

			g_pPBCoreEvents->PBWindowChangedHandledEvent					= g_pEventSync->CreateSyncEvent(PB_WINDOWCHANGED_EVENT);
			if(g_pPBCoreEvents->PBWindowChangedHandledEvent == NULL){
				return ERR;
			}
			
			g_pPBCoreEvents->RhoAppFocusHandledEvent					= g_pEventSync->CreateSyncEvent(RHO_APPFOCUS_EVENT);
			if(g_pPBCoreEvents->RhoAppFocusHandledEvent == NULL){
				return ERR;
			}

			/*
			g_pPBCoreEvents->PBPrimaryMsgHandledEvent				= g_pEventSync->CreateSyncEvent(PB_PRIMARY_MESSAGE_EVENT);
			if(g_pPBCoreEvents->PBPrimaryMsgHandledEvent == NULL){
				return ERR;
			}
			*/
			
			
			//wcscpy(g_pPBCore->szInstallDirectory,L"\\Program Files\\PocketBrowser\\");
			if(GetModuleFileName(NULL, g_pPBCore->szInstallDirectory, MAX_PATH)){
				//trim the file name off the end
#ifndef BUILD_AS_RHODES_EXTENSION
				pStr = wcsstr(g_pPBCore->szInstallDirectory,L"RhoElements.exe");
#else
				pStr = wcsrchr(g_pPBCore->szInstallDirectory, L'\\');
				pStr++;
#endif
				if(pStr) *pStr = NULL;
				return SUCCESS;
			}
			
			 
			
			//return SUCCESS;
		}
	}
			
	delete g_pPBCoreEvents;
	return MEM_ALLOC_ERR;//if it fails return error code
	
}


/**
*	Open the plug-in configuration file 
*	set up the meta class so that it can map the exported LPCTSTR function pointers	
*   and match module names or aliases to the correct exported function; 
*/
PBERR LoadPlugs(LPCTSTR lpPlugsFile)
{
	CXMLReader XML;
	int plugLoop,modLoop,attribLoop,aliasLoop,aliasAttribLoop,iErr= ERR;
	g_pPlgManager = new CMeta();
	LPCTSTR pPlugXml = lpPlugsFile;


	if(!g_pPlgManager){
		return MEM_ALLOC_ERR;
	}
	

	//XML return references
	XML_TAG xPlug,xPlugs,xMod,xAlias;
	XML_ATT xAtt;
	
	//CMeta return references
	PLG_REF plgRef;
	MOD_REF modRef;
	
	if(!_memicmp(pPlugXml,L"FILE://",14)) pPlugXml+=7;
	//this will write the XML file to memory structures
	if((iErr = XML.ParseFile(pPlugXml))!= SUCCESS) return (PBERR)iErr;
	//return SUCCESS;

	xPlugs = XML.GetElement(L"Plugins",0);
	if(!xPlugs){
		return XML_ERR;
	}
	//Now that we have successfully set the XML file
	//Loop through the memory structures
	for(plugLoop = 0;;plugLoop++) //for each plugin
	{								
		xPlug = XML.GetChildElement(xPlugs,L"Plugin",plugLoop);				//get a reference to the next element named 'Plugin'
		if(!xPlug)
			break;
		
		xAtt = XML.GetAttr(xPlug,0);
		if(xAtt)
			if((plgRef = g_pPlgManager->AddPlugin(xAtt->lpValue))==0){			//set the location of the plugin
				Log(PB_LOG_ERROR,L"Duplicate DLL found",L"LoadPlugs",__LINE__,L"Core");
				return ERR;
			}
		for(modLoop = 0;;modLoop++)	//for each module found under the reference plugin						
		{
			//call the function and specify the 'Module' child element to find
			xMod = XML.GetChildElement(xPlug,L"Module",modLoop);
			if(!xMod)
				break;
			
			for(attribLoop = 0;;attribLoop++)	//if we have found a module, iterate through each attrib until we find the name
			{
				xAtt = XML.GetAttr(xMod,attribLoop);
				if(!xAtt)		//if we have reached the last attribute
					break;
				
				
				if(!_wcsicmp(L"name",xAtt->lpName)){
					modRef  = g_pPlgManager->AddModule(plgRef,xAtt->lpValue);
					if(modRef == 0){
						Log(PB_LOG_ERROR,L"Could not add module",L"LoadPlugs",__LINE__,L"Core");
						return ERR;
					}
					continue;
				}
				
				
			}
			
			//get any aliases to the module
			for(aliasLoop = 0;;aliasLoop++)
			{
				xAlias = XML.GetChildElement(xMod,L"Alias",aliasLoop);
				if(!xAlias)		//if we have reached the last attribute
					break;
				for(aliasAttribLoop = 0;;aliasAttribLoop++){//now iterarte through each attribute until we find a name
					xAtt = XML.GetAttr(xAlias,aliasAttribLoop);
					if(!_wcsicmp(L"name",xAtt->lpName)){
						if(!g_pPlgManager->AddAlias(modRef,xAtt->lpValue)){
							Log(PB_LOG_ERROR,L"Could not add alias",L"LoadPlugs",__LINE__,L"Core");
							return ERR;	
						}
						break;
					}
				}
				          
			}
		}// for(modLoop = 0;;modLoop++)	
	}//for plugLoop
	
	return SUCCESS;
	
}


BOOL PageNavigate(int iInstID,LPCTSTR pURI)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	if(g_pEngine->NavigateOnTab(pURI,iInstID))
	{
		LPTSTR pPrompt = new TCHAR[_tcslen(pURI)+ 16];
		if(pPrompt){
			_tcscpy(pPrompt,L"Navigating to: ");
			_tcscat(pPrompt,pURI);
			Log(PB_LOG_INFO,pPrompt,L"Navigate",__LINE__,L"Core");
			delete [] pPrompt;
		}
		return TRUE;
	}
#else
	return g_pNavigateCB(pURI);
#endif


	return FALSE;
}




//not thread safe yet
BOOL Navigate(TCHAR* tcJSONNames[], int iAppID,LPCTSTR lpNavStr,PVARSTRUCT pVARS,LPCTSTR pCallingModule)
{
	if (g_bShuttingDown)
		return FALSE;

	int iTabID = iAppID,iRet;
	//check: is this a IMO Navigate?
	if(iAppID >= PB_APP_ARRSIZE || iAppID < 0){
		IMORec  * pIMO = GetIMOFromID(iAppID);
		if(pIMO){ 
			iTabID = pIMO->iParentInstance;
			if(pIMO->pCBFunc){
				iRet = pIMO->pCBFunc(pVARS,iTabID,pIMO->lParam);//call the function with the parent tabID
				if(iRet == IMO_ERR){
					Log(PB_LOG_ERROR,L"IMO Callback failed",L"Navigate",__LINE__,L"Core");
				}
				else if(iRet == IMO_HANDLED){
					return TRUE;
				}
				
				//to place in signature
				//static LRESULT CALLBACK IMOCBFunc(PVARSTRUCT pVars,int iTABID);
			}
		}
	
	}
		
	//count the vars in the linked list
	//there may be no replacement strings

	//  DCC
	//  Store the parameters list in a string array, this is to avoid having to 
	//  potentially loop through the parameter list for each variable which 
	//  would have made the algorithm O(n squared).
	//  Need a maximum size of the parameter array, loop through the array
	//  first to find the number of params
	WCHAR* NavString = NULL;
	WCHAR FormatString[MAX_URL + 1];

	memset (FormatString, 0, MAX_URL * sizeof (TCHAR));

	wcscpy(FormatString,lpNavStr);
	PVARSTRUCT	pVarStr;
	int iNoParams = 0;
	for(pVarStr = pVARS; pVarStr; pVarStr = pVarStr->pNextVar)///<for each variable
	{
		++iNoParams;
	}
	//  We now know how many parameters there are, create the string array of 
	//  parameters (O (n) complexity still)
	TCHAR** parameterList = new TCHAR*[iNoParams];
	int iParamIter = 0;
	for(pVarStr = pVARS; pVarStr; pVarStr = pVarStr->pNextVar)///<for each variable
	{
//		TCHAR* parameter = new TCHAR[wcslen(pVarStr->pStr) + 1];
//		memset(parameter, 0, sizeof(TCHAR) * (wcslen(pVarStr->pStr) + 1));
		parameterList[iParamIter] = new TCHAR[wcslen(pVarStr->pStr) + 1];
		if (parameterList[iParamIter] == NULL)
			return FALSE;
		memset(parameterList[iParamIter], 0, sizeof(TCHAR) * (wcslen(pVarStr->pStr) + 1));
		wcscpy(parameterList[iParamIter], (LPWSTR)pVarStr->pStr);
		iParamIter++;
	}

	//  Determine whether we want to create the JSON object ourselves (PB / RE1 behaviour)
	//  or whether we are calling back to a ruby function, in which case there is slightly
	//  different logic
	bool bIsRubyCallback = false;
	if (lpNavStr && wcslen(lpNavStr) >= 1 && lpNavStr[0] == '/')
		bIsRubyCallback = true;

	if (!bIsRubyCallback)
	{
		//  Determine if the format string requested by user contains %json
		if (wcslen(FormatString) > wcslen(L"%json") && wcslen(FormatString) < MAXURL &&
			(wcsistr(FormatString, L"%json")))
		{
			//  Copy the text up to %json into the navigation string
			TCHAR* tcPercentJSON = wcsistr(FormatString, L"%json");
			//  Add the bit before '%json' to the navigation string
			int iCharactersAfterPercentJSON = wcslen(tcPercentJSON) - wcslen(L"%json");
			//  User has specified to receive a JSON object
			JSONObject* json = new JSONObject();
			int i = 0;
			int iLengthOfNavString = 0;
			while (tcJSONNames[i] != NULL && parameterList[i] != NULL)
			{
				iLengthOfNavString +=
					json->put(tcJSONNames[i], parameterList[i]);
				iLengthOfNavString += 3;	//  Add some extra characters for the JSON formatting
				if(tcJSONNames[i+1] != NULL)
				{
					if(wcscmp(tcJSONNames[i+1],L"keyCode")==0)
					{
						iLengthOfNavString +=
							json->put(tcJSONNames[i+1], parameterList[i]);
						i++;
						iLengthOfNavString += 3;	//  Add some extra characters for the JSON formatting
					}					
				}
				i++;
			}
			//  Substitute %json into the navigation string
			iLengthOfNavString += wcslen(FormatString);
			iLengthOfNavString += wcslen(L"%json");
			NavString = new WCHAR[iLengthOfNavString + 1];
			wcsncpy(NavString, FormatString, tcPercentJSON - FormatString);
			json->toString(NavString + (tcPercentJSON - FormatString), iLengthOfNavString);
			delete json;

			//  Add the bit after %json to the navigation string
			wcscat(NavString, tcPercentJSON + wcslen(L"%json"));

			//  Delete the Parameter List Array (This is done in PBformat function for
			//  non JSON returns)
			for (int param = 0; param < iNoParams; param++)
			{
				delete [] parameterList[param];
			}
			delete [] parameterList;
		}
		else
		{
			//  User has not specified to receive a JSON object, replace the %s, %number e.t.c.
			NavString = new WCHAR[MAX_URL + 1];
			
			memset(NavString,0,(MAX_URL + 1) * sizeof(wchar_t));
			
			PBformat(FALSE, iNoParams, NavString, FormatString, parameterList);
		}
	}
	else
	{
		//  Data is returned to Ruby as a JSON object
		JSONObject* json = new JSONObject();
		int i = 0;
		int iLengthOfNavString = 0;
		while (tcJSONNames[i] != NULL && parameterList[i] != NULL)
		{
			iLengthOfNavString +=
				json->put(tcJSONNames[i], parameterList[i]);
			iLengthOfNavString += 3;	//  Add some extra characters for the JSON formatting
			if(tcJSONNames[i+1]!=NULL)
			{
				if(wcscmp(tcJSONNames[i+1],L"keyCode")==0)
				{
					iLengthOfNavString +=
						json->put(tcJSONNames[i+1], parameterList[i]);
					i++;
					iLengthOfNavString += 3;	//  Add some extra characters for the JSON formatting
				}					
			}
			i++;
		}
		iLengthOfNavString += wcslen(FormatString);
		iLengthOfNavString += 3;	//  Padding for the extra characters being added below
		NavString = new WCHAR[iLengthOfNavString];
		wcscpy(NavString, FormatString);
		wcscpy(NavString + wcslen(NavString) + 1, L"");
		bool bSuccess = json->toString(NavString + wcslen(NavString) + wcslen(L"") + 1, 
			iLengthOfNavString);
		delete json;

		//  Delete the Parameter List Array (This is done in PBformat function for
		//  non JSON returns)
		for (int param = 0; param < iNoParams; param++)
		{
			delete [] parameterList[param];
		}
		delete [] parameterList;

		if (!bSuccess)
		{
			Log(PB_LOG_WARNING, L"Failed to invoke Ruby callback, URI is too long",
				_T(__FUNCTION__), __LINE__, L"Core");
			delete[] NavString;
			return FALSE;
		}
	}

	//  NavString now contains the formatted return string.
	LRESULT lRes = SendMessage(g_hBaseWnd,PB_NAVIGATETAB,(WPARAM)iTabID,(LPARAM)NavString);
	delete[] NavString;
	return TRUE;
		
}

/**
*  \author:		Darryn Campbell (DCC, JRQ768)
*  \date:		August 2008
*/
void PBformat(BOOL bEscapeAllOutput, int iNoParams, TCHAR *Destination, TCHAR *Format, TCHAR** parameterList) 
{
	if ((Destination == NULL) || (Format == NULL)) goto _Cleanup;;
	if (Format[0] == NULL) goto _Cleanup;;

	int parametersLeftToProcess = iNoParams;
	//  Currently copying-to character in the Destination array
	UINT DestPtr = 0;	
	//  Variable used to keep track of the currently processing parameter, used
	//  to enable a mixiture of %s and %<number>
	UINT currentParameter = 0;
	
	//  Look for %s or %<number> in the Format string and 
	//  replace with the appropriate list argument
	for (DWORD i = 0; i < wcslen(Format); i++)
	{
		//  Handle the character sequence "\%", we wish to replace this in the destination
		//  string with the % character and not action the character after it.
		if (parametersLeftToProcess > 0 && Format[i] == L'\\' && i+1 < wcslen(Format))
		{
			if (Format[i+1] == '%')
			{
				//  We are escaping the % character, copy the % and advance the Dest 
				//  and format character pointers
				Destination[DestPtr++] = Format[i+1];
				i+=2;
			}
		}
		//  If there are parameters left to process and the currently 
		//  considering character is a parameter then process it
		if (parametersLeftToProcess > 0 && Format[i] == L'%')
		{
			//  Replace the parameter with the appropriate value in the destination
			//  string
			//  Check whether format specifier is %s or %<number>
			if (wcslen(&Format[i]) > 1 && (Format[i+1] == L's' || Format[i+1] == L'S')) 
			{
				//  %s format used, replace with next parameter
				if (wcslen(parameterList[currentParameter]) + DestPtr + 1 <= MAXURL)
				{
					DestPtr += CopyString(&Destination[DestPtr], parameterList[currentParameter], false);
					DestPtr += wcslen(parameterList[currentParameter]);
					i++;
				}
				else
				{
					//  Replacing this parameter would exceed the maximum URL
					int length = wcslen(parameterList[currentParameter]);
					break;
				}
			}
			else if (wcslen(&Format[i]) > 1)
			{
				//  %<number> format used, check number is valid
				int replacingParameter = _wtoi(&Format[i+1]);
				if (replacingParameter > 0 && replacingParameter <= iNoParams)
				{
					//  Replace with the specified parameter
					if (wcslen(parameterList[replacingParameter-1]) + DestPtr + 1 <= MAXURL)
					{
						DestPtr += CopyString(&Destination[DestPtr], parameterList[replacingParameter-1], false);
						DestPtr += wcslen(parameterList[replacingParameter-1]);
						TCHAR replacingParameterAsString[5];
						memset(replacingParameterAsString, 0, sizeof(TCHAR) * 5);
						_itow(replacingParameter, replacingParameterAsString, 10);
						i+=wcslen(replacingParameterAsString);
					}
					else
					{
						//  Replacing this parameter would exceed the maximum URL
						break;
					}
				}
				else
				{
					//  character following '%' is out of range or not a number,
					//  copy the '%' character to the destination string
					Destination[DestPtr++] = Format[i];
					continue;
				}
			}
			else
			{
				//  We are processing a '%' at the end of the Format string, do a direct copy
				Destination[DestPtr++] = Format[i];
				continue;
			}

			parametersLeftToProcess--;
			currentParameter++;
		}
		else
		{
			//  The currently considering character does not need to be 
			//  replaced with a parameter.
			if (parametersLeftToProcess == 0)
			{			
				//  No Parameters left to process, copy remainder of String and finish
				CopyString(&Destination[DestPtr], &Format[i], false);				
				break;
			}
			else
			{	
				//  Copy a single character of the format string to the destination.
				Destination[DestPtr++] = Format[i];				
			}
		}	
	}
	  
	   if (DestPtr == wcslen(Format))
           {
              Destination[DestPtr] = L'\0';
           }

	//  If we need to escape the entire output string (replacing all characters 
	//  with %XX) do that now, else just escape the '%' character, replacing it
	//  with %25 (hex)
	if (bEscapeAllOutput)
		Escape(Destination);
	else
	{
		for (int i=0; i < MAXURL; i++)
		{
			//  Test to see if end of copying string reached.
			if (Destination[i] == '\0')
				break;
			//  For each '%' character in the original string replace with %25
			if (Destination[i] == '%' && (wcslen(Destination) < MAXURL - 3))
			{
				TCHAR tcTempDestination[MAXURL];
				memset(tcTempDestination, 0, MAXURL * sizeof(TCHAR));
				wcsncpy(tcTempDestination, Destination, i);
				wsprintf(tcTempDestination, L"%s%c%02x%s", tcTempDestination, 
					'%', (int)Destination[i], Destination+i+1);
				wcscpy(Destination, tcTempDestination);
				i+=2;
			}
		}
	}

_Cleanup:
	//  Delete the Parameter List Array
	for (int param = 0; param < iNoParams; param++)
	{
		delete [] parameterList[param];
	}
	delete [] parameterList;
	return;
}

/**
*  \author:		Darryn Campbell (DCC, JRQ768)
*  \date:		July 2008
*/
void Escape(TCHAR *stringToEscape)
{
	//  Make a copy of the given String
	TCHAR destcpy[MAXURL+1];
 	wcscpy(destcpy, stringToEscape);
	TCHAR characterToCopy;
	//  Declare an additional pointer to the passed string, allows us to iterate over the passed string modifying
	//  it and still keep track of where the beginning of the string was
	TCHAR *stringIter = stringToEscape;

	//  Iterate over the copy of the original string received.  Note that the escaped string will
	//  be 3 * the length of the unescaped string, hence the limit being MAXURL / 3.
	for(int i = 0; i < MAXURL / 3; i++)
	{
		characterToCopy = destcpy[i];
		//  Test to see if end of copying string reached.
		if (characterToCopy == '\0')
			break;
		//  For each character in the original string modify the passed string
		//  to contain have 3 characters, one for a '%' and two to represent the copied character's
		//  ASCII code in hex.
		wsprintf(stringToEscape, L"%c", '%');
		stringToEscape++;
		wsprintf(stringToEscape, L"%02x", (int)characterToCopy);
		stringToEscape+=2;
	}
	//  Just in case the string isn't null terminated though it should be.
	wsprintf(stringToEscape, L"%c", '\0');
	//  Move passed string pointer back to the beginning of the string being escaped.
	stringToEscape = stringIter;
}

/**
*  \author:		Sudhakar Murthy (SM, DBQ486)
*  \authir:		Paul Henderson (PH, PTJK76)
*  \date:		March 2008, SM - Initial Creation
*  \date:		May 2008, PH - The original placed a '\' before the control 
*               character when it needed to place the appropriate break char 
*               such as '\n\r'
*/
UINT CopyString (TCHAR *Dst, TCHAR *Src, bool bJSON)
{
	
	UINT cnt = 0; // spcial char counter
	TCHAR tc = 0;
	while (*Src != L'\0')
	{
		switch(*Src)
		{
		case L'\f':
			tc = L'f';
			break;
		case L'\n':
			tc = L'n';
			break;
		case L'\r':
			tc = L'r';
			break;
		case L'\"':
			tc = L'"';
			break;
		case L'\'':
			//  JSON values containing ' need to have this escaped as they need to be expressed
			//  as strings
			if (bJSON)
				tc = L'\'';
			break;
		case L'\\':
			if (bJSON)
				tc = L'\\';
			break;
		} // end of switch(Src[is])

		if(tc)
		{
			*Dst++	= L'\\';
			*Dst++	= tc;
			cnt++;
		}
		else
			*Dst++	= *Src;

		Src++;
		tc = 0;

	} // end of while (*Src != L'\0')

	*Dst = L'\0'; // NULL string char at the end

	return cnt; // return number of '\\' chars inserted during Src to Dst copy
}



/**
*	Global method that can be called from a module or application for setting plugin properties	
*
*/
BOOL SetPlugProperty(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod,
					 LPCTSTR pParam,LPCTSTR pValue,LPCTSTR pCallingModule)
{
	
	Lock AutoLock(g_pPlgManager->GetCriticalSect());
	
	if(pTargetMod && pValue && pParam){
		return g_pPlgManager->SetPlugProperty(pPBStructure,pTargetMod,pParam,pValue,pCallingModule);
	}
		
	return FALSE;

}

/**
*	Global method that can be called from a module or application for calling plugin methods	
*	
*/
BOOL CallPlugMethod(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod,
					LPCTSTR pMethod,LPCTSTR pCallingModule)
{
	Lock AutoLock(g_pPlgManager->GetCriticalSect());
	
	if(pTargetMod && pMethod){
		return g_pPlgManager->CallPlugMethod(pPBStructure,pTargetMod,pMethod,pCallingModule);
	}
		
	return FALSE;

}

BOOL PreloadModule(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod)
{
	Lock AutoLock(g_pPlgManager->GetCriticalSect());
	if(g_pPlgManager){
		return g_pPlgManager->PreloadModule(pPBStructure,pTargetMod);
	}
	return FALSE;
}


void KillApp(PPBSTRUCT pPBStructure)
{
	
	
	if(g_pAppManager){
		g_pAppManager->LockObj();
		g_pAppManager->RemoveApp(pPBStructure->iTabID);
		g_pAppManager->UnlockObj();
		g_pPlgManager->LockObj();
		g_pPlgManager->RemoveApp(pPBStructure);
		g_pPlgManager->UnlockObj();
	
	}
		
}
const wchar_t RegExPatternsHE[][2][MAXURL] =
{
//  Position
	{
		L"([a-z]+)navigate",		//  Pattern to Match
		L"\\1-navigate",				//  Replace Template
	},	
	
};

#pragma endregion
/*
#pragma region REGEXPAT_CT

const wchar_t RegExPatternsCT[][2][MAXURL] =
{
//  Position
	{
		L"^import;(.*)",		//  Pattern to Match
		L"\\1; import;",				//  Replace Template
	},
	{
		L"^delete;(.*)",		//  Pattern to Match
		L"\\1; delete;",				//  Replace Template
	},
	{
		L"^Javascript:(.*)",		//  Pattern to Match
		L"url('Javascript:\\1');",				//  Replace Template
	},
	{
		L"^http:(.*)",		//  Pattern to Match
		L"url('http:\\1');",				//  Replace Template
	},
	{
		L"^https:(.*)",		//  Pattern to Match
		L"url('https:\\1');",				//  Replace Template
	},
	{
		L"^ftp:(.*)",		//  Pattern to Match
		L"url('ftp:\\1');",				//  Replace Template
	},
	{
		L"([x|X])=([0-9]+)",		//  Pattern to Match
		L"left:\\2",				//  Replace Template
	},	
	{
		L"[Y|y]=([0-9]+)",			//  Pattern to Match
		L"top:\\1",					//  Replace Template
	},
	{
		L"[w|W]=([0-9]+)",			//  Pattern to Match
		L"width:\\1",				//  Replace Template
	},
	{
		L"([h|H])=([0-9]+)",		//  Pattern to Match
		L"height:\\2",				//  Replace Template
	},
	{
		L"url=(.*)",						//  Pattern to Match
		L"url('\\1')",						//  Replace Template
	},
	{
		L"http:$",					//  Pattern to Match ($ means http: is end of string)
		L"http",					//  Replace Template
	},
	{
		L"http:([0-9]+)",			//  Pattern to Match
		L"http\nport:\\1",			//  Replace Template
	},
	{
		L"ftp:$",					//  Pattern to Match
		L"ftp",						//  Replace Template
	},
	{
		L"(ftp):([0-9]+)",			//  Pattern to Match
		L"ftp\nport:\\2",			//  Replace Template
	},
	{
		L"show",					//  Pattern to Match
		L"visibility:visible",		//  Replace Template
	},
	{
		L"hide",					//  Pattern to Match
		L"visibility:hidden",		//  Replace Template
	},
	
	{
		L"COM[0-9]+:",				//  Pattern to Match
		L"&",						//  Replace Template
	},
	//  Colour
	{
		L".*:([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)",
		L"color:#\\1\\2\\3",
	},		
	{
		L".*:([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)",
		L"pencolor:#\\1\\2\\3",
	},
	{
		//  Case Sensitive 
		L"x=([0-9]+)",		//  Pattern to Match
		L"left:\\1",		//  Replace Template
	},
	{
		//  Capitalisation
		L".*:([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)",
		L"color:#\\1\\2\\3",
	}
};

#pragma endregion
*/

//  This method is called when the Engine reports that an Editable field receives focus,
//  i.e. we want to raise the SIP.
LRESULT CALLBACK onFieldFocus(EngineEventID eeID, LPARAM value, int tabIndex)
{
	//  Only show / hide the SIP automatically if the SIP Control has been set to 
	//  automatic
	BOOL bEditableFieldFocus = (BOOL)value;
	g_bEditableFieldFocused = bEditableFieldFocus;

	if (g_sipControlMode == SIP_CONTROL_AUTOMATIC)
	{
		bool bSIPVisible = false;
		HWND sipHWND = FindWindow(L"SipWndClass", NULL);
		if (sipHWND && IsWindowVisible(sipHWND))
			bSIPVisible = true;

		BOOL bEditableFieldFocus = (BOOL)value;

		//  This is the most reliable way I have found of toggling the SIP
		if (bEditableFieldFocus)
		{
			if (!bSIPVisible)	
			{
				m_pSIP->ToggleSIP();
				if (!g_bShuttingDown)
					PostMessage(g_hBaseWnd,PB_ENGINESIPCHANGE,(WPARAM)tabIndex,(LPARAM)NULL);
			}
		}
		else
		{
			if (bSIPVisible)
			{
				m_pSIP->ToggleSIP();
				if (!g_bShuttingDown)
					PostMessage(g_hBaseWnd,PB_ENGINESIPCHANGE,(WPARAM)tabIndex,(LPARAM)NULL);
				ResizePB(FALSE, FALSE);
			}
		}
	}
	
	return S_OK;
}



LRESULT CALLBACK onGeolocation(EngineEventID eeID, LPARAM value, int tabIndex)
{
	if (eeID == EEID_GPS_POSITIONRECEIVED)
		SendMessage(g_hBaseWnd, PB_NEWGPSDATA, 0, 0);
	else if (eeID == EEID_GPS_LOCATIONERROR)
		SendMessage(g_hBaseWnd, PB_GEOLOCERROR, 0, 0);
	return S_OK;
}

void HideSipIfShown()
{
	HWND sipHWND = FindWindow(L"SipWndClass", NULL);
	if (sipHWND && IsWindowVisible(sipHWND))
	{
		m_pSIP->ToggleSIP();
		if (!g_bShuttingDown)
			PostMessage(g_hBaseWnd,PB_ENGINESIPCHANGE,(WPARAM)0,(LPARAM)NULL);
		ResizePB(FALSE, FALSE);
	}

}

LRESULT CALLBACK onConsoleMessage(EngineEventID eeID, LPARAM value, int tabIndex)
{
	Log(PB_LOG_ERROR, (LPCTSTR)value, L"ConsoleLog", 0, L"Engine");
	return S_OK;
}

LRESULT CALLBACK onPopUp(EngineEventID eeID, LPARAM value, int tabIndex)
{
	//  eeID will always be EEID_JSPOPUP_ALERT, EEID_JSPOPUP_CONFIRM, 
	//  EEID_JSPOPUP_PROMPT
	//  Dialog title should be the page title but call to GetTitleOnTab
	//  returns blank string.
	LRESULT lReturnValue = S_OK;
	m_bDialogShown = true;
#ifdef BUILD_AS_RHODES_EXTENSION
	HWND hParent = g_hRhodesWnd;	
#endif
	
	switch (eeID)
	{
	case EEID_JSPOPUP_ALERT:
		{
		CJSPopup* pDialog = new CJSPopup((void*)value, EEID_JSPOPUP_ALERT);
		lReturnValue = pDialog->ShowJSDialog(hParent);
		delete pDialog;
		}
		break;
	case EEID_JSPOPUP_CONFIRM:
		{
		CJSPopup* pDialog = new CJSPopup((void*)value, EEID_JSPOPUP_CONFIRM);
		lReturnValue = pDialog->ShowJSDialog(hParent);
		delete pDialog;
		}
		break;
	case EEID_JSPOPUP_PROMPT:
		CJSPopup* pDialog = new CJSPopup((void*)value, EEID_JSPOPUP_PROMPT);
		lReturnValue = pDialog->ShowJSDialog(hParent);
		delete pDialog;
		break;
	}
	m_bDialogShown = false;
	//SetFocus(g_hBrowserWnd);
	return lReturnValue;
}

//  This function is called when the engine is requesting authentication
LRESULT CALLBACK onAuthenticationRequest(EngineEventID eeID, LPARAM value, int tabIndex)
{
	LRESULT lReturnValue = S_OK;
	//  value contains a pointer to an authentication info structure whose memory
	//  is allocated by the caller (engine)
	EngineJSPopupAuthentication* pAuthenticationInfo;
	pAuthenticationInfo = (EngineJSPopupAuthentication*)value;
	//  Logic is as follows:
	//  1) Look in config file for authentication information
	//  2) If no configuration settings are found then popup a dialog
	LPCTSTR pUsername;
	LPCTSTR pPassword;
	pUsername = g_pConfig->GetAppSettingPtr(tabIndex,L"Authentication\\Username", L"Value");	
	pPassword = g_pConfig->GetAppSettingPtr(tabIndex,L"Authentication\\Password", L"Value");	
	if(pUsername && pPassword)
	{
		if (wcslen(pUsername) <= pAuthenticationInfo->iUsernameLength - 1)
			wcscpy(pAuthenticationInfo->szUsername, pUsername);
		else
		{		
			Log(PB_LOG_WARNING, L"Failed to send Authentication info to server, the username is too long", _T(__FUNCTION__), __LINE__, L"Core");
			lReturnValue = S_FALSE;
		}
		if (wcslen(pPassword) <= pAuthenticationInfo->iPasswordLength - 1)
			wcscpy(pAuthenticationInfo->szPassword, pPassword);
		else
		{		
			Log(PB_LOG_WARNING, L"Failed to send Authentication info to server, the password is too long", _T(__FUNCTION__), __LINE__, L"Core");
			lReturnValue = S_FALSE;
		}
	}
	else
	{
#ifdef BUILD_AS_RHODES_EXTENSION
		HWND hParent = g_hRhodesWnd;	
#endif
		Log(PB_LOG_INFO, L"Authentication username / password were not found in the configuration file, prompting user", _T(__FUNCTION__), __LINE__, L"Core");
		CJSPopup* pDialog = new CJSPopup((void*)value, EEID_JSPOPUP_AUTHENTICATION);
		lReturnValue = pDialog->ShowJSDialog(hParent);
		if (lReturnValue == S_FALSE)
			Log(PB_LOG_WARNING, L"Authentication dialog was dismissed or enterred username / password were too long", _T(__FUNCTION__), __LINE__, L"Core");
		delete pDialog;
	}


	return lReturnValue;
}


LRESULT CALLBACK onNavEvent(EngineEventID eeID, LPARAM value, int tabIndex)
{
	BOOL bOk = FALSE;
	DWORD dwRes = WAIT_FAILED;
	CApp *pApp;
	BadLinkType	eReason = BADLINK_TIMEOUT;
	switch(eeID)
	{
		
	case EEID_BEFORENAVIGATE:
		{
#ifndef BUILD_AS_RHODES_EXTENSION
			if (g_pEngine)
			{	
				if(!g_pEngine->Navigable((LPCTSTR)value))
				{
					//  The Application license applied is preventing the page from 
					//  navigating to the requested URL.
					pApp = g_pAppManager->GetApp(tabIndex);
					if(pApp)
					{
						if(pApp->m_bBadlink)
						{
							PageNavigate(0,pApp->GetBadlink(pApp->m_szCurrentURL, BADLINK_UNLICENSED));
							break;
						}
					}
					break;
				}
			}
#endif
			if (wcslen((LPCTSTR)value) >= wcslen(L"history:") && wcsnicmp((LPCTSTR)value, L"history:", 8) == 0)
			{
				//  We are being instructed to navigate to history:back
				BrowserBack(tabIndex,L"Core",1);
				break;
			}

			//  Hide the SIP if it is shown
			HideSipIfShown();

			g_pAppManager->LockObj();
			g_pAppManager->SetCurrURL(tabIndex,(LPTSTR)value);
			DEBUGMSG(TRUE, (L"Before Navigate (%s)\n", (LPTSTR)value));
			g_pAppManager->UnlockObj();		

			g_pEventSync->LockObj();
			bOk = g_pEventSync->Run(PB_BROWSER_BEFORE_NAV_EVENT,(LPARAM) tabIndex,NULL); 
			g_pEventSync->UnlockObj();
			
			if(bOk){
				dwRes = WaitForSingleObject(g_pPBCoreEvents->PBBrowserBeforeNavHandledEvent,10000); 
				if(dwRes ==WAIT_TIMEOUT){
					Log(PB_LOG_ERROR,
					L"at least one plug-in took too long to handle the onBeforeNavigate notification",
					L"onNavEvent",__LINE__,L"Core");
					return S_FALSE;
				}
			}

		}
			break;
		case EEID_NAVIGATECOMPLETE:
			{

			g_pEventSync->LockObj();
			bOk = g_pEventSync->Run(PB_BROWSER_NAV_COMPLETE_EVENT,(LPARAM) tabIndex,NULL);
			DEBUGMSG(TRUE, (L"Navigate Complete (%s)\n", (LPTSTR)value));
			g_pEventSync->UnlockObj();
			if(bOk){
				dwRes = WaitForSingleObject(g_pPBCoreEvents->PBBrowserNavCompleteHandledEvent,10000); 
				if(dwRes ==WAIT_TIMEOUT){
					Log(PB_LOG_ERROR,
					L"at least one plug-in took too long to handle the onNavigateComplete notification",
					L"onNavEvent",__LINE__,L"Core");
					return S_FALSE;
				}
			}
			HideSIPButton();

			//do default tags for the new page
			SendMessage(g_hBaseWnd,PB_DEFTAGS,(WPARAM)tabIndex,NULL);
			}
			break;
		case EEID_DOCUMENTCOMPLETE:
			{

			g_pEventSync->LockObj();
			bOk = g_pEventSync->Run(PB_BROWSER_DOC_COMPLETE_EVENT,(LPARAM) tabIndex,NULL);
			DEBUGMSG(TRUE, (L"Document Complete (%s)\n", (LPTSTR)value));
			g_pEventSync->UnlockObj();
			if(bOk){
				dwRes = WaitForSingleObject(g_pPBCoreEvents->PBBrowserDocumentCompleteHandledEvent,10000); 
				if(dwRes ==WAIT_TIMEOUT){
					Log(PB_LOG_ERROR,
					L"at least one plug-in took too long to handle the onNavigateDocumentComplete notification",
					L"onNavEvent",__LINE__,L"Core");
					return S_FALSE;
				}
			}
//			PostMessage(g_pAppManager->m_pApp[0]->m_pbStruct.hWnd, WM_SETCURSOR, 0x7C082610, 0x02010001);
			}
			break;
		case EEID_NAVIGATIONERROR:
			DEBUGMSG(TRUE, (L"Nav Error (%s)\n", (LPTSTR)value));
			eReason = BADLINK_NAVERROR;

		case EEID_NAVIGATIONTIMEOUT:
			g_pAppManager->LockObj();
			DEBUGMSG(TRUE, (L"Nav Timeout (%s)\n", (LPTSTR)value));
				if (DetermineJavaScriptAlertShown())
				{
					//  Work around to prevent nav timeouts whilst waiting for user input
					//  Do nothing if there is a popup of any kind visible
				}
				else
				{
					pApp = g_pAppManager->GetApp(tabIndex);
					if(pApp)
					{
						if(pApp->m_bBadlink)
						{
							//  BeforeNavigate tries to Lock the App Manager therefore
							//  release the critical section here
							g_pAppManager->UnlockObj();	
							//  Bad link needs to navigate in a separate thread.
							CloseHandle(CreateThread(NULL, 0, 
								(LPTHREAD_START_ROUTINE) BadLinkNavigateThread,
								(LPVOID) pApp->GetBadlink(pApp->m_szCurrentURL,eReason), 
								0, 0));
							g_pAppManager->LockObj();
						}
						
						else
						{
							//MessageBox(NULL,L"Address unreachable",L"Connection error",MB_OK);
							//BrowserBack(tabIndex,L"Core",0);
							//  Functionality change requested by SAP to avoid form data being deleted
							int iMsgBoxRet = MessageBox(g_hBrowserWnd, L"Unable to Reach Page, Try again?", L"Connection", MB_RETRYCANCEL | MB_ICONQUESTION);
							if (iMsgBoxRet == IDCANCEL)
								onNavEvent(EEID_DOCUMENTCOMPLETE, (LPARAM)value, 0);
							else
							{
								//  Retry
								pApp = g_pAppManager->GetApp(tabIndex);
								CloseHandle(CreateThread(NULL, 0, 
									(LPTHREAD_START_ROUTINE) BadLinkNavigateThread,
									(LPVOID) pApp->m_szCurrentURL, 
									0, 0));
							}
						}
						
					}
				}
			g_pAppManager->UnlockObj();	
			break;
		case EEID_APPLICATIONUNLICENSED:
			//  The Application license applied is preventing the page from 
			//  navigating to the requested URL.
			pApp = g_pAppManager->GetApp(tabIndex);
			if(pApp)
			{
				if(pApp->m_bBadlink)
				{
#ifndef BUILD_AS_RHODES_EXTENSION
					PageNavigate(0,pApp->GetBadlink(pApp->m_szCurrentURL, BADLINK_UNLICENSED));
#else
                    PageNavigate(0,pApp->GetBadlink((LPCTSTR)value, BADLINK_UNLICENSED));
#endif
				}
			}
			break;
	

	}
	return S_OK;
}


LRESULT CALLBACK onTopmostWnd(EngineEventID eeID, LPARAM value, int tabIndex)
{
	//	The engine needs to navigate to a page before it can provide the topmost window.
	//  We cannot call preload until we have a valid window handle
	//	If all of the instances have been loaded then the meta event and navigate event will be registered 
	
	_ASSERT(g_hBaseWnd);
	return SendMessage(g_hBaseWnd,PB_ONTOPMOSTWINDOW,(LPARAM)tabIndex,(WPARAM)value);
		
}






int iLastApp = PB_INVALID_APPID;
LRESULT  CALLBACK onMeta(EngineEventID eeID, LPARAM value, int tabIndex)
{
	return SendMessage(g_hBaseWnd,PB_ONMETA,(WPARAM)tabIndex,value);
}



LRESULT CALLBACK TagProc( LPPBNAMEVALUE pNameVal)
{
	LPTSTR	pModule = pNameVal->szModule,
			pName	= pNameVal->szName,	//used to make the code less cluttered
			pValue	= pNameVal->szValue;
	
	
	
	if(pModule &&  pName && g_pTargetApp){
		if(pValue){
			//since we have a name and value we are setting a property
			if(g_pTargetApp->SetPlugProperty(pModule,pName,pValue))
				return S_OK;
		}
		else{
			
			//else call a method
			if(g_pTargetApp->CallPlugMethod(pModule,pName))
				return S_OK;
		}
		//Log(PB_LOG_ERROR,L"we either don't have a module name/parameter or a function failed",L"TagProc",__LINE__,L"Core");
		return S_FALSE;

	}
	
	Log(PB_LOG_ERROR,L"Missing module name or parameter",L"TagProc",__LINE__,L"Core");
	return S_FALSE;
}



BOOL RegisterForEvent(PBModule *pObj,PB_SyncEvent dwEventID,LPCTSTR pCallingModule)
{
	
	BOOL bRet = FALSE;
	
	if(dwEventID == PB_PRIMARY_MESSAGE){
		return pSyncPrimMsg->RegisterCallback(pObj,pCallingModule);
	}
	
	g_pEventSync->LockObj();		
	bRet = g_pEventSync->RegisterCallback(pObj,dwEventID,pCallingModule);
	g_pEventSync->UnlockObj();
	
	return bRet;
	
}

BOOL UnRegisterForEvent(PBModule *pObj,PB_SyncEvent dwEventID,LPCTSTR pCallingModule)
{
	BOOL bRet = FALSE;

	if(dwEventID == PB_PRIMARY_MESSAGE){
		return pSyncPrimMsg->UnRegisterCallback(pObj);
	}
	
	g_pEventSync->LockObj();
	bRet = g_pEventSync->UnRegisterCallback(pObj,dwEventID);
	g_pEventSync->UnlockObj();
	
	return bRet;
}


BOOL RegisterForMessage (REGISTERMESSAGE *pregister)
{
	g_pMessageManager->AddMap(new CMessageMap (pregister->nAppID,
		pregister->hWnd,
		pregister->uMessage,
		pregister->pModule,
		pregister->pCallback));

#ifndef BUILD_AS_RHODES_EXTENSION
	g_pEngine->RegisterForMessage(pregister->uMessage);
#else
	g_pRegisterForMessageCB(pregister->uMessage);
#endif

	return TRUE;
}

BOOL DeRegisterForMessage (REGISTERMESSAGE *pregister)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	g_pEngine->DeRegisterForMessage(pregister->uMessage);
#else
	g_pDeRegisterForMessageCB(pregister->uMessage);
#endif

	return TRUE;
}

BOOL RegisterForPrimaryMessage (UINT iWMMsgID)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	g_pEngine->RegisterForPrimaryMessage(iWMMsgID);
#else
	g_pRegisterForPrimaryMessageCB(iWMMsgID);
#endif

	return TRUE;
}

BOOL DeRegisterForPrimaryMessage (UINT iWMMsgID)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	g_pEngine->DeRegisterForPrimaryMessage(iWMMsgID);
#else
	g_pDeRegisterForPrimaryMessageCB(iWMMsgID);
#endif

	return TRUE;
}

BOOL RegisterForPaint (REGISTERPAINT *pregister)
{
	g_pPaintManager->AddMap (new CPaintMap (
		pregister->nAppID,
		pregister->hWnd,
		pregister->pModule,
		pregister->pCallback));

	return TRUE;
}



// OnMemoryLogTimeOut is an application-defined callback function that 
// processes WM_TIMER messages. 
 
void CALLBACK OnMemoryLogTimeOut( HWND hwnd,        // handle to window for timer messages 
									UINT message,     // WM_TIMER message 
									UINT idTimer,     // timer identifier 
									DWORD dwTime)     // current system time 
{ 
 
	//check message id
	if(message != WM_TIMER)
		return;

	//check timer id
	if(idTimer != PB_MEMLOGTIMER)
		return;

	LogMemory();

} 

void LogMemory()
{
	//Get the memory stats
	MEMORYSTATUS sMemStat;
	sMemStat.dwLength = sizeof(MEMORYSTATUS);

	GlobalMemoryStatus(&sMemStat); 

	sMemStat.dwTotalPhys /= DIV;
	sMemStat.dwAvailPhys /= DIV;
	sMemStat.dwTotalVirtual /= DIV;
	sMemStat.dwAvailVirtual /= DIV;

	TCHAR Stats[1024];
	wsprintf(Stats, L"Stats: Load=%d%%  TotalPhy=%dKB  FreePhy=%dKB  TotalVM=%dKB  FreeVM=%dKB", 
						sMemStat.dwMemoryLoad, 
						sMemStat.dwTotalPhys, 
						sMemStat.dwAvailPhys,
						sMemStat.dwTotalVirtual,
						sMemStat.dwAvailVirtual);

	//Log the memory stats
	//Function name, caller and line no. not relevant for memory logs
	Log(PB_LOG_MEMORY,Stats,L"",0,L"");
}


errType StartLogger()
{
	int iErr = ERR;
	InitializeCriticalSection(&g_PBLoggingCriticalSection);
	
	memset(&g_bLogOptions,0,sizeof(LogTypeInterface)*6);
	
	//iterate through the logger options and set the enabled types
	LPCTSTR pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogError",L"Value");
	if(pLogOption){
		
		if(pLogOption[0] == L'1'){
			g_bLogOptions[PB_LOG_ERROR] = TRUE;
		}

	}

	pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogWarning",L"Value");
	if(pLogOption){
		if(pLogOption[0] ==L'1'){
			g_bLogOptions[PB_LOG_WARNING] = TRUE;
		}
	}
	pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogInfo",L"Value");
	if(pLogOption){
		if(pLogOption[0] ==L'1'){
			g_bLogOptions[PB_LOG_INFO] = TRUE;
		}
	}
	pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogUser",L"Value");
	if(pLogOption){
		if(pLogOption[0] ==L'1'){
			g_bLogOptions[PB_LOG_USER] = TRUE;
		}
	}
	pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogMemory",L"Value");
	if(pLogOption){
		if(pLogOption[0] ==L'1'){
			g_bLogOptions[PB_LOG_MEMORY] = TRUE;
		}
	}
#ifdef _DEBUG	
	g_bLogOptions[PB_LOG_DEBUG] = TRUE;
#endif

	//set the logger max size in KB
	pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogMaxSize",L"Value");
	if(!pLogOption){
		return XML_ERR;
	}
	g_dwLogMaxSize = _wtoi(pLogOption);
	g_bMaxReached = FALSE;
	

	//set the memory logger frequency (time in milliseconds)
	pLogOption = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogMemPeriod",L"Value");
	
	if(!pLogOption){
		g_dwMemoryLogFreq = 5000; //set default value 
	}
	else{
		g_dwMemoryLogFreq = _wtoi(pLogOption);
	}
	

	//create the log event
	//create this event as signalled so that the LogThread can use it to signal that it is ready
	g_pPBCoreEvents->PBLogEvent	= CreateEvent(NULL,TRUE,TRUE,NULL); 
	
	if(g_pPBCoreEvents->PBLogEvent == NULL || g_pPBCoreEvents->PBQuitEvent == NULL){
		//iErr = NULL_HANDLE_ERR ;
		return NULL_HANDLE_ERR; 
	}
#ifndef BUILD_AS_RHODES_EXTENSION
	g_pLogQueue = new std::queue<stLog>;
	if(g_pLogQueue == NULL){
		return MEM_ALLOC_ERR;
	}
	g_hLogThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) LogThread,(LPVOID) g_pPBCoreEvents, 0, 0);
	if(g_hLogThread== NULL){
		delete g_pLogQueue;
		return THREAD_ERR;
	}
#endif	
	DWORD dwCycles;
	//wait for the LogThread to reset StructOfEvents.PBLogEvent, when it does it signals that the logger is ready
	for(dwCycles = 0;dwCycles < 100;dwCycles++)
	{
		if(WaitForSingleObject(g_pPBCoreEvents->PBLogEvent,0)== WAIT_OBJECT_0){
			Sleep(10);
			continue;
		}
		
		return SUCCESS;//logger has started
		
	}

		
	return TIMEOUT_ERR;
	

}

BOOL Log(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
	LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule)
{
#ifndef BUILD_AS_RHODES_EXTENSION
	LogTypeInterface Severity = logSeverity;

	// Check for quit event
	if (WaitForSingleObject (g_pPBCoreEvents->PBQuitEvent, 0) == WAIT_OBJECT_0)
		return FALSE;

	//if the logger is not running turn the message into a messagebox.
	if(!g_hLogThread){
		if(logSeverity == PB_LOG_ERROR){
			MessageBox(g_hBaseWnd,pLogComment,pFunctionName,MB_OK | MB_TOPMOST);
			return TRUE;
		}
		return FALSE;
	}

	// prevent the release build outputting PB_LOG_DEBUG messages
	#ifndef _DEBUG
		if(logSeverity == PB_LOG_DEBUG)
			return FALSE;
	#endif

	//
	#ifdef _DEBUGLOG
		if(logSeverity != PB_LOG_DEBUG)
			return FALSE;
							
	#else
		if(logSeverity == PB_LOG_DEBUG)
			Severity = PB_LOG_INFO;
		
	#endif
	
	
		
	if(!g_bLogOptions[Severity]){
		return TRUE;//not enabled in the config file
	}

	
	if(!g_pLogQueue)
		return FALSE;
	
	stLog LogStruct;
	
	
	LogStruct.dwLogSeverity		= Severity;
	if(logSeverity == PB_LOG_USER){
		//wcsncpy(LogStruct.szFunctionName,g_pAppManager->GetCurrURL(g_pPBCore->iCurrentInstID),MAX_URL);
		wcsncpy(LogStruct.szFunctionName,g_pAppManager->GetCurrURL(0),MAX_URL);
		wcscpy(LogStruct.szCaller,L"USER");
		LogStruct.dwLogSeverity = dwLineNumber+2;
	}
	else{
		wcscpy(LogStruct.szFunctionName,pFunctionName);
		wcscpy(LogStruct.szCaller,pCallingModule);
		LogStruct.dwLineNumber		= (DWORD)dwLineNumber;
	}
	if(logSeverity == PB_LOG_MEMORY)
	{
		LogStruct.dwLogSeverity += 1; 
	}

	wcscpy(LogStruct.szLogComment,pLogComment);
		
	
	//  only one thread can queue a log at a time
	EnterCriticalSection(&g_PBLoggingCriticalSection);
		g_pLogQueue->push(LogStruct);///< this will add to the log queue and return immediately
	LeaveCriticalSection(&g_PBLoggingCriticalSection); 
	SetEvent(g_pPBCoreEvents->PBLogEvent);//signal a log event
		
#else
	//  Process the Log message in a Rhodes Extension environment
	if (g_pLogCB)
		g_pLogCB(logSeverity, pCallingModule, pLogComment, pFunctionName, dwLineNumber);

#endif
	return TRUE;	

}


#ifndef BUILD_AS_RHODES_EXTENSION
//Open the log file
DWORD WINAPI LogThread(LPVOID lparam)

{
	TCHAR arrSeverity[][10]={L"ERROR:",L"WARNING:",L"INFO:",L"LOW:",L"MEDIUM:",L"HIGH:",L"MEMORY:",};

	WCHAR szLineNum[5],szTime[50];
	SYSTEMTIME sysTime;
	int iLenSev,iLenCaller,iLenFunc,iLenTot,iLenComm,iLenTime,iRet= 1;
	char *pFileData,*pOutStr;
	
	DWORD dwNumBytes,dwBytesToWrite,dwEvent,dwBytesWritten = 0;
	HANDLE	hEvents[2];
	
	PPBEVENTSTRUCT pStructOfEvents = (PPBEVENTSTRUCT)lparam;
	if(pStructOfEvents == NULL)
		return 1;
	
	hEvents[0] = pStructOfEvents->PBLogEvent;
	hEvents[1] = pStructOfEvents->PBQuitEvent;
		
	HANDLE					hFile,hBkupFile=NULL;
	LPCTSTR					pLogBkupFile = L"\\Program Files\\RhoElements\\LogBkup.txt"; // Used as temp only so no Global declaration required.
	CSend					*pHttp = NULL;
	
	
	//std::queue<stLog>		*pLogQueue = NULL;
	stLog					Log;
	if(!g_pLogFile || *g_pLogFile==NULL){//check for a log filename
		iRet = BAD_PARAM;
		goto _exit;
	}
	if(0 == wcsncmp(pLogBkupFile,g_pLogFile,wcslen(g_pLogFile))) //Case to handle in case PB user accidentaly -
		pLogBkupFile = L"\\Program Files\\RhoElements\\BkupLog.txt"; // matches the log-file name in config.xml 
	
	if(g_bFileProtocol){
		hFile = CreateFile (g_pLogFile,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ ,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
		if(hFile == INVALID_HANDLE_VALUE ){
			iRet = FILE_ERR;
			goto _exit;
		}
		
	}
	else{//the config file contains a http URI 
		pHttp = new CSend;
		if(!pHttp){
			iRet = MEM_ALLOC_ERR;
			goto _exit;
		}
		if(!pHttp->Open(g_pLogFile,NULL,NULL)){
			iRet = BAD_PARAM;
			goto _exit;
		}
		LPCTSTR pLogPort;
		if((pLogPort = g_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogPort",L"Value"))==NULL){
			goto _exit;
		}
		WORD nPort = (WORD)_ttoi(pLogPort);
		pHttp->SetPort(nPort);
	}
	


	for(;;)
	{
		dwEvent =  WaitForMultipleObjects( 2, hEvents,FALSE,INFINITE);
		switch(dwEvent - WAIT_OBJECT_0)
		{
			case 0://a log entry has occurred
				ResetEvent(hEvents[0]);
				while (1)//keep looping until the log queue is empty
				{
					EnterCriticalSection(&g_PBLoggingCriticalSection);
					if(g_pLogQueue->empty()){
						LeaveCriticalSection(&g_PBLoggingCriticalSection);
						break;
					}
					Log = g_pLogQueue->front();
					g_pLogQueue->pop();
					LeaveCriticalSection(&g_PBLoggingCriticalSection); 
					
					if(pHttp){ 
						//send to HTTP URI
						pHttp->AddNameValue(L"LogSeverity",(LPCTSTR)arrSeverity[Log.dwLogSeverity]);
						pHttp->AddNameValue(L"LogComment",Log.szLogComment);
						pHttp->AddNameValue(L"FunctionName",Log.szFunctionName);
						pHttp->AddNameValue(L"Caller",Log.szCaller);
						
						pHttp->Send();
						continue;
					}
					
					//else save to file
					if(TRUE == g_bMaxReached)
					{
						DeleteFile(pLogBkupFile);//At first instance bkup file won't exist, however no crashing will happen...
						if(NULL != hFile)
							CloseHandle(hFile);
						MoveFile(g_pLogFile,pLogBkupFile);//This API combination will save space at that point of time...
						hFile = CreateFile (g_pLogFile, 
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ , NULL,CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,NULL);
						if(INVALID_HANDLE_VALUE == hFile ){
							iRet = FILE_ERR;
							goto _exit;
						}
						g_bMaxReached = FALSE;
						dwBytesWritten = 0;
					}
					iLenSev		= _tcslen(arrSeverity[Log.dwLogSeverity]);
					iLenCaller	= wcslen(Log.szCaller);
					iLenFunc	= wcslen(Log.szFunctionName);
					iLenComm	= wcslen(Log.szLogComment);
					
					iLenTot = iLenSev + iLenCaller + iLenFunc + iLenComm + 50;//allow for spaces, the timestring and the newline 
					pFileData = new char[iLenTot];
					if(pFileData == NULL){
						iRet = MEM_ALLOC_ERR;
						break;
					}
					memset(pFileData,0,iLenTot);
					
					pOutStr = pFileData;

					wcstombs( pFileData, arrSeverity[Log.dwLogSeverity],iLenSev);
					//strcpy(pFileData,(char*)arrSeverity[Log.dwLogSeverity]);
					pFileData += iLenSev;
					*pFileData++ = ' ';///<insert space seperator
												
					pFileData+= wcstombs(pFileData,Log.szCaller,iLenCaller);
					*pFileData++ = ' ';///<insert seperator
												
					pFileData+= wcstombs(pFileData,Log.szFunctionName,iLenFunc);
					*pFileData++ = ' ';///<insert seperator
					
					pFileData+= wcstombs(pFileData,Log.szLogComment,iLenComm);
					*pFileData++ = ' ';///<insert seperator
					
					_itow(Log.dwLineNumber,szLineNum,10);//> turn the line number into a string
					pFileData+= wcstombs(pFileData,szLineNum,wcslen(szLineNum));
					*pFileData++ = ' ';///<insert seperator

					GetLocalTime(&sysTime);///<get the date & time
					iLenTime = wsprintf((LPWSTR)&szTime,L"%d-%.2d-%.2d %.2d:%.2d:%.2d"
					,sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
					

					pFileData+= wcstombs(pFileData,szTime,iLenTime);
					*pFileData++ = 0x0D;
					*pFileData = 0x0A;
					++pFileData;
					dwBytesToWrite = pFileData - pOutStr;
					
					
					//iLenTot = strlen(pOutStr);


#ifdef _DEBUG
					//int len;
					//for(len = 0;pOutStr[len]!=0x0A;len++);
					
					WCHAR szlogStr[MAX_PATH + 1];
					mbstowcs(szlogStr,pOutStr,MAX_PATH);
					szlogStr[MAX_PATH]= NULL;
					DEBUGMSG(1, (szlogStr));

#endif
					dwBytesWritten+=dwBytesToWrite;
					if((DWORD)(dwBytesWritten/1000)>= g_dwLogMaxSize){
						g_bMaxReached = TRUE;
					}
					WriteFile(hFile,pOutStr,dwBytesToWrite, &dwNumBytes, NULL);
					delete [] pOutStr;
					pOutStr = NULL;
					//if(!bOk || dwNumBytes != dwBytesToWrite)
					
					
						
				}
				
				break;
			case 1://closethread
				CHAR buffer[513]; //Read cycle of 512 bytes only, so new and delete not required.
				DWORD nBytesRead,nBytesToRead = 512; /*matching the buffer size-1*/
				DWORD dwBkupLogSize,dwExcessLogSize,dwTemp=1;

				hBkupFile = CreateFile (pLogBkupFile,GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//Open only if exist!

				if(INVALID_HANDLE_VALUE != hBkupFile)  
				{										//Backup File Exists so we need to merge 
					SetFilePointer(hFile,0, 0,FILE_BEGIN);
					SetFilePointer(hBkupFile,0, 0,FILE_END);
					while(dwTemp)
					{
						buffer[0] = NULL;
						if(ReadFile(hFile, buffer, nBytesToRead, &nBytesRead, NULL))
							WriteFile(hBkupFile, buffer, nBytesRead, &dwTemp, NULL);
						else
						{
							iRet = FILE_ERR;
							goto _exit;
						}

					}
					if(NULL != hFile)
						CloseHandle(hFile);
					hFile = CreateFile (g_pLogFile, GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ , NULL, CREATE_ALWAYS, //CREATE_ALWAYS :-), takes care to delete/truncate previous log.txt
						FILE_ATTRIBUTE_NORMAL,NULL);
					if(INVALID_HANDLE_VALUE == hFile)
					{
						iRet = FILE_ERR;
						goto _exit;
					}
					dwBkupLogSize = GetFileSize (hBkupFile, NULL);
					dwExcessLogSize = dwBkupLogSize - (g_dwLogMaxSize*1024);
					if(dwExcessLogSize> 0 && dwExcessLogSize < dwBkupLogSize)
					{
						SetFilePointer(hBkupFile,dwExcessLogSize, 0,FILE_BEGIN);//Move file pointer; We will start copying from here. 
						dwTemp =1;
						while(dwTemp)
						{
							buffer[0] = NULL;
							if(ReadFile(hBkupFile, buffer, nBytesToRead, &nBytesRead, NULL))
								WriteFile(hFile, buffer, nBytesRead, &dwTemp, NULL); 
							else
							{
								iRet = FILE_ERR;
								goto _exit;
							}

						}
					}
					else
					{
						if(hFile)
							CloseHandle(hFile);
						CopyFile(pLogBkupFile,g_pLogFile,FALSE);
					}
				}
				iRet = SUCCESS;

				// GD - quit thread
				goto _exit;
		}
	}
			
			
			
		
	
_exit:		
	DeleteCriticalSection(&g_PBLoggingCriticalSection);
	if(hFile){
		CloseHandle(hFile);
	}
	if(hBkupFile){
		CloseHandle(hBkupFile);
		DeleteFile(pLogBkupFile);
		pLogBkupFile = NULL;
	}
	else if(pHttp){
		delete pHttp;
	}
	CloseHandle(pStructOfEvents->PBLogEvent);
	delete g_pLogQueue;
	g_pLogQueue = NULL;
	DEBUGMSG(1,(L"LOG: Exiting Log Thread\n"));
	
	return iRet;
}
#endif




#pragma endregion




#ifndef BUILD_AS_RHODES_EXTENSION
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;
		
	
	//  Ascertain whether or not RhoElements is already running, create
	//  a named event and if that event already exists PB must already be 
	//  running
	SetLastError(0);
	HANDLE hPBRunning = CreateEvent(NULL, TRUE, FALSE, L"SymbolPBRunning");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//  RhoElements is already running, switch to the running window
		CloseHandle(hPBRunning);
		HWND hwndRunningPB = FindWindow(PB_WINDOW_NAME, NULL);
		if (hwndRunningPB)
		{
			//  Found the already running instance
			// GD 8/10/10
			// Remove logging because logger not yet started
			/*Log(PB_LOG_INFO, 
				L"Attempted to launch multiple Browser instances, showing hidden instance", 
				_T(__FUNCTION__), __LINE__, L"Core");*/
			//  Read the command line parameters
			TCHAR tcCommandLineStartPage[MAX_URL];
			memset(tcCommandLineStartPage, 0, MAX_URL * sizeof(TCHAR));
			if (ParseCommandLine(lpCmdLine, tcCommandLineStartPage, g_szConfigFilePath))
			{
				//  Store command line parameter in the registry (as per PB2.2, 
				//  re-evaluate for RhoElements devices)
				HKEY hKey;
				DWORD Disposition;
				RegCreateKeyEx( HKEY_CURRENT_USER, L"Software\\Symbol\\SymbolPB\\Temp", 0, NULL, 0, 0, 0, &hKey, &Disposition ); 
				RegSetValueEx(hKey, L"cmdline", 0, REG_MULTI_SZ, (const BYTE *) tcCommandLineStartPage, (wcslen(tcCommandLineStartPage)+1)*2); 
				RegCloseKey(hKey);
			}
			//  switch to it
			SendMessage(hwndRunningPB, PB_WINDOW_RESTORE, NULL, TRUE);
			SetForegroundWindow(hwndRunningPB);
		}
		return 0;
	}
	
	{
		//get the system OEM string
		TCHAR szPlatform[MAX_PATH+1];
		memset(szPlatform, 0, MAX_PATH*sizeof(TCHAR));
		SystemParametersInfo(SPI_GETOEMINFO, MAX_PATH, szPlatform, 0);

		TCHAR tcMsg[MAX_PATH+1];
		TCHAR tcMsg1[MAX_PATH+1];

		wsprintf(tcMsg, L"RhoElements is not validated to run on this platform.  If you choose to continue, Symbol does not guarantee the software on this device.  For information on validation of this product please contact the helpdesk\nwww.symbol.com/Business/XU-EN/Pages/Contact_Us\n[%s]", szPlatform);
		wsprintf(tcMsg1,L"RhoElements is not validated to run on this platform or it is not a Symbol device.\n[%s]", szPlatform);
		//force to lower case
		for(UINT i=0; i<wcslen(szPlatform); i++)
			szPlatform[i] |= 0x20;
		
		if(wcsstr(szPlatform, L"symbol") || wcsstr(szPlatform, L"Intermec"))
		{
			if(MessageBox(NULL, tcMsg
						   , L"Symbol RhoElements", MB_SETFOREGROUND | MB_TOPMOST | MB_ICONSTOP | MB_OKCANCEL) == IDOK)
			{
				//  Do nothing
				//return TRUE;
			}
			else
				return FALSE;
		}
		else
		{
			MessageBox(NULL, tcMsg1, L"Symbol RhoElements", MB_SETFOREGROUND | MB_TOPMOST | MB_ICONSTOP | MB_OKCANCEL);
			return FALSE;
		}
	}


	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	
	
	if(!Initialise(lpCmdLine)){
		return FALSE;
	}


	

	
	




	HACCEL hAccelTable;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PBCORE));

	//  Engine is initialised above as part of Initialise() therefore 
	//  PreProcessMessage exported function will not be NULL (or if it was 
	//  an error has already been thrown)
	tPB_PreprocessMessage lpPreprocessMessage = g_pEngine->GetlpPreprocessMessage();

	int iIn;
	
	// Main message loop:
	LRESULT preProcRes;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
#ifdef DEBUG
	if (msg.message == WM_KEYDOWN && msg.wParam == 81)
	{
		//  'Q' has been pressed, simulate the app losing focus
		ProcessAppActivate(false);
	}
	if (msg.message == WM_KEYDOWN && msg.wParam == 87)
	{
		//  'W' has been pressed, simulate the app gaining focus
		ProcessAppActivate(true);
	}
#endif
		//check to see if any plugins have registered for primary messages
			if(pSyncPrimMsg)
			{
				iIn = g_pPBCore->iCurrentInstID;
				if(pSyncPrimMsg->RunSync((LPARAM) &msg,(LPARAM)&iIn))
				{
					//this is a blocking function that unblocks when any registered plug-in has handled the message
					if(iIn == PB_HANDLED)
					{
						//if any of the plugins return TRUE, the message is to be swallowed and iIn will contain PB_HANDLED
						continue;
					}
				}
			}


		if (!g_bEditableFieldFocused && msg.message == WM_KEYDOWN && 
			!g_pAppManager->m_bUsingLegacyEngine &&   //The Legacy (IE) Engine doesn't send us the notification for OnFieldFocused
    		msg.wParam == (WPARAM)VK_BACK)
		{
			//  An editable field does not have focus and the back key has been pressed,
			//  ignore the key.
			continue;
		}
		preProcRes = ACCELERATE_KEY_HANDLE;
		if (!g_bShuttingDown)
			preProcRes = lpPreprocessMessage(msg);
		else
			continue;
		//  Allow the browser to process the accelerator keys (most notably Tab)
		//  Do not preProcess the message if we are shutting down as the 
		//  Engine DLL will have been deinitialised

		if (ACCELERATE_KEY_DONT_HANDLE != preProcRes)
		{
			if (preProcRes != ACCELERATE_KEY_DONT_TRANSLATE
			{

				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
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

	return (int) msg.wParam;
}

#endif

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    //HWND hWnd;
	//TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name
    
	
	//LoadString(hInstance, IDI_PBCORE, L"PocketBrowser", MAX_LOADSTRING);

    if (!MyRegisterClass(hInstance,L"RhoElements"))
    {
    	return FALSE;
    }
/*
    g_hBaseWnd = CreateWindow(L"PocketBrowser", L"Core", WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
*/
#ifndef BUILD_AS_RHODES_EXTENSION
	g_hBaseWnd = CreateWindowEx(WS_EX_NOANIMATION,PB_WINDOW_NAME, L"Symbol RhoElements", 
			WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT, NULL,NULL,hInstance, NULL);
#else
	g_hBaseWnd = CreateWindowEx(WS_EX_NOANIMATION,PB_WINDOW_NAME, L"Symbol RhoElements", 
			0,50,50,50,50, NULL,NULL,hInstance, NULL);
#endif

	g_hInst = hInstance; // Store instance handle in our global variable

    if (!g_hBaseWnd)
    {
        int iErr = GetLastError();
		return FALSE;
    }
	
	ShowWindow(g_hBaseWnd, nCmdShow);
    UpdateWindow(g_hBaseWnd);

    return TRUE;
}



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,iTabID;
	LPWSTR pEquiv,pContent;
    static CApp *pApp;
	LPTSTR pStr;
	PPBMETASTRUCT pMeta = NULL;
	PPBSTRUCT	  pPBStruct = NULL;
	EngineMETATag* metaTag = NULL;
	LRESULT rtRes;
#ifndef BUILD_AS_RHODES_EXTENSION
#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
    static SHACTIVATEINFO s_sai;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP
#endif	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
			break;

        case WM_CREATE:
			
            break;
#ifdef BUILD_AS_RHODES_EXTENSION
		case WM_WINDOWPOSCHANGED:
			break;
#endif
#ifndef BUILD_AS_RHODES_EXTENSION
		case WM_ACTIVATE:
			{
				WORD fActive = LOWORD(wParam);
				//  If we are receiving notification that we are the
				//  active window and we are currently minimized then
				//  restore ourselves ( where the
				//  browser was only partially visible after being shown
				//  from task manager
				if (m_bMinimized && fActive != WA_INACTIVE)
					BrowserRestore(0, NULL);
			}
			break;
#endif
		case PB_DEFTAGS:
			iTabID = (int)wParam;
			g_pTargetApp = g_pAppManager->GetApp(iTabID);
			if(g_pTargetApp){
				g_pTargetApp->RunDefaultMetaTags();
				//SetFocus(g_hBrowserWnd);
			}
			break;
		
		case PB_ONMETA:
			g_pTargetApp = g_pAppManager->m_pApp[0];//GetApp(iTabID);
			if(g_pTargetApp){
				metaTag = (EngineMETATag*)lParam;
				g_pTargetApp->m_pbStruct.bInvoked = FALSE;
				g_pTargetApp->m_pEMML->ParseEMMLTag(metaTag->tcHTTPEquiv,metaTag->tcContents,TagProc);
				//SetFocus(g_hBrowserWnd);
				return S_OK; 
			}
			return S_FALSE; 
			

		case PB_NAVIGATETAB:
			
			pStr = (LPTSTR)lParam;
			iTabID = (int)wParam;
			if (!pStr)
				break;
			
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

			if(_memicmp(pStr,L"JavaScript:",11*2) == 0)
			{
#ifndef BUILD_AS_RHODES_EXTENSION
				if(g_pEngine->JavaScript_Exist(iTabID,pStr))
				{
					return g_pEngine->JavaScriptInvoke(iTabID,pStr);
				}
#else
				if (g_pExistsJSCB(pStr))
					return g_pInvokeJSCB(pStr);
#endif
			}
			else{
				return PageNavigate(iTabID,pStr);
			}
		
			break;
		
		case PB_ENGINESIPCHANGE:
			{
				//  Fired when we want to inform WebKit it may want to reposition the webview
#ifndef BUILD_AS_RHODES_EXTENSION
				if (!g_bShuttingDown && g_pAppManager->GetApp(0)->m_iScrollTechnique == SCROLLING_FINGERSCROLL)
					g_pAppManager->m_pEngine->NotifyEngineOfSipPosition();
#else
				//  Inform Rhodes framework to notify engine of SIP Position
				g_pPassSipPositionToEngineCB();
#endif
			}
			break;
		case PB_GEN_ACTIVEX:
			// GD - don't attempt to call plugins while shutting down
			if (g_bShuttingDown)
				break;

			pEquiv = (LPWSTR) wParam;
			pContent = (LPWSTR)lParam;
			
/*	//temporary application switch
#ifdef _DEBUG
			if(!_memicmp(pEquiv,L"application",11*2)){
					if(!_memicmp(pContent,L"switch:",7*2)){
						iAppID = _wtoi(pContent+7);
						g_pAppManager->SwitchAppInst(iAppID);
						break;
						
					}
			}

#endif
*/

			pApp = g_pAppManager->GetApp(0);
			if(pApp){
				pApp->m_pbStruct.bInvoked = TRUE;
				pApp->m_pEMML->ParseEMMLTag(pEquiv,pContent,TagProc);
			}
			
			break;
		case PB_GEN_QUIT://currently quits all instances of PB
			DeInitialise();
			break;
		case PB_SETPLG_PROPERTY:
			
			break;
		case PB_CALLPLG_METHOD:

			break;
		case PB_ONTOPMOSTWINDOW:
			rtRes = g_pAppManager->ProcessOnTopMostWnd((HWND)lParam,(int)wParam); 
			if(rtRes == S_FALSE){
				Log(PB_LOG_ERROR, L"Failed to load RhoElements", _T(__FUNCTION__), __LINE__, L"Core");
				PostMessage(g_hBaseWnd,PB_GEN_QUIT,0,0);//we have not successfully processed the topMostWindow so shutdown
			}
			else
			{
				//  Set the topmost window successfully, store a reference to the browser HWND
				g_hBrowserWnd = g_pAppManager->m_pApp[0]->m_pbStruct.hWnd;
			}
			return rtRes;
				
		case PB_WINDOW_RESTORE:
			//  The window has been restored 
			BrowserRestore(0, NULL);

			break;
		case PB_SCREEN_ORIENTATION_CHANGED:
			//g_bLeftRotated = wParam == TYPEHERE?TRUE:FALSE;//update the global
			//use a thread so that we don't hold up the message pump
			CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WindowChangedThread,(LPVOID) lParam, 0, 0));
			break;

#ifndef BUILD_AS_RHODES_EXTENSION
		case WM_DESTROY:
			PostQuitMessage(0);
            break;
#endif
		case PB_NEWGPSDATA:
			//Notifies the web engine when there are new location data available
#ifndef BUILD_AS_RHODES_EXTENSION
			g_pEngine->SendLocationDatoToEngine();
#else
			g_pGeolocationCB(true);
#endif
			break;
		case PB_GEOLOCERROR:
#ifndef BUILD_AS_RHODES_EXTENSION
			g_pEngine->SendLocationErrorToEngine();
#else
			g_pGeolocationCB(false);
#endif
			break;
#ifndef BUILD_AS_RHODES_EXTENSION
		case WM_SETFOCUS:
			{	
				if (g_hBrowserWnd && !m_bMinimized)
				{
					HWND hWndLostFocus = (HWND)wParam;
					if (hWndLostFocus == g_hBrowserWnd)
						break;
					WCHAR wWindowName[30];
					if (hWndLostFocus && GetClassName(hWndLostFocus, wWindowName, 30))
					{
						//  Allow the following windows to remain having focus, avoids
						//  the bug where we're unable to select anything from a combo 
						//  box
						if (wcscmp(wWindowName, L"PopupWindowClass") == 0)
							break;
					}
					SetFocus(g_hBrowserWnd);
				}
			}
			break;
#endif
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	February 2010 (DCC, Initial Creation)
*/
BOOL ParseCommandLine(LPTSTR lpCmdLine, TCHAR* ptcStartPage, WCHAR* szConfigurationFile)
{
	//  Expect the command line parameter to be in the form:
	//  /S:address for legacy RhoElements applications
	//  /S address will suffice for Browser and above
	//  /C:config file path
	//  /C config file path
	//  First search for the start page
	BOOL bRetVal = FALSE;

	WCHAR* szStartPageStart = NULL;
	if (wcslen(lpCmdLine) > 3 && wcsnicmp(lpCmdLine, L"/s", 2) == 0)
		szStartPageStart = lpCmdLine;
	else
	{
		szStartPageStart = wcsstr(_wcslwr(_wcsdup(lpCmdLine)), L" /s");
		if (szStartPageStart)
			 szStartPageStart += 1;
	}
	WCHAR* szConfigFileStart = NULL;
	if (wcslen(lpCmdLine) > 3 && wcsnicmp(lpCmdLine, L"/c", 2) == 0)
		szConfigFileStart = lpCmdLine;
	else
	{
		szConfigFileStart = wcsstr(_wcslwr(_wcsdup(lpCmdLine)), L" /c");
		if (szConfigFileStart)
			szConfigFileStart += 1;
	}

	if (szStartPageStart)
	{
		//  User has specified a start page, move to the start of their supplied parameter
		//  (which can not start with a ':')
		szStartPageStart += 2;
		while (szStartPageStart[0] == L' ' || szStartPageStart[0] == L':')
			szStartPageStart++;

		//  Now determine the end of their supplied parameter
		WCHAR* szEndOfParameter = wcsstr(_wcslwr(_wcsdup(lpCmdLine)), L" /c");
		int iParameterLength;
		if (szEndOfParameter)
			iParameterLength = wcslen(szStartPageStart) - wcslen(szEndOfParameter);
		else
			iParameterLength = wcslen(szStartPageStart);

		if (iParameterLength < MAX_URL)
		{
			wcsncpy(ptcStartPage, szStartPageStart, iParameterLength);
			bRetVal = TRUE;
		}
	}

	if (szConfigFileStart)
	{
		//  User has specified a start page, move to the start of their supplied parameter
		//  (which can not start with a ':'
		szConfigFileStart += 2;
		while (szConfigFileStart[0] == L' ' || szConfigFileStart[0] == L':')
			szConfigFileStart++;

		if (wcslen(szConfigFileStart) > wcslen(L"file://") && 
			wcsnicmp(szConfigFileStart, L"file://", 7) == 0)
			szConfigFileStart += 7;

		//  Now determine the end of their supplied parameter
		WCHAR* szEndOfParameter = wcsstr(_wcslwr(_wcsdup(lpCmdLine)), L" /s");
		int iParameterLength;
		if (szEndOfParameter)
			iParameterLength = wcslen(szConfigFileStart) - wcslen(szEndOfParameter);
		else
			iParameterLength = wcslen(szConfigFileStart);

		if (iParameterLength < MAX_URL)
		{
			wcsncpy(szConfigurationFile, szConfigFileStart, iParameterLength);
			bRetVal = TRUE;
		}
	}





	return bRetVal;
}


DWORD WINAPI WindowChangedThread(LPVOID lparam)
{
	
	g_pEventSync->LockObj();
	
	BOOL bOk = g_pEventSync->Run(PB_WINDOWCHANGED_EVENT,(LPARAM) lparam,NULL); 
	
	g_pEventSync->UnlockObj();
	
	if(bOk){
		DWORD dwRes = WaitForSingleObject(g_pPBCoreEvents->PBWindowChangedHandledEvent,1000); 
		if(dwRes ==WAIT_TIMEOUT){
			Log(PB_LOG_WARNING,
			L"at least one plug-in took too long to handle the onWindowChanged notification",
			L"WindowChangedThread",__LINE__,L"Core");
			
		}
	}
	return 0;

}

/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	August 2010, Initial Creation (DCC)
*/
DWORD WINAPI BadLinkNavigateThread(LPVOID lparam)
{
	//  Navigate to the specified URL
	PageNavigate(0,(TCHAR*)lparam);
	return 0;
}

/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	August 2010, Initial Creation (DCC)
*/
BOOL DetermineJavaScriptAlertShown()
{
	HWND hwndJSDialog = FindWindow(L"Dialog", L"Scripting Alert");
	if (!hwndJSDialog)
	{
		//  Could not find a dialog window whose title is "Scripting Alert"
		//  Try 'RhoElements'
		hwndJSDialog = FindWindow(L"Dialog", L"RhoElements");
		if (!hwndJSDialog)
			hwndJSDialog = FindWindow(L"REJSPopup", L"RhoElements");
		if (!hwndJSDialog)
		{
			//  Could not find a dialog window whose title is "RhoElements"
			//  Try the page's title
			TCHAR tcPageTitle[MAX_URL];
			memset(tcPageTitle, 0, MAX_URL * sizeof(TCHAR));
#ifndef BUILD_AS_RHODES_EXTENSION
			g_pEngine->GetTitleOnTab(tcPageTitle, MAX_URL, 0);
#else
			g_pGetTitleCB(tcPageTitle, MAX_URL);
#endif
			if (!(wcslen(tcPageTitle) > 0 && FindWindow(L"Dialog", tcPageTitle)))
			{
				return FALSE;
			}
		}
	}
	//  Found a JS dialog window therefore can not close RhoElements
	return TRUE;
}

/**
*  Called by the Rhodes framework when this application gains or loses focus, this
*  is an override of CExtManager's OnAppActivate method
*  \param bActivate true if this application has gained focus, false if it has lost focus
*  Whilst this function is only used in the Rhodes Extension I am also defining it in RE1 code for testing
*/
void ProcessAppActivate(bool bActivate)
{
	//  Set the Focus state for the current application
	g_pTargetApp = g_pAppManager->GetApp(0);
	g_pTargetApp->m_pbStruct.bLaunchingAppHasFocus = bActivate;
	if (g_pEventSync)
	{
		g_pEventSync->LockObj();
		BOOL bOk = g_pEventSync->Run(RHO_APPFOCUS_EVENT, bActivate, 0);
		g_pEventSync->UnlockObj();
		if(bOk)
		{
			DWORD dwRes = WaitForSingleObject(g_pPBCoreEvents->RhoAppFocusHandledEvent, 10000); 
			if(dwRes ==WAIT_TIMEOUT)
			{
				Log(PB_LOG_ERROR,
				L"at least one plug-in took too long to handle the onRhoAppFocus notification",
				_T(__FUNCTION__), __LINE__, L"RhoElements Extension");
			}
		}
	}
}

#ifdef BUILD_AS_RHODES_EXTENSION
BOOL InitialiseREAsExtension(HINSTANCE hInst, HWND hRhodesWnd, HWND hWndWebKit,
							 LPTSTR szCommandLine, NAVIGATECALLBACK *pNavigateCB,
							 EXISTSJSCALLBACK *pExistsJSCB, INVOKEJSCALLBACK *pInvokeJSCB,
							 LOGCALLBACK *pLogCB, GEOLOCATIONCALLBACK *pGeolocationCB,
							 HISTORYFORWARDCALLBACK *pHistoryForwardCB, HISTORYBACKCALLBACK *pHistoryBackCB,
							 REFRESHCALLBACK *pRefreshCB, QUITCALLBACK *pQuitCB,
							 MINIMIZECALLBACK *pMinimizeCB, RESTORECALLBACK *pRestoreCB,
							 RESIZEBROWSERWINDOWCALLBACK *pResizeBrowserWindowCB,
							 ZOOMPAGECALLBACK *pZoomPageCB, ZOOMTEXTCALLBACK *pZoomTextCB,
							 GETTEXTZOOMCALLBACK *pGetTextZoomCB, GETTITLECALLBACK *pGetTitleCB,
							 SETBROWSERGESTURINGCALLBACK *pSetBrowserGesturingCB,
							 SIPPOSITIONCALLBACK *pPassSipPositionToEngineCB,
							 REGISTERFORMESSAGECALLBACK *pRegisterForMessageCB,
							 DEREGISTERFORMESSAGECALLBACK *pDeRegisterForMessageCB,
							 REGISTERFORPRIMARYMESSAGECALLBACK *pRegisterForPrimaryMessageCB,
							 DEREGISTERFORPRIMARYMESSAGECALLBACK *pDeRegisterForPrimaryMessageCB)
{
	//  Populate the callback pointers
	g_hRhodesWnd		= hRhodesWnd;
	g_hBrowserWnd       = hWndWebKit; //should be initialized to avoid crash if m_bBadlink = false in OnNavEvent NAVERROR case
	g_pNavigateCB		= pNavigateCB;
	g_pExistsJSCB		= pExistsJSCB;
	g_pInvokeJSCB		= pInvokeJSCB;
	g_pLogCB			= pLogCB;
	g_pGeolocationCB	= pGeolocationCB;
	g_pHistoryForwardCB = pHistoryForwardCB;
	g_pHistoryBackCB	= pHistoryBackCB;
	g_pRefreshCB		= pRefreshCB;
	g_pQuitCB			= pQuitCB;
	g_pMinimizeCB		= pMinimizeCB;
	g_pRestoreCB		= pRestoreCB;
	g_pResizeBrowserWindowCB	= pResizeBrowserWindowCB;
	g_pZoomPageCB		= pZoomPageCB;
	g_pZoomTextCB		= pZoomTextCB;
	g_pGetTextZoomCB	= pGetTextZoomCB;
	g_pGetTitleCB		= pGetTitleCB;
	g_pSetBrowserGesturingCB	= pSetBrowserGesturingCB;
	g_pPassSipPositionToEngineCB	= pPassSipPositionToEngineCB;
	g_pRegisterForMessageCB	= pRegisterForMessageCB;
	g_pDeRegisterForMessageCB	= pDeRegisterForMessageCB;
    g_pRegisterForPrimaryMessageCB = pRegisterForPrimaryMessageCB;
    g_pDeRegisterForPrimaryMessageCB = pDeRegisterForPrimaryMessageCB;

	HWND hWebkitEngineWindow = hWndWebKit;

	if (!InitInstance(hInst, SW_HIDE)) {return FALSE;}
	
	if(!Initialise(szCommandLine)){return FALSE;}

	if (hWebkitEngineWindow)
	{
		//SendMessage(g_hBaseWnd,PB_ONTOPMOSTWINDOW,(LPARAM)0,(WPARAM)hWebkitEngineWindow);
	}
	else
		return FALSE;

	return TRUE;
}

LRESULT SendRhoElementsOneMessage(DWORD message, WPARAM wParam, LPARAM lParam, bool bSend)
{
	if (bSend)
		return SendMessage(g_hBaseWnd, message, wParam, lParam);
	else
		return PostMessage(g_hBaseWnd, message, wParam, lParam);
}

bool ProcessWindowsMessage(MSG& msg)
{
	//  This is the same code as is executed in the message pump
//	WCHAR logMsg[512];
//	wsprintf(logMsg, L"WINDOWS MESSAGE: %i - %i - %i", msg.message, msg.lParam, msg.wParam);
//	Log(PB_LOG_INFO, logMsg, L"WINDOWS MESSAGE", 123, L"Core");

	const char *xmlConfigType   = rho_wmimpl_get_webengine();
	if (g_pAppManager)
	{
		g_pAppManager->HTMLWndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}

	if(pSyncPrimMsg)
	{
		int iIn = 0;//g_pPBCore->iCurrentInstID;
		if(pSyncPrimMsg->RunSync((LPARAM) &msg,(LPARAM)&iIn))
		{
			//this is a blocking function that unblocks when any registered plug-in has handled the message
			if(iIn == PB_HANDLED)
			{
				//  message is swallowed
				return true;
			}
		}
	}

	
	if (g_pAppManager &&
	!g_bEditableFieldFocused && msg.message == WM_KEYDOWN && 
	!g_pAppManager->m_bUsingLegacyEngine &&   //The Legacy (IE) Engine doesn't send us the notification for OnFieldFocused
  		msg.wParam == (WPARAM)VK_BACK && !strncmp(xmlConfigType,"Webkit",6))
	{
		//  An editable field does not have focus and the back key has been pressed,
		//  ignore the key.
		return true;
	}
	

	return false;
}

bool ProcessHTMLMessage(MSG& msg)
{
	HDC hdc;
	if (msg.message == WM_PAINT)
	{
		if ((hdc = GetDC (msg.hwnd)) != NULL)
		{
			if(g_pPaintManager)
				g_pPaintManager->Process (msg.hwnd, hdc);

			ReleaseDC (msg.hwnd, hdc);
		}
	}
	return false;
}

#endif
