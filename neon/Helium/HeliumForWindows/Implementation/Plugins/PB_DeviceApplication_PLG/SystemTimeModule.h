/**
*  \file SystemTimeModule.h
*  \brief System Time Plug-in module interface.
*  File to house the CSystemTime Module class and all related includes / defines
*  to interface between Browser core and the System Time/SNTP functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "SystemTime.h"

/**
*  Class to define the interface between the core and the SystemTime functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate SystemTime behaviour.
*/
class CSystemTimeModule: public PBModule
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
	*  Accepts 'Meta tags' associated with the SystemTime module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:

	/**
	*  This Function checks whether the provided string is an IP address
	*  It only checks that it contains four numbers separated by full stops,
	*  it does not check the numerical validity of the IP address.
	*/
	BOOL isIPv4Addr(LPCTSTR pAddr);

	/**
	*  Data associated with each instance of the SystemTime module.
	*/
	struct INSTANCE_DATA
	{
		CSystemTime*		pSystemTime;	///<  SystemTime Client associated with this module
		NtpServerResponse*	pSNTPresponse;	///<  SNTP Response object associated with this module
	};
};