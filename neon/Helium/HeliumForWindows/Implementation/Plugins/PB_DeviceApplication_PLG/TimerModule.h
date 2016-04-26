/**
*  \file TimerModule.h
*  \brief Timer Plug-in module interface.
*  File to house the CTimer Module class and all related includes / defines
*  to interface between Browser core and the Timer functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"

static TCHAR* tcTimeoutEventNames[] = {L"time", NULL};

/**
*  Class to define the interface between the core and the Timer functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Timer behaviour.
*/
class CTimerModule: public PBModule
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
	*  Unregisters OnBrowserNavigate callback.
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates instance data for this timer object.  Stores a reference to 
	*  the instance data in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the Timer data associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::OnBeforeNavigate.  
	*  Please see that function for further definition.
	*  Clears the registered TimeoutEvent (NavString) When the application
	*  navigates to a new page.
	*/
	BOOL onBeforeNavigate(int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Timer module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:
	/**
	*  Data associated with each instance of the Timer module.
	*/
	struct INSTANCE_DATA
	{
		int instanceID;					///< Unique Application Identifier.
		HANDLE	hTimerCancel;			///< Event triggered to cancel the timer without Navigate
		HANDLE	hTimerThread;			///< Handle to the timer thread.
		int iTimeoutDuration;			///< Duration for the Timer to Run for
		TCHAR tcTimerNavigate[MAX_URL];	///< URI to navigate to when the timer fires
		BOOL bTimerRunning;				///< Whether or not there is a timer already running
	};

	/**
	*  Stops the timer associated with the specified instance if it is running.
	*  \param pData Data associated with the timer necessary to stop it.
	*  \return Whether or not the timer was stopped.
	*/
	BOOL StopTimer(INSTANCE_DATA* pData);

	/**
	*  Thread representing the running timer.  The Thread can be stopped by 
	*  setting its stop event, otherwise it will run for the configured
	*  duration and send a PB Navigate to the specified address.
	*  \param lParam Pointer to the module associated with the thread.
	*  \return 0 when the thread exits.
	*/
	static DWORD TimerThread(LPVOID lParam);
};