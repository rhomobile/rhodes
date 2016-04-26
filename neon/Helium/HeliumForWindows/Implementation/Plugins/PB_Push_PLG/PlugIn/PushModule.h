/** 
* \file PushModule.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
/** Macro giving count of elements in an array */
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

/** Macro to copy a wide string, guaranteed not to overrun the destination and always to be null terminated */
#define WSAFECOPY(d,s)	StringCchCopy(d,COUNTOF(d),s)

/** Macro to write to PBModule log automatically adding function name and line number */
#define LOG(s,m) Log(s, m,_T(__FUNCTION__), __LINE__)

/******************************************************************************/
#include "..\..\..\Common\Public\PBplugin.h"
#include "..\httpserver\httpserver.h"
#include "powerup.h"


/******************************************************************************/
/**
* Structure to hold per-instance data
*/
/******************************************************************************/
struct INSTANCE_DATA
{
	int nID;	///< App ID from PBModule
	WCHAR sNavigate [MAX_BUFFER+1];	///< URL navigation target when successful Push received
	BOOL bUnattendedEnabled;	///< Flag to show unattended mode is enabled for this instance
	BOOL bMessagesRegistered;	///< Flag to show if instance has registered for Windows messages

	CPowerUp *pPowerUp;
};

/******************************************************************************/
/**
* Main Push plugin module class derived from PBModule.
*/
/******************************************************************************/
class CPushModule: public PBModule
{
public:
	CPushModule ();
	~CPushModule ();

private:
	static CPushModule *pThis;	///< Static pointer to ourselves so static functions can get a reference.

	/**
	* Called by CHttpServer object when a successful Push is detected. Calls SendPBNavigate to notify core.
	* \param prequest Object holding name/value pairs from request. Freed by CHttpServer after callback so a pointer should not be kept to this object.
	* \param pserver Server which has detected Push.
	*/
	static void OnRequest (CHttpRequest *prequest, CHttpServer *pserver);

	static void OnPowerUp (void*);

	/**
	* Case-insensitive test for YES, TRUE or ENABLED
	* \param pstring String to test
	*/
	BOOL StringToBool (LPCTSTR pstring);

	/** \name Overrides of functions in PBModule base class */
	//@{
	BOOL onInit(PPBSTRUCT pPBStructure);
	void onDeInit(PPBSTRUCT pPBStructure);
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
	BOOL onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject);
	//@}
};
