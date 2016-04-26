/**
*  \file BacklightModule.h
*  \brief Backlight Plug-in module interface.
*  File to house the CBacklight Module class and all related includes / defines
*  to interface between Browser core and the Backlight functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "dispcapi.h"

#define BACKLIGHT_API_DLL				L"DispAPI32.dll"
#define BACKLIGHT_GETSTATE				TEXT("DISPLAY_GetBacklightState")
#define BACKLIGHT_SETSTATE				TEXT("DISPLAY_SetBacklightState")
#define BACKLIGHT_GETINTENSITYLEVELS	TEXT("DISPLAY_GetBacklightIntensityLevels")
#define BACKLIGHT_GETINTENSITY			TEXT("DISPLAY_GetBacklightIntensity")
#define BACKLIGHT_SETINTENSITY			TEXT("DISPLAY_SetBacklightIntensity")

//  Maximum allowable length of a log message
#define MAX_LOG_MESSAGE			1024

static TCHAR* tcBacklightEventNames[] = {L"intensity", L"intensityRange", L"state", NULL};

/**
*  Class to define the interface between the core and the Backlight functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Backlight behaviour.
*/
class CBacklightModule: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onDeInit.  
	*  Please see that function for further definition.
	*  Deregisters for App Focus events.
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates instance data to be associated with this Backlight module and 
	*  initialises it based on the current value of the backlight.  Logs 
	*  the maximum intensity level.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the instance data associated with this module.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onAppFocus.
	*  Please see that function for further definition.
	*  Receive change of application focus notifications, if the new ID 
	*  represents an application using this module set the Backlight state
	*  if necessry.
	*/
	BOOL onAppFocus(int iOldID, int iNewID);

	/**
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Backlight module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:	

	/**
	*  Load the Backlight DLL into memory
	*/
	BOOL LoadBacklightDLL();
	
	/**
	*  Remove the Backlight DLL from memory.
	*/
	void UnloadBacklightDLL();

	/**
	*  Set all exported function pointers to NULL
	*/
	void NullifyExportedFunctionPointers();

	DWORD		m_dwBacklightDLL;		///<  Result of loading the EMDK Backlight DLL

	//  function pointers to exported DLL functions
	typedef DWORD (WINAPI* LPFN_BACKLIGHT_GET_INTENSITY_LEVELS)		(LPDWORD);
	typedef DWORD (WINAPI* LPFN_BACKLIGHT_GET_INTENSITY)			(LPDWORD);
	typedef DWORD (WINAPI* LPFN_BACKLIGHT_SET_INTENSITY)			(DWORD);
	typedef DWORD (WINAPI* LPFN_BACKLIGHT_GET_STATE)				(LPDWORD);
	typedef DWORD (WINAPI* LPFN_BACKLIGHT_SET_STATE)				(DWORD);

	LPFN_BACKLIGHT_GET_INTENSITY_LEVELS		lpfn_BacklightGetIntensityLevels;
	LPFN_BACKLIGHT_GET_INTENSITY			lpfn_BacklightGetIntensity;
	LPFN_BACKLIGHT_SET_INTENSITY			lpfn_BacklightSetIntensity;
	LPFN_BACKLIGHT_GET_STATE				lpfn_BacklightGetState;
	LPFN_BACKLIGHT_SET_STATE				lpfn_BacklightSetState;

	/**
	*  Data associated with each instance of the Backlight module.
	*/
	struct INSTANCE_DATA
	{
		int instanceID;					///<  Unique Application Identifier.
		DWORD dwBacklightState;			///<  Whether the backlight is currently on or off for this module.
		DWORD dwBacklightIntensity;		///<  The current intensity level of the backlight
		DWORD dwBacklightRange;			///<  The backlight maximum intensity level
		DWORD dwDefaultState;			///<  The default state
		DWORD dwDefaultIntensity;		///<  The default intensity
		TCHAR tcBacklightSettingsEvent[MAX_URL];///<  URI to navigate to when a getBacklightSettings method is received
		bool bAppHasFocus;				///<  TRUE if this app has current focus
	};	
};