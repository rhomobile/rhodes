/**
*  \file KeylightModule.h
*  \brief Keylight Plug-in module interface.
*  File to house the CKeylight Module class and all related includes / defines
*  to interface between Browser core and the Keylight functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "Keylight.h"

/**
*  Class to define the interface between the core and the CKeylight functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Keylight behaviour.
*/
class CKeylightModule: public PBModule
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
	*  Creates an instance of CKeylight.  Stores a reference to 
	*  the CKeylight object in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the CKeylight associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onAppFocus.
	*  Please see that function for further definition.
	*  Receive change of application focus notifications, if the new ID 
	*  represents an application using this module set the Keylight state
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
	*  Accepts 'Meta tags' associated with the Keylight module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

};