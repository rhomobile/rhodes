/**
*  \file PowerOnModule.h
*  \brief PowerOn Plug-in module interface.
*  File to house the CPowerOn Module class and all related includes / defines
*  to interface between Browser core and the PowerOn functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "PowerOn.h"

/**
*  Class to define the interface between the core and the CPowerOn functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate PowerOn behaviour.
*/
class CPowerOnModule: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers for Navigation events.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onDeInit.  
	*  Please see that function for further definition.
	*  Deregisters for Navigation events.
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
	*  This Function is inherited from PBModule::onBeforeNavigate.  
	*  Please see that function for further definition.
	*  Clears the registered Navigation URI
	*  \todo NEEDS TESTING IN A BROWSER ENVIRONMENT
	*/
	BOOL onBeforeNavigate	(int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the PowerOn module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

};