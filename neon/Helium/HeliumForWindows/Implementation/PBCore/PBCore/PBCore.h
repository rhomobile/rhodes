#pragma once
#ifndef PBCORE_H
#define PBCORE_H
#define PBERR	errType



#ifdef STANDARDSHELL_UI_MODEL
	#include "resource.h"
#endif
//#include <assert.h>
#include "..\..\Common\Public\PB_Defines.h"
#include "..\..\Common\Private\GenericHeader.h"

//
#include "Eng.h"
#include <queue>
//#include "XMLReader.h"
#include "Meta.h"
#include "AppManager.h"
#include "Message.h"
#include "Paint.h"
#include "Generic.h"
#include "Sync.h"
#include "SyncMsg.h"
#include "Send.h"
#include "Webserver.h"
#ifdef BUILD_AS_RHODES_EXTENSION
//  Typedefs of functions called back into Rhodes from this Extension
typedef BOOL (NAVIGATECALLBACK) (const wchar_t*);
typedef BOOL (EXISTSJSCALLBACK) (const wchar_t*);
typedef BOOL (INVOKEJSCALLBACK) (const wchar_t*);
typedef void (LOGCALLBACK)		(int, const wchar_t*, const wchar_t*, const wchar_t*, int);
typedef void (GEOLOCATIONCALLBACK)		(bool);
typedef void (STOPCALLBACK)				();
typedef void (HISTORYFORWARDCALLBACK)	();
typedef void (HISTORYBACKCALLBACK)		();
typedef void (REFRESHCALLBACK)			(BOOL);
typedef void (QUITCALLBACK)				();
typedef void (MINIMIZECALLBACK)			();
typedef void (RESTORECALLBACK)			();
typedef void (RESIZEBROWSERWINDOWCALLBACK)	(RECT);
typedef void (ZOOMPAGECALLBACK)			(float);
typedef void (ZOOMTEXTCALLBACK)			(int);
typedef int (GETTEXTZOOMCALLBACK)		();
typedef void (GETTITLECALLBACK)			(wchar_t*, int);
typedef void (SETBROWSERGESTURINGCALLBACK) (BOOL);
typedef void (SIPPOSITIONCALLBACK)		();
typedef BOOL (REGISTERFORMESSAGECALLBACK) (unsigned int);
typedef BOOL (DEREGISTERFORMESSAGECALLBACK) (unsigned int);
typedef BOOL (REGISTERFORPRIMARYMESSAGECALLBACK) (unsigned int);
typedef BOOL (DEREGISTERFORPRIMARYMESSAGECALLBACK) (unsigned int);
#endif

BOOL	BrowserReload			(int iAppID,BOOL bCacheLoad,LPCTSTR pCallingModule); 
BOOL	BrowserStop				(int iAppID,LPCTSTR pCallingModule);
BOOL	BrowserBack				(int iAppID,LPCTSTR pCallingModule);
BOOL	BrowserBack				(int iAppID,LPCTSTR pCallingModule,int iPagesBack);
BOOL	BrowserQuit				(int iAppID,LPCTSTR pCallingModule);
BOOL	BrowserHome				(int iAppID,LPCTSTR pCallingModule);
BOOL	BrowserResize			(int id, LPCTSTR module, int left, int top, int width, int height);
BOOL	BrowserMinimize			(int iAppID,LPCTSTR pCallingModule);
BOOL	BrowserRestore			(int iAppID,LPCTSTR pCallingModule);
double  BrowserGetPageZoom		(int iAppID,LPCTSTR pCallingModule);
BOOL	BrowserSetPageZoom		(double fZoomFactor,int iAppID,LPCTSTR pCallingModule);

DWORD	BrowserGetTxtZoom		(int iAppID,LPCTSTR pCallingModule);///< returns the current text zoom factor
BOOL	BrowserSetTxtZoom		(int Val,int iAppID,LPCTSTR pCallingModule);

BOOL	BrowserGetSize			(int appid, int *pwidth, int *pheight);

BOOL	BrowserSetVScroll		(int appid, int scroll);
BOOL	BrowserSetHScroll		(int appid, int scroll);
/**
*  Sets the Accelerator Key mode to the new value, ALL, NORM or OFF.
*  \param eAcceleratorValue mode to set the Accelerator keys to.
*  \return TRUE if successfully set.
*/
BOOL	BrowserSetAccelerator	(AcceleratorValue eAcceleratorValue);
BOOL	RegisterForMessage		(REGISTERMESSAGE *pregister);
BOOL	DeRegisterForMessage	(REGISTERMESSAGE *pregister);
BOOL	RegisterForPrimaryMessage (UINT iWMMsgID);
BOOL	DeRegisterForPrimaryMessage (UINT iWMMsgID);
BOOL	SetPlugProperty			(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod,LPCTSTR pParam,LPCTSTR pValue,LPCTSTR pCallingModule);
BOOL	CallPlugMethod			(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod,LPCTSTR pMethod,LPCTSTR pCallingModule);
void	KillApp					(PPBSTRUCT pPBStructure);
BOOL	RegisterForPaint		(REGISTERPAINT *pregister);


