/**
*  \file SNTPModule.h
*  \brief SNTP Plug-in module interface.
*  File to house the CSNTP Module class and all related includes / defines
*  to interface between Browser core and the SNTP functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "SNTP.h"

/**
*  Class to define the interface between the core and the SNTP functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate SNTP behaviour.
*/
class CSNTPModule: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

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
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the SNTP module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:

	/**
	*  Data associated with each instance of the Timer module.
	*/
	struct INSTANCE_DATA
	{
		CSNTPClient*		pSNTPClient;	///<  SNTP Client associated with this module
		NtpServerResponse*	pSNTPresponse;	///<  SNTP Response object associated with this module
	};
};