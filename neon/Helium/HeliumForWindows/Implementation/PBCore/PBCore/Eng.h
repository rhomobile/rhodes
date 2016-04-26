//! \author		Paul Henderson(PH, PTJK76)
//! \date		April 2014 (PH: Initial Creation)
//! \file Eng.h
//! \brief Contains the declaration of the CEng class. Abstract class.
//!
#pragma once



#define MAX_TABS 30
#define PHYSMEM_LIMIT_XML L"Configuration\\TabInstance\\NewTabPhysicalMemLimit"
#define VIRTMEM_LIMIT_XML L"Configuration\\TabInstance\\NewTabVirtualMemLimit"
#define TAB_INIT_WAIT_TIME 60000
#define MAX_TAB_NAME_LEN 10


//logging option config paths
#define ENGINELOG_ERROR_XML		L"Configuration\\Logger\\LogError"
#define ENGINELOG_WARNING_XML	L"Configuration\\Logger\\LogWarning"
#define ENGINELOG_INFO_XML		L"Configuration\\Logger\\LogInfo"
#define ENGINELOG_USER_XML		L"Configuration\\Logger\\LogUser"


//as Newtab is a blocking function we have a timeout. 
#define NEWTAB_IPC_ERR	-1
#define NEWTAB_TIMEOUT	-2	// the new tab call timed out  
#define NEWTAB_LOW_MEM	-3 // A tab cannot be created due to hitting the memory limit m_iMemLimit
#define NEWTAB_MAX_TABS -4 // we have hit the #defined max tabs value defined in Engine2Core; MAX_TABS
#define DEFAULT_PHYSMEM_LIMIT 100
#define DEFAULT_VIRTMEM_LIMIT 100

#define		MAX_CONFIG_STR_LEN	200


#include "..\..\Engine\common\pbengine_defs.h"
#include "..\..\Engine\common\Engine2Core.h"
#include "..\..\PBCore\PBCore\Config.h"

#define ENGINELOG(s,m) EngineLog(s, m,_T(__FUNCTION__), __LINE__)


typedef BOOL (CALLBACK* ENGINEMESSAGEPROC)(int iTabID,LPMSG pMsg);
typedef BOOL (CALLBACK* ENGINE_PRIMARY_MESSAGEPROC)(int iTabID, LPMSG pMsg);

/**
 * Enumeration to specify the severity of the logged data
 */
//enum LogType {
//	LOG_ERROR	= 0,	///< Show stopper
//	LOG_WARNING,	///<  Can continue with reduced functionality
//	LOG_INFO,		///< General Information
//	LOG_USER,
//	LOG_DEBUG,	///< the only message to show during debug
//	LOG_MEMORY
//};

//LRESULT CALLBACK GetAppConfig(int iInstID, LPCTSTR tcSetting, WCHAR* tcValue);
//LPCWSTR GetConfig(LPCTSTR tcSetting, WCHAR* tcValue);


class CEng
{
public:
	CEng(HWND hParentWnd,CConfig *pConfig);//,BYTE LogMask = 0);
	//CEng(HWND hParentWnd,CConfig *pConfig,ENGINELOGPROC Log);//constructor for in process engines. Note no log mask
	
	virtual ~CEng(void);
	bool isWebkitInProcess;
	
	virtual BOOL RegisterForMessage(UINT uMsgId)=0;//should be handled in the derived class as implementation is different for In v Out process
	virtual BOOL SetNavigationTimeout(DWORD dwTimeout)=0;
	virtual int  NewTab()=0;
	virtual BOOL SwitchTab(int iNewTab)=0;
//implemented in CEng	
	BOOL	RegisterMessageHandler(ENGINEMESSAGEPROC pCBMsgProc) { return ((m_MessageProc = pCBMsgProc) != NULL); };
	BOOL	RegisterPrimaryMessageHandler(ENGINE_PRIMARY_MESSAGEPROC pCBPrimaryMsgProc) { return ((m_PrimaryMessageProc = pCBPrimaryMsgProc) != NULL); };
	
	//!
	//!
	//! \author		Paul Henderson (PH,PTJK76)
	//! \date		March 2013 (PH: Initial Creation)
	//! \ 
	
	BOOL	Navigable(LPCTSTR szURL);
	
	//****************************implemented by derived class**************************************/
	virtual BOOL ReloadOnTab (BOOL bFromCache, UINT iTab)=0;
	virtual BOOL ResizeOnTab(int iInstID,RECT rcNewSize)=0;

