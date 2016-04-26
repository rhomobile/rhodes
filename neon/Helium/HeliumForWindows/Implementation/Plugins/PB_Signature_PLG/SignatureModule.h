/**
*  \file SignatureModule.h
*  \brief Signature Plug-in module interface.
*  File to house the CSignature Module class and all related includes / defines
*  to interface between Browser core and the Signature functionality.
*/

#pragma once

#include "..\..\Common\Public\pbplugin.h"
#include "Signature.h"

/**
*  Class to define the interface between the core and the Signature Capture
*  functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Signature Capture behaviour.  See CSignature for 
*  the implementation of Signature Capture.
*/
class CSignatureModule : public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers to receive 
	*  navigation events.  Creates IMO object to allow communciation with 
	*  FileTransfer.
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
	*  Creates and Initialises a new CSignature Object.  Stores a reference to 
	*  the new object in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the CSignatureCapture object associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Signature Capture module and 
	*  actions them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam);

	/**
	*  This Function is inherited from PBModule::onMessage.
	*  Please see that function for further definition.
	*  Handles WM_MOUSEMOVE events used to paint the signature.
	*/
	BOOL onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject);

	/**
	*  This Function is inherited from PBModule::OnBeforeNavigate.  
	*  Please see that function for further definition.
	*  Stops showing the signature capture box.
	*/
	BOOL onBeforeNavigate(int iInstID);

	/**
	*  This Function is inherited from PBModule::onAppFocus.
	*  Please see that function for further definition.
	*  Receive change of application focus notifications, inform the old and 
	*  new IDs whether they have lost / gained focus
	*  \todo Implement Focus change for Signature capture.  This can not be 
	*  done at the present time as the Multiple Instance Test Container is 
	*  out of commission and application switching has not yet been implemented
	*  in the core.
	*/
	BOOL onAppFocus(int iOldID, int iNewID);
	
	static int CALLBACK IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam);
private:
	IMOREF m_FileTransferIMO;	///< Inter module object to communicate with File Transfer
	bool m_bDestinationSet;
};

	
