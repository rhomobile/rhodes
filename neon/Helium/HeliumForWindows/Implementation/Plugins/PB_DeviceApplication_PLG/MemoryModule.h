/**
*  \file MemoryModule.h
*  \brief Memory Plug-in module interface.
*  File to house the CMemory notification module class and all related includes / defines
*  to interface between RhoElements core and the Memory notification module functionality.
*/

#pragma once

// Use to convert bytes to KB
#define DIV 1024

#include "../../common/Public/PBPlugin.h"

static TCHAR* tcMemoryEventNames[] = {L"totalMemory", L"availMemory", NULL};

/**
*  Class to define the interface between the core and the Memory functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Memory notification module behaviour.
*/
class CMemoryModule: public PBModule
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
	*  Creates instance data for this Memory object.  Stores a reference to 
	*  the instance data in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the Memory data associated with the instance being
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
	*  Accepts 'Meta tags' associated with the Memory notification module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:

	/**
	*  Data associated with each instance of the Memory notification module.
	*/
	struct INSTANCE_DATA
	{
		long long	llLowMemThreshold;		///< Low memory Threshold value
		int			iMemoryRefreshInterval; ///< Memory thread refresh interval
		HANDLE		hMemoryThread;			///< Handle to the memory thread
		HANDLE		hMemoryCancel;			///< Handle to stop the memory thread
		BOOL		bThreadRunning;			///< Status of the memory thread
		int			instanceID;				///< Unique Application Identifier.
		TCHAR		tcMemoryNavigate[MAX_URL];///< URI to navigate to when the low Memory fires
		MEMORYSTATUS sMemStat;				///< Memory stats structure
	};

	/**
	*  Stops the Memory associated with the specified instance if it is running.
	*  \param pData Data associated with the Memory necessary to stop it.
	*  \return Whether or not the Memory was stopped.
	*/
	BOOL StopMemory(INSTANCE_DATA* pData);

	/**
	*  Thread representing the running Memory.  The Thread can be stopped by 
	*  setting its stop event, otherwise it will run for the configured
	*  duration and send a PB Navigate to the specified address.
	*  \param lParam Pointer to the module associated with the thread.
	*  \return 0 when the thread exits.
	*/
	static DWORD MemoryThread(LPVOID lParam);
};