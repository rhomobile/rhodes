#pragma once

#include <windows.h>
#include "..\Private\EMML.h"
#include "..\..\PBCore\PBCore\Error.h"
#include "..\..\PBCore\PBCore\Config.h"
#include "..\Private\sip.h"




///////////////////////////////////////////////////
///                                               
///              Global Defines                   
///												 
///////////////////////////////////////////////////

// Message sent if rendered height or scroll position of current page changes
// LOWORD (wparam) = vertical position, HIWORD (wparam) = height
// LOWORD (lparam) = horizontal position, HIWORD (lparam) = width
#define PBM_SCROLL_CHANGE	(WM_USER + 64)

#define PB_NAVIGATETAB					WM_USER	+ 1
#define PB_GEN_ACTIVEX					WM_USER	+ 2
#define PB_GEN_QUIT						WM_USER + 3
#define PB_INIT							WM_USER	+ 4
#define PB_SETPLG_PROPERTY				WM_USER	+ 5
#define PB_CALLPLG_METHOD				WM_USER + 6
#define PB_DEFTAGS						WM_USER + 7
#define PB_ONMETA						WM_USER + 8
#define PB_ONTOPMOSTWINDOW				WM_USER + 9
#define PB_WINDOW_RESTORE				WM_USER + 10
#define PB_SCREEN_ORIENTATION_CHANGED	WM_USER + 11
#define PB_NEWGPSDATA					WM_USER + 12
#define PB_GEOLOCERROR					WM_USER + 13
#define PB_REGISTER_MESSAGE				WM_USER + 14
#define PB_DEREGISTER_MESSAGE			WM_USER + 15

//  Defining this here as existing Scanner Plugin defines this event at the Scanner Level
//  It's the Windows message sent to retrieve BT connected / disconnected events
#define WM_SCAN_EVENT					WM_USER + 20
#define WM_USER_CONNECTION_MANGER_STATUS	WM_USER + 30
#define PB_ENGINESIPCHANGE				WM_USER + 31



#define MAXURL 2049			///< The maximum allowable length of a URL in Internet Explorer		
#ifndef MAX_URL
	#define MAX_URL MAXURL	///< Maximimum allowable length of a URL in Internet Explorer
#endif



#define MAXMODLEN 100		///< Maximum length of the module name


#define CONF_FILE			L"Config.xml"
#define PB_LOADPAGE			L"LoadPage.html"
#define PB_BIN_INSTDIR		L"Bin\\"
#define PB_NPAPI_INSTDIR	L"NPAPI\\"
#define PB_PLUGIN_INSTDIR	L"Plugin\\"
#define PB_CONFIG_INSTDIR	L"Config\\"	
#define PB_HTML_INSTDIR		L"HTML\\"
#define PB_IMAGE_INSTDIR	L"Image\\"
#define PB_ENG_DLL			L"WTG_Engine.dll"
#define	PB_WINDOW_NAME		L"RhoElements"


/*
*	Application IDs
*
*/
#define IMOS_PER_APP		50 
#define PB_APP_ARRSIZE		5 //size of the array to hold PB applications; used in CAppManager 
#define PB_INVALID_APPID	(PB_APP_ARRSIZE)*(PB_APP_ARRSIZE+IMOS_PER_APP)//set this higher than the valid range 
#define PB_LICENCE_SCREEN   -2

#define PB_MEM_ERR_STR	L"Memory Allocation Failure"
#define APP_NAME L"Symbol RhoElements"	///< Ap name used as the user agent string in the HTTP Protocol


/*
typedef struct IMO_RET{
	int		iIndex;	
	LPARAM	lParam;
	IMO_RET *pNext;
}ImoRet,*pImoRet;
*/
typedef long IMOREF;


#define PB_HANDLED -100

//  Named Windows

//  Windows Mobile browser parent window classname.
#define DISPLAYCLASS L"DISPLAYCLASS"
//  Window that appears in MPA2.0 confirming action when power key is pressed
#define POWERKEY_ACTION L"PowerKey Action"

//  End Named Windows

