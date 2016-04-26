/**
*  \file VolumeModule.h
*  \brief Volume Plug-in module interface.
*  File to house the CVolume Module class and all related includes / defines
*  to interface between Browser core and the Volume functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"

/**
*  Class to define the interface between the core and the Volume functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Volume behaviour.
*/
class CVolumeModule: public PBModule
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
	*  Accepts 'Meta tags' associated with the Volume module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:
	/**
	*  Data associated with each instance of the Backlight module.
	*/
	struct INSTANCE_DATA
	{
		int instanceID;			///<  Unique Application Identifier.
		DWORD dwCurrentVolume;	///<  Current module's volume setting
		bool bAppHasFocus;		///<  TRUE if the we are the foreground app
		DWORD dwDefaultVolume;	///<  Default volume to set when going to background
	};
};