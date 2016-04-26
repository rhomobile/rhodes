/**
 * \file PBPlugin.h
 * \brief File designed to be imported by Browser Plugins.  
 * Contains definitions and common
 * structures to be used throughout the plugin architecture.  Plugins are implemented as
 * a group of module classes, each of which should inherit from the PBModule abstract base class.
 * The module classes should be wrapped and the wrapped functions exported from the DLL.
 */

#pragma once
#include <windows.h>


///////////////////////////////////////////////////
///                                               
///              Global Defines                   
///												 
///////////////////////////////////////////////////


#define MAXURL 2049			///< The maximum allowable length of a URL in Internet Explorer		
#ifndef MAX_URL
	#define MAX_URL MAXURL	///< Maximimum allowable length of a URL in Internet Explorer
#endif

#define MAXMODLEN 100		///< Maximum length of the module name
#define PB_INVALID_APPID	-1  
///////////////////////////////////////////////////
//                                               //
//                Logging                        //
//												 //
///////////////////////////////////////////////////





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
 * General error codes
 */
enum errType
{
	SUCCESS,				
	ERR,
	MEM_ALLOC_ERR,			///< error dynamically allocating memory
	FILE_ERR,				
	ERR_DLL_LOAD,			///< error dynamically loading a dll
	DLL_ERR,
	TIMEOUT_ERR,
	NULL_HANDLE_ERR,
	NULL_PTR,
	BAD_PARAM,
	XML_ERR,
	THREAD_ERR,
	WND_DIM_ERR
};

/**
 * Enumeration to specify the severity of the logged data
 */
enum LogTypeInterface {
	PB_LOG_ERROR = 0,	///< Show stopper
	PB_LOG_WARNING = 1,	///< PB Can continue with reduced functionality
	PB_LOG_INFO = 2		///< General Information, e.g. Plugin X loaded
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
typedef void (PAINTCALLBACK) (PBModule*, int, PAINTSTRUCT*);		// GD

struct REGISTERPAINT
{
	int nAppID;
	HWND hWnd;
	PBModule *pModule;
	PAINTCALLBACK *pCallback;
};

/******************************************************************************/
/**
 *  Used to send multiple strings to the core
 */
typedef struct VarsStruct{
	LPTSTR		pStr;
	VarsStruct	*pNextVar;
}VARSTRUCT,*PVARSTRUCT;





/**
 * Structure passed to Modules on Initialisation and method invocation.  
 * Used to define PB environment
 * This will evolve as the module design progresses
 */
typedef struct PBStruct
{
	HWND			hWnd;				///<  Handle to the main window 
	HINSTANCE		hInstance;			///<  Main window instance
	BOOL			bInvoked;			///<  Whether the associated Meta Tag has been manually invoked
	int				iTabID;				///<  PB Application ID
	
}PBSTRUCT,*PPBSTRUCT;






/**
 *  Callback Function pointers
 */
typedef BOOL	(*pSetPlugProperty)(PPBSTRUCT,LPCTSTR,LPCTSTR ,LPCTSTR ,LPCTSTR );	
typedef BOOL	(*pCallPlugMethod)(PPBSTRUCT,LPCTSTR ,LPCTSTR,LPCTSTR);	
typedef BOOL	(*pRegMessFunc)(REGISTERMESSAGE*);				///<Register for message notification function type.
typedef BOOL	(*pRegPaintFunc) (REGISTERPAINT*);
typedef BOOL	(*pNavigateFunc)(int,LPCTSTR,PVARSTRUCT,LPCTSTR);	
typedef BOOL	(*pMessHandler)(LPCTSTR,MSG *,long *);			
typedef BOOL	(*pLogFunc)(LogTypeInterface,LPCTSTR,LPCTSTR,DWORD,LPCTSTR);
typedef BOOL	(*pLogFunc)(LogTypeInterface,LPCTSTR,LPCTSTR,DWORD,LPCTSTR);
typedef BOOL	(*pBrowserFunc)(PPBSTRUCT,LPCTSTR,LPCTSTR);
typedef void	(*pKillAppFunc)(PPBSTRUCT);
typedef BOOL	(*pBrowseFunc)(int,LPCTSTR);



/**
* The event structure is necessary to pass the event handles to threads
* If you used named events, we open PB up to a security vunerability where another application may have the same named event. 
*
*/
typedef struct stEvent{
	HANDLE PBLogEvent;			///>signalled when a log takes place
	HANDLE PBQuitEvent;			///>signalled when PB is shutting down
	HANDLE PBNavEvent;			///>signalled during browser navigation. Not to be confused with a Javascript navigation
	HANDLE PBAppGotFocusEvent;	///>signalled when a Browser application gains focus
	HANDLE PBJSEvent;			///>signalled when a JS navigation takes place 
}PBEVENTSTRUCT,*PPBEVENTSTRUCT;





/**
 * Structure passed to Preload on Initialisation of a module instance.  
 * Used to to provide implementaion for built in function, which ultimately post back to the core 
 * This will evolve as the module design progresses
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
	pRegPaintFunc		pRegPaintFunc;
	pLogFunc			pLoggingFunc;
	pBrowserFunc		pBrowserMethod;
	pKillAppFunc		pKillApplFunc;
	pBrowseFunc			pBrowReload;
	pBrowseFunc			pBrowStop;
	pBrowseFunc			pBrowBack;
	pBrowseFunc			pBrowHome;
	pBrowseFunc			pBrowsQuit;
	pBrowseFunc			pBrowsMininmize;
	pBrowseFunc			pBrowRestore;

	/*
	*	Core Events 
	*/
	PPBEVENTSTRUCT		pEventStructure;
	