/*
#define PB_WINRESTORED		1
#define PB_WINMINIMIZED		2
#define PB_WINROTATE_LEFT	3
#define PB_WINROTATE_RIGHT	4
*/
enum PB_SyncEvent
{
	PB_APPFOCUSEVENT = 1,
	PB_BROWSER_BEFORE_NAV_EVENT,
	PB_BROWSER_NAV_COMPLETE_EVENT,
	PB_BROWSER_DOC_COMPLETE_EVENT,
	PB_BROWSER_NAV_ERROR_EVENT,
	PB_WINDOWCHANGED_EVENT,
	PB_PRIMARY_MESSAGE,
	RHO_APPFOCUS_EVENT

};

/**
*  Modes in which RhoElements can be displayed
*/
enum PB_ScreenMode
{
	PB_NO_TASKBAR = 1,			///< Full Screen (Taskbar hidden)
	PB_WITH_TASKBAR,			///< Full Screen (Taskbar shown)
	PB_NOT_SHOWN				///< Minimized or shutting down
};


/**
*  Friendlier definitions for the screen orientations compared
*  to the Microsoft definitions in wingdi.h
*/
enum SCREEN_MODE
{
	PORTRAIT	= DMDO_0,	///< Portrait (Normal) Mode
	RIGHT_HANDED= DMDO_90,	///< 90 Degrees Clockwise
	UPSIDE_DOWN	= DMDO_180,	///< Upside Down
	LEFT_HANDED	= DMDO_270,	///< 90 Degrees Counter Clockwise
	NOT_SUPPORTED,			///< Internal Representation that Screen rotation is not supported on the running device.
	PB_WINRESTORED,			///< Window has been restored from minimized state
	PB_WINMINIMIZED			///< Window has been minimized
};



#define PB_MAXTHREADTIMEOUT 1000
#define PB_MEMLOGTIMER 1

enum PBNavEventID
{
	PB_BEFORENAVIGATE = 0,	///< onBrowserNav
	PB_NAVIGATECOMPLETE,
	PB_NAVIGATIONTIMEOUT,
	PB_NAVIGATIONERROR
};




/**
 * used by Dispose function in the base module
 */
enum INITRET
{
	OK,
	INITIALISED,
	DEINITIALISED,
	FAILED
};



/**
 * Enumeration to specify the severity of the logged data
 */
enum LogTypeInterface {
	PB_LOG_ERROR	= 0,	///< Show stopper
	PB_LOG_WARNING,	///< PB Can continue with reduced functionality
	PB_LOG_INFO,		///< General Information, e.g. Plugin X loaded
	PB_LOG_USER,
	PB_LOG_DEBUG,	///< the only message to show during debug
	PB_LOG_MEMORY
};

/**
*  Possible values for handling Accelerator Keys.  An accelerator key performs
*  some special browser behaviour, for example the backspace key will navigate
*  to the previous page if Accelerator keys are set to ALL or the tab key 
*  will cycle through the controls on a form unless Accelerator keys are set to
*  OFF.
*/
enum AcceleratorValue
{
	ACCELERATE_NOT_IMPLEMENTED = -2, ///<  Accelerator Keys are not implemented in this engine
	ACCELERATE_NORM = -1,			///< 'Normal' Accelerator Key Behaviour (as defined by RhoElements)
	ACCELERATE_ON = 1,				///< All Accelerator Keys are enabled
	ACCELERATE_OFF = 0				///< All Accelerator Keys are disabled.
};

/**
*  Values returned from the engine specifying whether or not the pressed
*  accelerator key (back, tab etc.) should be handled.  Special processing
*  is required for backspace on WM6.5.
*/
enum AccelerateKeyHandling
{
	ACCELERATE_KEY_HANDLE = S_FALSE,	///<  The Accelerator key should be handled
	ACCELERATE_KEY_DONT_HANDLE = S_OK,	///<  The Accelerate key should not be handled
	ACCELERATE_KEY_DONT_TRANSLATE = 2,	///<  Backspace pressed on WM6.5 (handle this a special way)
};

enum eScrollTechnique
{
	SCROLLING_NONE = 0,
	SCROLLING_SCROLLBARS = 1,
	SCROLLING_FINGERSCROLL = 2,
};

//  Forward Declaration
class PBModule;

/******************************************************************************/
/**
 *  Structure used by modules to register an interest in windows messages
 */