/**********************************************************************************************
* Global Functions
**********************************************************************************************/
BOOL	Initialise		();///>return 0 if no error
BOOL	Initialise(LPTSTR lpCmdLine);
void	DeInitialise	();///>return 0 if no error
PBERR	OpenConfigFile	(LPCTSTR lpConfigFilePath);
PBERR	InitCoreStruct	();
PBERR	LoadPlugs		(LPCTSTR lpPlugsFile);//< Loads the plugin table referencing the XML file: lpPlugsFile
BOOL	Log				(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);
void	HideSipIfShown	();

BOOL	PreloadModule(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod);
LPWSTR	replace(LPWSTR pSzTarget,LPWSTR replaceStr);
CEMML	*GetEMMLObj(int iTabID);
LPCTSTR GetCurrURL(int iAppID);


/**
*  Retrieve the version information from the program, including core exe version, plugin
*  version and the versions of the individual Plugin DLLs.
*  \param tcFileVersion [out] The released version of the product (first 4 digits of the
*  version string, e.g. v.w.x.y)
*  \param listVersions [out] Must be called with a CVersion object which will act as the
*  head of a linked list which, after calling, will contain all the component version information 
*  present in the system.  Note the caller is responsible for freeing the memory associated
*  by this method.
*  \return The length of tcFileVersion.  Call this method with tcFileVersion NULL to still 
*  obtain the length of the string to allocate.
*/
int GetComponentVersions(TCHAR* tcFileVersion, CVersion* listVersions);

/**
*  Inform the core that there has been a change to the SIP Control Meta Tag
*/
BOOL SipControlChange(SIP_CONTROL_VALUES newVal);

/**
*  Populates the version information for the specified component.  This is the full product
*  version (v.w.x.y.z), not the 4 digit file version returned by GetComponentVersions().
*  \param tcFileName Absolute path to the component whose version is to be retrieved.
*  \param listVersions either a single CVersion object or a list of CVersion objects this method
*  will create a new CVersion object which is appended to the end of the linked list.
*  \return true if the version information was successfully obtained.
*/
BOOL GetProductVersionString(TCHAR* tcFileName, CVersion* listVersions);

/**
*  Function which will be called by plug-ins whenever the device's screen 
*  orientation has been manually changed.  This enables Browser to resize
*  its self to fit to the new screen dimensions.
*  \return TRUE
*/
BOOL	ResizePB(BOOL bAccountForSIP, BOOL bAccountForIgnoreSettingChange);




/**
*  Converts the command line parameter (currently just the specified start page)
*  from the string specified by the user into a file or URL which can be 
*  passed to the browser component.  Currently the start page provided on the 
*  command line is assumed to apply to all running Browser applications.
*  \param lpCmdLine The command line parameters as passed to the application on start
*  \param ptcStartPage [out] web page specified as the start page in the command line.
*		  This will be unchanged if the command line parameter was invalid.
*  \return Whether or not there was a valid command line parameter present.
*/
BOOL	ParseCommandLine(LPTSTR lpCmdLine, TCHAR* ptcStartPage, WCHAR* szCommandLine);
LRESULT CALLBACK TagProc( LPPBNAMEVALUE pNameVal);




/**
*	The following functions are provided to the plugin as function pointers
*	They will also be called from Application  objects - CApp
*
*/
BOOL PageNavigate				(int iInstID,LPCTSTR pURI);
BOOL Navigate					(TCHAR* tcJSONNames[], int iAppID,LPCTSTR lpNavStr,PVARSTRUCT pVARS, LPCTSTR pCallingModule = NULL);
BOOL RegisterForEvent			(PBModule *pObj,PB_SyncEvent dwEventID,LPCTSTR pCallingModule);
BOOL UnRegisterForEvent			(PBModule *pObj,PB_SyncEvent dwEventID,LPCTSTR pCallingModule);

//IMO: Internal Module Object
// will return a reference to a new instance of module or NULL if module cannot be found
IMOREF	CreateIMO		(PPBSTRUCT pPBStructure,LPCTSTR pTargetModuleName,LPCTSTR pCallingModName);
BOOL	DeleteIMO		(IMOREF IMORef);	
BOOL	SetIMOProperty	(IMOREF IMORef,LPCTSTR pParam,LPCTSTR pValue);
BOOL	CallIMOMethod	(IMOREF IMORef,LPCTSTR pMethod);
BOOL    SetIMOCallBack  (IMOREF IMORef,IMOEVENTPROC IMOEventProc,LPARAM lParam);
//core functions not available in the core structure
IMORec  *DeleteIMOList	(IMORec * pIMORec);
IMORec  *GetIMOFromID(int iTabID);


