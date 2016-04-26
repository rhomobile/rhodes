/**
 * \file CardReaderMod.h
 * \brief CardReader module definition. The CardReader module allocates and 
 * initializes all of the data structures required for the operation of the 
 * card reader. 
 */
#pragma once

#include "../../Common/Public/pbplugin.h"

#define FOCUS_SWITCH		TRUE			///< flag used to indicate action to be performed as a result of a focus switch
#define NO_FOCUS_SWITCH		FALSE			///< flag used to indicate action to be performed not as a result of a focus switch

enum eFocusResumeAction
{
	NONE = 0,
	ACTIVATE,
	RESUME
};

/**
 * CCardReaderMod - CardReader module that allocates and initializes all of the
 * data structures required for the operation of the card reader. The primary 
 * data structure is a linked list of CCardReader objects, one for each application 
 * that has made a CardReader_Preload.
 */
class CCardReaderMod:public PBModule
{
public:
	CCardReaderMod();

	BOOL onInit(PBStruct *pPBStructure);

	void onDeInit(PBStruct *pPBStructure);

	BOOL onAttachInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct);

	BOOL onReleaseInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct);

	BOOL MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure, VOID *pParam);

	BOOL onAppFocus(int iOldID, int iNewID);

	BOOL onBeforeNavigate(int iInstID);

	/**
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

	/**
	 * Get the value of the attribute m_hStopEvent
	 * \return  value of the attribute m_hStopEvent
	 */
	HANDLE	GetStopEventHandle();

	/**
	 * Get the value of the attribute m_hKeypadMutex
	 * \return  value of the attribute m_hKeypadMutex
	 */
	HANDLE	GetKeypadMutexHandle();

	/**
	 * Method to allow an instance to append to the log file, annotating message with its app id
	 * \param logSeverity		Severity level of the log 
	 * \param pLogComment		Comment detailing the log
	 * \param pFunctionName		Function name where log was called from
	 * \param dwLineNumber		Line number of the file
	 * \return					TRUE if successful, FALSE otherwise.
	 */
	BOOL LogInst(LogTypeInterface logSeverity, LPCTSTR lpszLogComment, 
		   LPCTSTR lpszFunctionName, DWORD dwLineNumber);
	
	/**
	* Sets the persist state of the reader settings during a page navigate.
	* \param bState				TRUE to persist reader settings during a page navigate or FALSE not to 
	* \return					TRUE if successful, FALSE otherwise.
	*/
	void PersistReaderSettings(BOOL bState);

private:
	HANDLE				m_hStopEvent;			///< Handle to event indicating desire to stop read thread
	HANDLE				m_hKeypadMutex;			///< Handle to mutex used to prevent card reader from being closed while waiting for keypad input
	BOOL				m_bReaderStayOpen;		///< BOOL used to keep the reader open during page navigates 
	bool				m_bAppHasFocus;			///< TRUE if the we're the foreground app
	eFocusResumeAction  m_eResumeAction;		///< whether the card reader needs to be opened or re-opened on regaining focus
};