typedef BOOL (MESSAGECALLBACK) (PBModule*, int, HWND, UINT, WPARAM, LPARAM);

struct REGISTERMESSAGE
{
	int nAppID;
	HWND hWnd;
	UINT uMessage;
	PBModule *pModule;
	MESSAGECALLBACK *pCallback;
};

#define REGISTER_ANY_WINDOW		((HWND) 0)

/******************************************************************************/
/**
 *  Structure used by modules to register an interest in painting the window
 */
typedef void (PAINTCALLBACK) (PBModule*, int, HDC);		// GD

struct REGISTERPAINT
{
	int nAppID;
	HWND hWnd;
	PBModule *pModule;
	PAINTCALLBACK *pCallback;
};



/**
 *  Used to send multiple strings to the core
 */
typedef struct VarsStruct{
	LPCTSTR		pStr;
	VarsStruct	*pNextVar;
}VARSTRUCT,*PVARSTRUCT;


typedef int (CALLBACK* IMOEVENTPROC)(PVARSTRUCT pVars,int iTABID,LPARAM lParam);


/**
 * Structure passed to Modules on Initialisation and property/method invocation.  
 * Used to define PB environment
 * This will evolve as the module design progresses
 */
typedef struct PBStruct
{
	HWND			hWnd;				///<  Handle to the instance topmost window 
	HINSTANCE		hInstance;			///<  Main window instance
	BOOL			bInvoked;			///<  Whether the associated Meta Tag has been manually invoked
	int				iTabID;				///<  PB Application ID
	BOOL			bLaunchingAppHasFocus;	///<  Whether the application initialising the plugin currently has focus
	
}PBSTRUCT,*PPBSTRUCT;


///////////////////////////////////////////////////
///                                               
///              IMO Defines                   
///		  (Internal Module Object)								 
///////////////////////////////////////////////////

struct IMORec{
	int				iParentInstance;
	void			*pMod;	
	PBSTRUCT		PBStructure;
	IMOEVENTPROC	pCBFunc;
	LPARAM			lParam; //< used with SetIMOCallBack to store references
	IMORec			*pNext;
};


#define IMO_ERR		0
#define IMO_HANDLED 1
#define IMO_PASS_ON 2

/**
*  Class to encapsulate version information for a particular component.
*/
class CVersion
{
public:
#ifndef BUILD_AS_RHODES_EXTENSION
	CVersion(void);				///<  Constructor
	~CVersion(void);			///<  Destructor only deletes memory associated with this node.
#endif
	int	iMajorVer;				///<  Major Release version number
	int iFeatureVer;			///<  Feature Release version number
	int iMaintenanceVer;		///<  Maintenance Release version number
	int iReleaseCandidateVer;	///<  Release Candidate number
	int iHotFixVer;				///<  Hot Fix version number
	TCHAR* tcComponentName;		///<  Name of the component whose version information is contained.
	CVersion* pNext;			///<  Pointer to the next CVersion* in this linked list
};

UINT CopyString (TCHAR *Dst, TCHAR *Src, bool bJSON);



/**
 *  Callback Function pointers
 */