	int					*pCurrentInstID;
	int					*pPreviousInstID;

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
	//PBMetaStruct	*pNextMetaStruct;	///<  Pointer to another identical structure containing more tag info, set to NULL if not used
										
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



/////////////////////////////////////////////////////////
//                                                     //
//                PLUGIN BASE CLASS                    //
//                                                     //
/////////////////////////////////////////////////////////

/**
 * Abstract Base Class for all Modules.  This class must be inherited from by each module
 * to ensure conformance with the Browser architecture.
 */
class PBModule {
public:
	
	/**
	 *  Constructor for PBModule.
	 *  Sets the Module count variable and copies the default name "Abstract" to the object identifier
	 */
	PBModule();

	
	/**
	 *  Destructor for PBModule.
	 *  Destructor not currently used; Should use onDeInit() to cleanup. 
	 */
	~PBModule();
	

	/**
	 * Virtual member which can be overridden in the module class.
	 * Sets up the function pointers from the core.
	 * Maintains a module count and calls onInit() when called for the first time.
	 * @param	pbMetaStructure		Pointer to the meta tag to be processed.
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return						TRUE on sucess otherwise FALSE.
	 */
	virtual BOOL Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure);

	
	/**
	 * Virtual member which can be overridden in the module class.
	 * Calls onDeInit() when the last Dispose has been issued.
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return						An enumeration type INITRET defined above
	 */
	virtual INITRET Dispose(PPBSTRUCT pPBStructure);
	