/*********************************************************************************************/
//		Read and write to config.xml; Used by the plugins
/*********************************************************************************************/
BOOL	SetAppValue				(int iAppID,LPCTSTR pXmlPath,LPCTSTR pValue);
BOOL	SetGlobalValue			(LPCTSTR pXmlPath, LPCTSTR pValue);
BOOL	AddPreload				(int iAppID,LPCTSTR pPreloadStr);
BOOL	AddDefaultMeta			(int iAppID,LPCTSTR metaTag);
LPCTSTR GetSettingPtr			(LPCTSTR pXmlPath,LPTSTR pAttName);
LPCTSTR GetAppSettingPtr		(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName);
LPCTSTR GetDefaultMetaTagsPtr	(int iAppIndex,int iMetaIndex);
LPCTSTR GetPreLoadPtr			(int iAppID,int iPreLoadIndex);








/**
 *  Function to replace insertion specifiers with passed strings.
 *  This function mimics the behaviour of wsprintf by replacing instances
 *  %s and %[number] with the passed optional parameters, which are TCHARs.
 *  Calling function is responsible for allocating the parameterList but this 
 *  function will delete the allocated memory.
 *  \param bEscapeOutput Whether the output string should be 'escaped', 
 *         i.e. all characters output as the string '%' followed by the Hex 
 *         equivilent of their ASCII characters.  This allows barcodes containing 
 *         character sequences such as '...%de...' to be processed correctly.
 *  \param iNoParams The number of parameters in the 'parameterList' TCHAR**
 *  \param Destination An out parameter where the transformed string will be copied to
 *  \param Format The input string to be parsed
 *  \param parameterList Pointer to array of strings which comprise the substitution values
 */
void PBformat(BOOL bEscapeOutput, int iNoParams, TCHAR *Destination, TCHAR *Format, TCHAR** parameterList);
void BlockNanoSliders(BOOL bBlockSliders);

/**
 *  Function to convert the given string to its equivilent in Hex ASCII.  
 *  Upto a max length of MAXURL e.g. Escape('hi world'); will convert the 
 *  passed string to '%%68%%69%%20%%77%%6f%%72%%69%%64' 
 *  \param stringToEscape The string to be converted into Hex ASCII.  
 */
void Escape(TCHAR* stringToEscape);

/**
*  Inserts '\' char before special chars like '\n', '\r' etc while copying 
*  chars from Src to Dst to fix the Javascript errors.
*/
UINT CopyString (TCHAR *Dst, TCHAR *Src);

/**
 *
*/
void InitGpsInterfaceToEngine();

BOOL ModuleMessage				(MSG *pMsg);

DWORD WINAPI LogThread(LPVOID lparam);
DWORD WINAPI WindowChangedThread(LPVOID lparam);
/**
*  Navigates resulting from a BadLink call need to be invoked from a separate 
*  thread otherwise the navigation does not succeed.
*  \param lparam void* of the String to navigate to
*  \return 0
*/
DWORD WINAPI BadLinkNavigateThread(LPVOID lparam);

LRESULT CALLBACK onMeta(EngineEventID eeID, LPARAM value, int tabIndex);
LRESULT CALLBACK onNavEvent(EngineEventID eeID, LPARAM value, int tabIndex);
LRESULT CALLBACK onTopmostWnd(EngineEventID eeID, LPARAM value, int tabIndex);
LRESULT CALLBACK onFieldFocus(EngineEventID eeID, LPARAM value, int tabIndex);
//LRESULT CALLBACK onPopUp(EngineEventID eeID, LPARAM value, int tabIndex);
LRESULT CALLBACK onGeolocation(EngineEventID eeID, LPARAM value, int tabIndex);
LRESULT CALLBACK onAuthenticationRequest(EngineEventID eeID, LPARAM value, int tabIndex);
/*
*	Starts the logger
*
*/
PBERR StartLogger();
void CALLBACK OnMemoryLogTimeOut( HWND hwnd,UINT message,UINT idTimer,DWORD dwTime);  
void LogMemory();

/**
*  Determines whether or not there is a JavaScript alert shown.  Uses a series
*  of expected window titles to determine whether or not the dialog is
*  present, e.g. by default the JS alert title will be the page title.
*  \return TRUE if there is a JS alert shown, else False
*/
BOOL DetermineJavaScriptAlertShown();

#ifdef BUILD_AS_RHODES_EXTENSION
//  These functions are added specifically to interface with Rhodes
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
							 DEREGISTERFORPRIMARYMESSAGECALLBACK *pDeRegisterForPrimaryMessageCB);
LRESULT SendRhoElementsOneMessage(DWORD message, WPARAM wParam, LPARAM lParam, bool bSend);
bool ProcessWindowsMessage(MSG& msg);
bool ProcessHTMLMessage(MSG& msg);
#endif
void ProcessAppActivate(bool bActivate);


struct stLog{
	DWORD	dwLogSeverity;//Info, erro, warning, user 
	DWORD	dwLineNumber;//line number 
	WCHAR	szLogComment[MAX_URL + 1];
	WCHAR	szFunctionName[100]; 
	WCHAR	szCaller[50];//application or module name
	//WCHAR	szTime[20];//2009-12-19 17:55
	 
};




// Global Variables:





#endif