typedef BOOL	(*pSetPlugProperty)(PPBSTRUCT,LPCTSTR,LPCTSTR ,LPCTSTR ,LPCTSTR );	
typedef BOOL	(*pCallPlugMethod)(PPBSTRUCT,LPCTSTR ,LPCTSTR,LPCTSTR);	
typedef BOOL	(*pRegMessFunc)(REGISTERMESSAGE*);				///<Register for message notification function type.
typedef BOOL	(*pDeRegMessFunc)(REGISTERMESSAGE*);
typedef BOOL	(*pRegPrimaryMessFunc)(UINT);				///<Register for primary message notification function type.
typedef BOOL	(*pDeRegPrimaryMessFunc)(UINT);
typedef BOOL	(*pRegPaintFunc) (REGISTERPAINT*);
typedef BOOL	(*pNavigateFunc)(TCHAR*[], int,LPCTSTR,PVARSTRUCT,LPCTSTR);	
typedef BOOL	(*pMessHandler)(LPCTSTR,MSG *,long *);			
typedef BOOL	(*pLogFunc)(LogTypeInterface,LPCTSTR,LPCTSTR,DWORD,LPCTSTR);
typedef BOOL	(*pLogFunc)(LogTypeInterface,LPCTSTR,LPCTSTR,DWORD,LPCTSTR);
typedef void	(*pKillAppFunc)(PPBSTRUCT);
typedef BOOL	(*pBrowserFunc)(int,LPCTSTR);
typedef BOOL	(*pBrowserResizeFunc)(int id, LPCTSTR module, int left, int top, int width, int height);
typedef BOOL	(*pBrowserFunc2)(int,double,LPCTSTR);//browser function with additional parameter; double
typedef BOOL	(*pBrowSetPageZoom)(int,double,LPCTSTR);
typedef double	(*pBrowGetPageZoom)(int,LPCTSTR);//browser function with additional parameter; double
typedef BOOL	(*pBrowSetTxtZoom)(int,int,LPCTSTR);
typedef DWORD	(*pBrowGetTxtZoom)(int,LPCTSTR);
typedef BOOL	(*pBrowSetAccelerator)(AcceleratorValue);
typedef BOOL	(*pBrowserReloadFunc)(int,BOOL,LPCTSTR);
typedef BOOL	(*pRegForEvent)(PBModule *,PB_SyncEvent,LPCTSTR);
typedef BOOL	(*pUnRegForEvent)(PBModule *,PB_SyncEvent,LPCTSTR);
typedef IMOREF	(*pCreateIMO)(PPBSTRUCT,LPCTSTR,LPCTSTR);
typedef BOOL	(*pDeleteIMO)(IMOREF);
typedef BOOL	(*pSetIMOProperty)(IMOREF,LPCTSTR,LPCTSTR);
typedef BOOL	(*pCallIMOMethod)(IMOREF,LPCTSTR);
typedef BOOL	(*pSetIMOCallBack)(IMOREF,IMOEVENTPROC,LPARAM);
typedef CEMML*	(*pGetEMMLObj)(int);
typedef LPCTSTR	(*pGetAppPreload)(int,int);
typedef LPCTSTR	(*pGetDefaultMetaTags)(int,int);
typedef LPCTSTR	(*pGetGlobalSet)(LPCTSTR,LPTSTR);
typedef LPCTSTR	(*pGetAppSet)(int,LPCTSTR,LPTSTR);
typedef BOOL	(*pSetGlobalSet)(LPCTSTR,LPCTSTR);
typedef BOOL	(*pSetAppSet)(int,LPCTSTR,LPCTSTR);
typedef BOOL	(*pAddPreload)(int,LPCTSTR);
typedef BOOL	(*pAddDefaultMeta)(int,LPCTSTR);
typedef LPCTSTR	(*pGetCurrentURL)(int);
typedef BOOL	(*pResizePBFunc)(BOOL, BOOL);
typedef BOOL	(*pBrowserGetSizeFunc)(int, int*, int*);
typedef BOOL	(*pBrowserScrollFunc)(int, int);
typedef int		(*pGetComponentVersionsFunc)(TCHAR*, CVersion*);
typedef BOOL    (*pSipControlChangeFunc)(SIP_CONTROL_VALUES newVal);
typedef BOOL	(*pDisableScrollingFunc)(BOOL bDisableScrolling);


/**
* The event structure for synchronisation between the core and plug-in
* 
*
*/
typedef struct stEvent{
	HANDLE PBLogEvent;								///<signalled when a log takes place
	HANDLE PBQuitEvent;								///<signalled when PB is shutting down
	HANDLE PBAppFocusHandledEvent;					///<set by the core when all plugins have handled a registered onAppFocus
	HANDLE PBBrowserBeforeNavHandledEvent;			///<set by the core when all plugins have handled a registered onNavigateComplete
	HANDLE PBBrowserNavCompleteHandledEvent;		///<
	HANDLE PBBrowserDocumentCompleteHandledEvent;	///<
	HANDLE PBBrowserNavErrorHandledEvent;
	HANDLE PBWindowChangedHandledEvent;
	HANDLE RhoAppFocusHandledEvent;
	
}PBEVENTSTRUCT,*PPBEVENTSTRUCT;


  


/**
 * Structure passed to Preload on Initialisation of a module instance.  
 * Used to to provide implementaion for built in function, which ultimately post back to the core 
 * 
 * 
 */
