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
#include "PB_Defines.h"
#include <ctype.h>

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
	 *  Copies the default name "Abstract" to the object identifier
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
	 * @param	pPBStructure		Pointer to PBStructure, please see PBStruct defined above.
	 * @param	pPBCoreStructure	Pointer to the core callback functions and events.
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
	* Methods used when a module requests (or no longer requires) notification of a message from the core for specified window handle
	*/
	BOOL RegisterForMessage(HWND hwnd, UINT msg);
	BOOL UnRegisterForMessage(HWND hwnd, UINT msg);
	
	/** 
	* Register/DeRegister for message from the current host window.
	*/
	BOOL RegisterForMessage(UINT msg);
	BOOL UnRegisterForMessage(UINT msg);
	
	BOOL RegisterForMessage(PBModule *pmodule, MESSAGECALLBACK *pcallback, int appid, HWND hwnd, UINT msg);	
	BOOL UnRegisterForMessage(PBModule *pmodule, MESSAGECALLBACK *pcallback, int appid, HWND hwnd, UINT msg);	

	/** 
	* Register/Deregister for primary message (i.e. receive a message before it is handled by the browser).
	*/
	BOOL RegisterForPrimaryMessage(UINT iWMMsgID);
	BOOL UnRegisterForPrimaryMessage(UINT iWMMsgID);

	/**
	 * Processes notifications of Windows paint messages from the core.
	 * Calls virtual onPaint function which derived class overrides.
	 * Called by static callback function.
	 */
	void ProcessPaint (int appid, HDC hdc);
	
	/**
	 * Called back from core. Resolves 'this' pointer and calls ProcessPaint()
	 */
	static void StaticProcessPaint (PBModule *pmodule, int appid, HDC hdc);

	/**
	* Virtual function to override to get notifications of Windows paint messages.
	* 
	*/
	virtual void onPaint (HDC hdc, void *pobject) { }

	/** 
	* Register for paint from the current host window.
	*/
	BOOL RegisterForPaint (void);


	
	/**
	 * Used to request the onAppFocus notification.
	 * @param	bReg sets the core to register;TRUE or unregister;FALSE.
	 */
	//BOOL RegisterForAppFocus(BOOL bReg);

	/**
	 * Used to request the onAppFocus notification.
	 * @param	bReg sets the core to register;TRUE or unregister;FALSE.
	 */
	BOOL RegisterForEvent(PB_SyncEvent iEventID);

	BOOL UnRegisterForEvent(PB_SyncEvent iEventID);

	/**
	 * Used to implement the onMessage function and should only be used by the abstract class.
	 * pMsg
	 * @return			TRUE if the message is handled and not passed on to the core.
	 * @return			FALSE if the message is not handled and passed on to the core.
	 */
	static BOOL onMsg(MSG *pMsg,long *pMod);// this is the function that gets passed to the core

	

	/**
	 * Method to send a navigate string
	 * @param lpNavStr	Text containing the url including format specifiers.
	 * @param ...		Additional parameters used in the formatting of the navigation URI..
	 * @return			TRUE if successful.
	 */
	BOOL SendPBNavigate(TCHAR* tcJSONNames[], int iInstID,LPCTSTR lpNavStr, ...);

	/**
	 * Method to send a navigate string
	 * @param inst_id	Instance ID.
	 * @param pnavstr	Text containing the url including format specifiers.
	 * @param count		Number of strings in array.
	 * @param pstrings	Array of pointers to strings
	 * @return			TRUE if successful.
	 */
	BOOL SendPBNavigate (TCHAR* tcJSONNames[], int inst_id, LPCTSTR pnavstr, int count, LPCTSTR *pstrings);

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
	 * Method for comparing two TCHARs insensitive of case
	 * @param tc1 First string to compare
	 * @param tc2 Second string to compare
	 * @return TRUE if the two given strings are lexigraphically the same
	 * 
	 */
	virtual BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);

	/**
	 * Public method that is called when a application gains focus.
	 * If registered for, Browser cannot switch to another application until this returns TRUE.
	 * This needs to be public so that it can be called by the core.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForEvent(PB_APPFOCUSEVENT)', must be called first
	 * @param	iID					application instance ID.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onAppFocus(int iOldID,int iNewID){return TRUE;};

	/**
	 * Method that is called when the browser completes a navigate to a new URL.
	 * This needs to be public so that it can be called by the core.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForEvent(PB_BROWSER_NAV_COMPLETE_EVENT)', must be called first
	 * @param	iInstID					application instance ID.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onNavigateComplete	(int iInstID){return TRUE;};
	
	/**
	 * Method that is called when the browser is about to navigate to a new URL.
	 * This needs to be public so that it can be called by the core.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT)', must be called first
	 * @param	iInstID					application instance ID.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onBeforeNavigate	(int iInstID){return TRUE;};
	
	
	/**
	 * Method that is called when the browser completes a navigate and all of the meta tags have been processed.
	 * This needs to be public so that it can be called by the core.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForEvent(PB_BROWSER_DOC_COMPLETE_EVENT)', must be called first
	 * @param	iInstID					application instance ID.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onDocumentComplete	(int iInstID){return TRUE;};
	
	
	/**
	 * Method that is called when the browser fails to navigate to a new URL.
	 * This needs to be public so that it can be called by the core.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForEvent(PB_BROWSER_NAV_ERROR_EVENT)', must be called first
	 * @param	iInstID					application instance ID.
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onNavigateError	(int iInstID){return TRUE;};

	
	virtual BOOL onPrimaryMessage	(MSG *pMsg, int iCurrID){return FALSE;};
	
	/**
	 * Method that is called with the browser window change. Minimize, Restore and screen orientation 
	 * This needs to be public so that it can be called by the core.
	 * For a module to receive notification, it must overridden in the derived class.
	 * To acivate the notification 'RegisterForEvent(PB_WINDOWCHANGED_EVENT)', must be called first
	 * @param	iEvent				PORTRAIT,RIGHT_HANDED,UPSIDE_DOWN,LEFT_HANDED,NOT_SUPPORTED,PB_WINRESTORED or PB_WINMINIMIZED .
	 * @return						TRUE if handled successfully.
	 */
	virtual BOOL onWindowChanged	(int iEvent){return TRUE;};

	/**
	*  Public method that is called when a Rhodes application gains or loses focus.
	*  This replaces the previous onAppFocus method from RE2.2 onwards.
	*  \param bActivate true if the app has gained focus, else false.
	* To activage the notification 'RegisterForEvent(RHO_APPFOCUSEVENT)', must be called first
	*  \return is ignored.
	*/
	virtual BOOL onRhoAppFocus(bool bActivate, int iInstID){return TRUE;};

	
	/**
	 *	IMO: Internal Module Object 
	 *	Method that is called to create an instance of module for internal use.
	 *	@param	pTargetModuleName	Target module name.
	 *	@return						A reference to a new instance of module or NULL if module cannot be found.
	 */
	IMOREF	CreateIMO		(LPCTSTR pTargetModuleName);
	
	
	/**
	 *	IMO: Internal Module Object 
	 *	Method that is called when this plugin needs to delete an instance of a module
	 *	@param	IMORef		A reference to an instance of a module.
	 *	@return				TRUE if successful.
	 */
	BOOL	DeleteIMO		(IMOREF IMORef);	
	
	
	/**
	 *	IMO: Internal Module Object 
	 *	Sets a property on a IMO reference
	 *	@param	IMORef		A reference to an instance of a module.
	 *	@param	pParam		The property parameter name to set.
	 *	@param	pValue		The property value to set.
	 *	@return				TRUE if successful.
	 */
	BOOL	SetIMOProperty	(IMOREF IMORef,LPCTSTR pParam,LPCTSTR pValue);
	
	
	/**
	 *	IMO: Internal Module Object 
	 *	Calls a method on a IMO reference
	 *	@param	IMORef		A reference to an instance of a module.
	 *	@param	pMethod		The name of the method to call on the IMO.
	 *	@return				TRUE if successful.
	 */
	BOOL	CallIMOMethod	(IMOREF IMORef,LPCTSTR pMethod);

	/**
	 *	IMO: Internal Module Object 
	 *	Sets a callback function to receive navigation (return data) from the plugin.
	 *	@param	IMORef			A reference to an instance of a module.
	 *	@param	IMOEventProc	A callback function pointer to receive plug-in returns.
	 *	@return				TRUE if successful.
	 */
	BOOL	SetIMOCallBack	(IMOREF IMORef,IMOEVENTPROC IMOEventProc,LPARAM lParam);
	
	/**
	 *	GetAppIDFromIMO
	 *	Every IMO is created by an application, this will convert from IMO ID to app ID.
	 *	@param	iImoID			Browser IMO ID.
	 *	@return					Returns the application ID of the IMO..
	 */
	int		GetAppIDFromIMO(int iImoID);
	
	/**
	 *	isIMO
	 *	determines if the ID is an IMO or not.
	 *	@param	iID			ID.
	 *	@return				TRUE if an IMO ID or FALSE if not.
	 */
	BOOL	isIMO(int iID){return (iID > PB_APP_ARRSIZE || iID<0);};
	
	/**
	 *	GetEMMLObj
	 *	Gets the EMML object for iTabID.
	 *	@param	iTabID			Browser application instance.
	 *	@return					A pointer to the EMML object.
	 */
	CEMML	*GetEMMLObj(int iTabID);

	
	
	/**
	 *	GetSettingPtr
	 *	Gets the settings
	 *	@param	pXmlPath			
	 *  @param  pAttName 
	 *	@return					A pointer to the setting string, this string will exist for the duration of PB.
	 */
	LPCTSTR GetSettingPtr(LPCTSTR pXmlPath,LPTSTR pAttName);
		
	LPCTSTR GetAppSettingPtr(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName);
	
	LPCTSTR GetDefaultMetaTagsPtr(int iAppID,int iMetaIndex);
	
	LPCTSTR GetPreLoadPtr(int iAppID,int iPreLoadIndex);
	
