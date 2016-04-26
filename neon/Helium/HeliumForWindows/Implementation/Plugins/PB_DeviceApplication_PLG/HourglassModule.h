/**
*  \file HourglassModule.h
*  \brief Hourglass Plug-in module interface.
*  File to house the CHourglassModule class and all related includes / defines
*  to interface between Browser core and the Hourglass functionality.
*  The Hourglass will be shown automatically during page navigates and also 
*  when told to.  The Hourglass will not be shown if the request originates from 
*  a module which does not have focus.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "HourglassAnimation.h"

/**
*  Class to define the interface between the core and the CHourglassAnimation 
*  functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Hourglass behaviour. 
*/
class CHourglassModule: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers for OnAppFocus
	*  events.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onDeInit.  
	*  Please see that function for further definition.
	*  Deregisters for OnAppFocus events.
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates instance data for the new Object.  Stores a reference to 
	*  the instance data in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the instance data associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onAppFocus.
	*  Please see that function for further definition.
	*  Receive change of application focus notifications, inform the old and 
	*  new IDs whether they have lost / gained focus
	*/
	BOOL onAppFocus(int iOldID, int iNewID);

	/**
	*  This Function is inherited from PBModule::onBeforeNavigate.  
	*  Please see that function for further definition.
	*  Displays the Hourglass if this application is in the foreground.
	*/
	BOOL onBeforeNavigate	(int iInstID);

	/**
	*  This Function is inherited from PBModule::onDocumentComplete.
	*  Please see that function for further definition.
	*  Hides the Hourglass once navigation has completed.
	*/
	BOOL onDocumentComplete	(int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Hourglass module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

	/**
	*  Function to retrieve the 'WaitCursorEnabled' configuration setting.
	*  \param iInstance Application instance to retrieve the setting for
	*  \param value [out] Value of the configuration setting
	*  \return Whether or not the value was successfully read from the 
	*          configuration.
	*/
	bool RetrieveNoWaitConfigValue(int iInstance, BOOL* input);

	/**
	*  Function to retrieve the 'WaitCursorLeft' configuration setting.
	*  \param iInstance Application instance to retrieve the setting for
	*  \param value [out] Value of the configuration setting
	*  \return Whether or not the value was successfully read from the 
	*          configuration.
	*/
	bool RetrieveHourglassDefaultLeft(int iInstance, DWORD* value);

	/**
	*  Function to retrieve the 'WaitCursorTop' configuration setting.
	*  \param iInstance Application instance to retrieve the setting for
	*  \param value [out] Value of the configuration setting
	*  \return Whether or not the value was successfully read from the 
	*          configuration.
	*/
	bool RetrieveHourglassDefaultTop(int iInstance, DWORD* value);

	/**
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

private:
	HANDLE m_hWindowInitialisedEvent;
};