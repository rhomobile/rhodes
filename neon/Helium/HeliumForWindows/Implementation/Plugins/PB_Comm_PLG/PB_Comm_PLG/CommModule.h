/**
 * \file CommModule.h
 * \brief Interface functionality for the Comm module.  The class
 * defined in this file provides implementations 
 * for the plugin interface, and it overrides the onInit, MetaProc,
 * onAttachInstance, and onReleaseInstance methods.  It instantiates 
 * the CComm class, which provides the 
 * module's functionality.
 */

#pragma once
#include "../../../Common/Public/pbplugin.h"
#include <windows.h>

static TCHAR* tcCommEventNames[] = 
{
	L"data",
	NULL
};



/** 
 * Class which instantiates the CComm object, which provides the module's 
 * functionality.
 */
class CCommModule : public PBModule
{
public:
	/**
	 * Public function which is called when the first instance of
	 * the Comm module is preloaded.  It creates the instance of 
	 * the CComm class.  
	 *	 
	 * \return TRUE the CComm object was successfully instantiated
	 * \return FALSE the CComm object was not successfully instantiated
	 */
	BOOL onInit(PBStruct *pPBStructure);

	/**
	 * Public function that processes the meta functions that are passed
	 * to the Comm module.  
	 *
	 * \return TRUE if meta-tag is processed without errors
	 * \return FALSE if errors occur during meta-tag processing
	 */
	BOOL MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure, VOID *pParam);

	/**
	 * Public function which is called when an instance of
	 * the Comm module needs to be attached to support a new concurrent application.
	 * It creates the instance of the CComm class.  
	 *
	 * \return TRUE the CComm object was successfully instantiated
	 * \return FALSE the CComm object was not successfully instantiated
	 */
	BOOL onAttachInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStructure);

	/** 
	 * Public function which is called when an instance of 
	 * the Comm module is released due to a concurrent application.
	 * It frees the CComm object instance specified
	 * and sets the instance pointer to NULL.  
	 *
	 * \return TRUE if successful
	 */
	BOOL onReleaseInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStructure);

	/** 
	 * Public function which is called when an instance of 
	 * the Comm module needs to be notified that the browser is about to 
	 * navigate away from the current page.  
	 *
	 * \param iInstID ID of the application instance affected by the nav event
	 * \return TRUE if successful
	 */
	BOOL onBeforeNavigate(int iInstID);

	/** 
	 * Public function which is called when an instance of 
	 * the Comm module needs to post a log message.  This function
	 * will call the 'Log' function, which will handle
	 * the actual logging action.
	 *
	 * \param logSeverity an enumerated type describing the severity of the message
	 * \param pLogComment the message associated with the log entry
	 * \param pFunctionName the name of the function that produced the log message
	 * \param iTabID the intance ID of the application this instance is supporting
	 * \param dwErrorCode the Microsoft error code if applicable (if not, it will be NULL)
	 * \param dwLineNumber the line number of the file
	 * \return TRUE if successful
	 */
	BOOL LogInst(LogTypeInterface logSeverity, LPCTSTR pLogComment, 
				 LPCTSTR pFunctionName, int iTabID, DWORD dwErrorCode, DWORD dwLineNumber);
	/**
	 * Public function which is called when an instance of the Comm
	 * module needs to initiate a navigate action.  
	 *
	 * \param tcNavigateUrl string containing the navigate URL for this navigate request
	 * \param pVarsStructure pointer to a VarsStruct containing the navigate data from the serial port
	 * \return TRUE if successful
	 */
	BOOL SendPBNavigateInst(PBStruct *pPBStructure, TCHAR *tcNavigateUrl, TCHAR *tcNavigateData);

	/**
	 *
	 * \return m_pCoreStructure pointer
	 */
	PBCoreStruct * GetCoreStructure();

	CCommModule(void);
	~CCommModule(void);

};