//write settings

	BOOL	SetAppValue		(int iAppID,LPCTSTR pXmlPath,LPCTSTR pValue);
	
	BOOL	SetGlobalValue	(LPCTSTR pXmlPath, LPCTSTR pValue);
	
	BOOL	AddPreload		(int iAppID,LPTSTR pPreloadStr);
	
	BOOL	AddDefaultMeta	(int iAppID,LPTSTR pPreloadStr);


	/**
	 *	GetCurrURL
	 *	Gets the current URL for the application referenced by iAppID
	 *	@param	iAppID			The application ID of the current URL of interest			
	 *  @return					A pointer to the current URL.
	 */
	LPCTSTR GetCurrURL(int iAppID){return m_pCoreStructure->pGetAppCurrURL(iAppID);};
	
protected:	

	
	

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
	 * Method that is used internally.
	 * Recursively deletes the linked list created in SendPBNavigate
	 * @param	pVarStruct		Pointer to a structure used internally.
	 */
	void	deleteMem(PVARSTRUCT pVarStruct);



	/**
	 * 
	 * Retreives a pointer to a wrapped target class.
	 * The returned pointer should be cast to the target object type before use.
	 * @param  application iID.
	 */
	void	*GetObjFromID(int iID);
	
	/**
	 * Method that checks if a string contains any non digit characters.  
	 * @param	str		Null terminated wchar string to check
	 * @return	TRUE	if all characters are digits
	 */
	BOOL isInt(const wchar_t *str);

	/**
	 * Method that checks sf a string contains any non digit characters
	 * @param	str		Null terminated wchar string to check
	 * @return	TRUE	if all characters are digits and there is one or less '.'
	 */
	BOOL isFloat(const wchar_t *str);


	
	PPBSTRUCT			m_pCurrentPBstruct;			
	PPBCORESTRUCT		m_pCoreStructure;
	
	TCHAR				m_szModName[MAXMODLEN + 1];	///< String space for the name of the module.
	int					m_iLoadCount;		///< The internal count used to implement onInit() and onDeInit().
	
	PPBINSTSTRUCT		m_pInstHead;
	PPBEVENTSTRUCT		m_pCoreEvents;
	
	
};