	virtual BOOL start(int iID,LPWSTR pEnginePath)=0;
	BOOL Start(LPWSTR pEnginePath);
	
	//tPB_PreprocessMessage	GetlpPreprocessMessage() { return & CEngInProcWK::preprocessMessage; };

	virtual BOOL RegisterForEvent(EngineEventID EventID,ENGINEEVENTPROC pCBFunc)=0;

	virtual BOOL NavigateOnTab (LPCTSTR szURL, UINT iTab)=0;

	virtual BOOL SetScrollingThreshold(unsigned int iThreshold)=0;
	virtual BOOL JavaScriptInvoke(int iInstID,LPCTSTR pfunctionJS)=0;
	virtual BOOL SetVScrollOnTab (int scroll, int tab)= 0;
	virtual BOOL SetHScrollOnTab (int scroll, int tab)= 0;
	virtual BOOL ZoomTextOnTab (TextZoomValue ZoomLevel, UINT iTab)=0;
	virtual BOOL ZoomPageOnTab (double dFactor, UINT iTab)=0;
	virtual BOOL BackOnTab(int iInstID,int iPagesBack = 1)=0;
	virtual BOOL ForwardOnTab(int iInstID)=0;
	virtual BOOL Shutdown()=0;
	virtual HWND GetHTMLWND(int iInstID)=0;
	virtual BOOL RegisterForPrimaryMessage(UINT uMsgId)=0;
	virtual BOOL StopOnTab (int iTabID)=0;
	virtual BOOL CloseTab(int iID)=0;
	
	
	virtual LRESULT SendLocationDatoToEngine()=0;
	virtual LRESULT SendLocationErrorToEngine()=0;
	virtual BOOL	GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab)=0;
	virtual BOOL	GetTextZoomOnTab (TextZoomValue* pZoomLevel, UINT iTab)=0;
	virtual BOOL	JavaScript_Exist(int iInstID,LPCTSTR pfunctionJS)=0;
	virtual BOOL	SetBrowserGesturing(BOOL bBrowserGesturing)=0;
	virtual BOOL	NotifyEngineOfSipPosition()=0;
	virtual BOOL	DeRegisterForMessage(UINT uMsgId)=0;
	virtual BOOL	DeRegisterForPrimaryMessage(UINT uMsgId)=0;
	



	virtual BOOL RegisterForInitComplete(ENGINEEVENTPROC pCBFunc)=0;//does nothing on in process engine

	
	static BOOL	EngineLog(LogType logSeverity,LPCTSTR pLogComment, LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule=L"Engine");
protected:
    virtual void onInitialisationComplete();
	//  Sets the Out of Process Engine executable to be non-pageable, if appropriate on the current device.  This eliminates the performance
	//  issue on CE6 device caused by a pageable Engine
	virtual BOOL ConfigureEnginePaging(LPCWSTR pEnginePath);
	virtual BOOL SetPageable(LPCWSTR pEnginePath,BOOL bSet);
	
	//static long				preprocessMessage(MSG msg) { return ACCELERATE_KEY_HANDLE; };

	WNDPROC				m_OwnerProc;

#ifndef BUILD_AS_RHODES_EXTENSION
	static BOOL				messageHandler(LPMSG pMsg, int tabIndex, void* lparam);

	static LRESULT			onDialog(EngineEventID eeID, LPARAM value, int tabIndex);
#endif

	BOOL                    DoCallback(EngineEventID iEventID,LPARAM pParam,int iInstID);

	WCHAR                   *m_szPreNavString;

    BOOL                    m_bInitialisationComplete;

    BOOL                    m_bNavEventsRegComplete;

    HANDLE                  m_hInstanceEvent;

	
	

	int						m_iVirtualLimit;
	int						m_iPhysicalLimit;

	//an array of handles to HTML windows
	HWND m_arrHTMLWindows[MAX_TABS];
	
	
	ENGINEMESSAGEPROC			m_MessageProc;
	ENGINE_PRIMARY_MESSAGEPROC	m_PrimaryMessageProc;
	
	//static WNDPROC	m_OwnerProc;
	CConfig * m_pConfig;

	BoolSettingValue m_bScrollBars;

	//ENGINELOGPROC m_Log;
	

	BOOL availableMemory();
};