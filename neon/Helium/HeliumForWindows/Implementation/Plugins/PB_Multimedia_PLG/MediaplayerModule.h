#pragma once

#include "..\..\Common\Public\pbplugin.h"
#include "MediaPlayer.h"

class CMediaPlayerModule :
	public PBModule
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
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Signature Capture module and 
	*  actions them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam);

	/**
	*  This Function is inherited from PBModule::OnBeforeNavigate.  
	*  Please see that function for further definition.
	*  Stops showing the signature capture box.
	*/
	BOOL onBeforeNavigate(int iInstID);
	static int CALLBACK IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam);
	bool GetMediaFile(CMediaPlayer *pPlayer, IMOEVENTPROC callback);
private:
	IMOREF m_FileTransferIMO;	///< Inter module object to communicate with File Transfer
	
	


};