	/**
	 * Virtual member.
	 * This is called by the exported function  <Module name>_MetaProc. 
	 * It associates a PB application with the correct instance of the wrapped object.
	 * It then calls MetaProc in the derived class with that pointer.
	 * @param	pbMetaStructure		Pointer to the meta tag to be processed.
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return						TRUE if successfully invoked, else FALSE.
	 */
	virtual BOOL Process(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);

/******************************************************************************/
	/**
	 * Processes notifications of Windows messages from the core.
	 * Calls virtual onMessage function which derived class overrides.
	 * Called by static callback function.
	 */
	BOOL ProcessMessage (int appid, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/**
	 * Called back from core. Resolves 'this' pointer and calls ProcessMessage()
	 */
	static BOOL StaticProcessMessage (PBModule *pmodule, int appid, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/**
	* Virtual function to override to get notifications of Windows messages.
	* Return TRUE to prevent further processing of the message.
	* Returns FALSE by default if not overriden.
	*/
	virtual BOOL onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject) { return FALSE; }

	/**
	* Method used when a module requests notification of a message from the core for specified window handle
	*/
	BOOL RegisterForMessage(HWND hwnd, UINT msg);
	
	/** 
	* Register for message from the current host window.
	*/
	BOOL RegisterForMessage(UINT msg);
	
	BOOL RegisterForMessage(PBModule *pmodule, MESSAGECALLBACK *pcallback, int appid, HWND hwnd, UINT msg);	

/******************************************************************************/
	void ProcessPaint (int appid, PAINTSTRUCT *pps);
	static void StaticProcessPaint (PBModule *pmodule, int appid, PAINTSTRUCT *pps);

	virtual void onPaint (PAINTSTRUCT *pps, void *pobject) { }

	BOOL RegisterForPaint (void);

/******************************************************************************/
	/**
	 * Used to request the onAppFocus notification.
	 * 
	 *
	 */
	void RegisterForAppFocus();


	/**
	 * Used to implement the onMessage function and should only be used by the abstract class.
	 * pMsg
	 * @return			TRUE if the message is handled and not passed on to the core.
	 * @return			FALSE if the message is not handled and passed on to the core.
	 */
	static BOOL onMsg(MSG *pMsg,long *pMod);// this is the function that gets passed to the core

	static DWORD WINAPI AppFocusThread(LPVOID lparam);

	/**
	 * Method to send a navigate string
	 * @param lpNavStr	Text containing the url including format specifiers.
	 * @param ...		Additional parameters used in the formatting of the navigation URI..
	 * @return			TRUE if successful.
	 */
	BOOL SendPBNavigate(int iInstID,LPCTSTR lpNavStr,...);

protected:	

		
	/**
	 * Method that is used when a module sets a property in module usually contained in another DLL.
	 * @param pModname	The name of the module to receive the request
	 * @param pParam	The parameter part of a meta tag.
	 * @param pValue	The value of the meta tag - can be NULL. 
	 * @return			TRUE if successful.
	 */
	BOOL SetModProperty (LPCTSTR pModName,LPCTSTR pParam,LPCTSTR pValue);
	
	/**
	 * Method that is used when a module calls a method in another module usually contained in another DLL.
	 * @param pModname	The name of the module to receive the request
	 * @param pParam	The parameter part of a meta tag.
	 * @param pValue	The value of the meta tag - can be NULL. 
	 * @return			TRUE if successful.
	 */
	BOOL CallModMethod	(LPCTSTR pModName,LPCTSTR pParam);
	
	

	
	
	
	/**
	 * Method to append to the log file
	 * @param logSeverity		Severity level of the log 
	 * @param pLogComment		Comment detailing the log
	 * @param pFunctionName		Function name where log was called from
	 * @param dwLineNumber		Line number of the file
	 * @return			TRUE if successful.
	 */
	BOOL PBModule::Log(LogTypeInterface logSeverity, LPCTSTR pLogComment, 
		   LPCTSTR pFunctionName, DWORD dwLineNumber);

	/**
	 * Pure virtual member which must be overridden in the module class.
	 * Called by PBModule::Process
	 * Provides Browser with a way of invoking functionality in the module.
	 * pbMetaStructure should be parsed to determine which action to perform.
	 * @param	pbMetaStructure		Pointer to the property or method to be processed.
	 * @param	pPBStructure		Pointer to a PBStructure.
	 * @param	pParam				A void pointer referencing an instance of wrapped object
	 * @return						TRUE if successfully invoked, else FALSE.
	 */
	virtual BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)= 0;
	
	
	/**
	 * Method for comparing two TCHARs insensitive of case
	 * @param tc1 First string to compare
	 * @param tc2 Second string to compare
	 * @return TRUE if the two given strings are lexigraphically the same
	 * 
	 */
	virtual BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);
	
	
	
	/**
	 * Method that is called when a module receives the first preload.
	 * For a module to receive notification, it must overridden in the derived class. 
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return						TRUE if initialisation is successful.
	 */
	virtual BOOL onInit(PPBSTRUCT pPBStructure){return TRUE;};

	/**
	 * Method that is called when a module receives the a final Dispose.
	 * For a module to receive notification, it must overridden in the derived class. 
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return TRUE if de-initialisation is successful.
	 */
	virtual void onDeInit(PPBSTRUCT pPBStructure){return;};
	
	/**
	 * Method that is called when a module receives the first preload for a new instance.
	 * For a module to receive notification, it must overridden in the derived class. 
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct){return TRUE;};
	
	/**
	 * Method that is called when a module receives a final Dispose on an instance.
	 * For a module to receive notification, it must overridden in the derived class. 
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct){return TRUE;};

	/**
	 * Method that is called when a application gains focus.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForAppFocus()', must be called first
	 * @param	iID					application instance ID.
	 * @return						TRUE if handled successfully.
	 */
	virtual void onAppFocus(int iOldID,int iNewID){return;};

	

	/**
	 * Method that is used internally.
	 * Recursively deletes the linked list created in SendPBNavigate
	 * @param	pVarStruct		Pointer to a structure used internally.
	 */
	void	deleteMem(PVARSTRUCT pVarStruct);
	
	/**
	 * Method that is used internally.
	 * Recursively deletes the linked list used for tracking instance state
	 * @param	pInstStruct		Pointer to an instance structure used internally.
	 */
	void	deleteInst(PPBINSTSTRUCT pInstStruct);

	
	PPBINSTSTRUCT dispose(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);
	
	/**
	 * 
	 * Sets up the function pointers from the core.
	 * 
	 * @return  TRUE for success.
	 */
	BOOL	Init(PPBCORESTRUCT pPBCoreStructure);

	/**
	 * 
	 * Retreives a pointer to a wrapped target class.
	 * The returned pointer should be cast to the target object type before use.
	 * @param  application iID.
	 */
	void	*GetObjFromID(int iID);
	


	
	PPBSTRUCT			m_pCurrentPBstruct;			
	PPBCORESTRUCT		m_pCoreStructure;
		
	


	TCHAR				m_szModName[100];	///< String space for the name of the module.
	int					m_iLoadCount;		///< The internal count used to implement onInit() and onDeInit().
	
	PPBINSTSTRUCT		m_pInstHead;
	PPBEVENTSTRUCT		m_pCoreEvents;
	//int					*m_pCurrentID;
	HANDLE				m_hQuitEvent;		///< Used to shut down any threads that are running when the module is deinitialising
	

	
};

