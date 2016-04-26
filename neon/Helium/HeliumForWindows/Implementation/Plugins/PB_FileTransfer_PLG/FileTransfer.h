/**
*  \file FileTransfer.h
*  \brief File Transfer Plug-in module interface.
*  File to house the CFileTransfer Module class and all related includes / defines
*  to interface between Browser core and the File Transfer functionality.
*/
#pragma once

#include "..\..\Common\Public\PBPlugin.h"
#include "Inet.h"

/**
*  Class to define the interface between the core and the File Transfer
*  functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate File Transfer behaviour.  See CInet for 
*  the implementation of File Transfer.
*/
class CFileTransfer: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers for a before 
	*  navigation callback.
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
	*  Creates and Initialises a new CInet Object.  Stores a reference to 
	*  the new object in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the CInet object associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);
	
	/**
	*  This Function is inherited from PBModule::OnBeforeNavigate.  
	*  Please see that function for further definition.
	*  Clears the TransferEvent string.
	*/
	BOOL onBeforeNavigate(int iInstID);

	
	
	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the File Transfer module and 
	*  actions them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam);
};
	
