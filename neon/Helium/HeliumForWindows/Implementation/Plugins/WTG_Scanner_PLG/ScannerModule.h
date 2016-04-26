#pragma once

#include "../../../Implementation/Common/Public/PBPlugin.h"

class CScannerModule : public PBModule
{
public:
	/**
	*  Overrides the default constructor to store a pointer to the PB Core 
	*  structure.  Store a local pointer to the core structure which contains
	*  function pointer to inform core of screen orientation change.
	*/
	CScannerModule(PPBCORESTRUCT pPBCoreStructure);

	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onDeInit.  
	*  Please see that function for further definition.
	*  No Functionality for Telemetry, all closing down is done in onReleaseInstance
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates an object to hold the current instance identifier.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the Instance Data associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onBrowserNav.  
	*  Please see that function for further definition.
	*  Clears the registered TelemetryDataEvent (NavString) When the application
	*  navigates to a new page.
	*/
	BOOL onBeforeNavigate(int iInstID);

	/**
	*  This Function is inherited from PBModule::onDocumentComplete.  
	*  Please see that function for further definition.
	*  navigates to a new page.
	*/
	BOOL onDocumentComplete	(int iInstID);

	/**
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the MoveSIP module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:

};