typedef struct PBCoreStruct
{
	/*
	*	function pointers
	*/
	pSetPlugProperty	pSetPlugProp;
	pCallPlugMethod		pCallPlgMethod;
	pNavigateFunc		pNavigateFunc;
	pRegMessFunc		pRegMessFunc;
	pDeRegMessFunc		pDeRegMessFunc;
	pRegPrimaryMessFunc	pRegPrimaryMessFunc;
	pDeRegPrimaryMessFunc pDeRegPrimaryMessFunc;
	pRegPaintFunc		pRegPaintFunc;
	pLogFunc			pLoggingFunc;
	pKillAppFunc		pKillApplFunc;
	pBrowserReloadFunc	pBrowserReload;
	pBrowserFunc		pBrowserStop;
	pBrowserFunc		pBrowserBack;
	pBrowserFunc		pBrowserForward;
	pBrowserFunc		pBrowserHome;
	pBrowserFunc		pBrowserQuit;
	pBrowserFunc		pBrowserMininmize;
	pBrowserFunc		pBrowserRestore;
	pBrowserResizeFunc	pBrowserResize;
	pBrowSetPageZoom	pBrowserSetPageZoom;
	pBrowGetPageZoom	pBrowserGetPageZoom;
	pBrowSetTxtZoom		pBrowserSetTxtZoom;
	pBrowGetTxtZoom		pBrowserGetTxtZoom;
	pBrowSetAccelerator	pBrowserSetAccelerator;
	pRegForEvent		pRegisterForEvent;
	pUnRegForEvent		pUnRegisterForEvent;
	pCreateIMO			pCreateImo;
	pDeleteIMO			pDeleteImo;
	pSetIMOProperty		pSetImoProperty;
	pCallIMOMethod		pCallImoMethod;
	pSetIMOCallBack		pSetImoCallBack;
	pGetEMMLObj			pGetEmmlObj;
	pGetGlobalSet		pGetGlobalSettingPtr;
	pGetAppSet			pGetAppSettingPtr;
	pSetGlobalSet		pSetGlobalSetting;
	pSetAppSet			pSetAppSetting;
	pGetDefaultMetaTags	pGetDefaultMetaTagsPtr;
	pGetAppPreload		pGetAppPreloadsPtr;
	pAddPreload			pAddPreLoad;
	pAddDefaultMeta		pAddDefaultMetaTag;
	pGetCurrentURL		pGetAppCurrURL;
	pBrowserGetSizeFunc	pBrowserGetSize;
	pBrowserScrollFunc	pBrowserSetVScroll;
	pBrowserScrollFunc	pBrowserSetHScroll;
	pGetComponentVersionsFunc	pGetComponentVersions;
	pSipControlChangeFunc pSipControlChange;
	pDisableScrollingFunc	pDisableScrolling;
	pBrowserFunc		pSipButtonClick;
	
	/*
	*	Core Events 
	*/
	PPBEVENTSTRUCT		pEventStructure;
	
	int					iCurrentInstID;
	int					iPreviousInstID;
	WCHAR				szInstallDirectory[MAX_PATH + 1];
	pResizePBFunc		pResizePB;
	HWND				hParentWnd;
	bool				bBuiltAsRhodesExtension;
	

}PBCORESTRUCT,*PPBCORESTRUCT;





/**
 * Structure used by MetaProc function.  
 * 
 * This may evolve as the module design progresses
 * EG: <META HTTP-Equiv="signaturecapture" Content="w=200">
 *
 */
typedef struct PBMetaStruct
{
	LPCTSTR			lpParameter;		///<  Parameter of a meta tag
	LPCTSTR			lpValue;			///<  Value of a meta tag		
										
}*PPBMETASTRUCT;





typedef struct PBInstStruct
{
	int				instID;
	PBInstStruct	*pNext;
	void			*pWrappedObj;
}*PPBINSTSTRUCT;


typedef BOOL (*pMetaProcFunc)(PPBMETASTRUCT,PPBSTRUCT);
typedef BOOL (*pPreLoadFunc)(PPBSTRUCT,PPBCORESTRUCT);
typedef BOOL (*pDisposeFunc)(PPBSTRUCT);

