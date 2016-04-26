/**
*  \file ApplicationModule.h
*  \brief Application Plug-in module interface.
*  File to house the CApplication Module class and all related includes / defines
*  to interface between Browser core and the Application functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"

static TCHAR* tcApplicationEventNames[] = {L"applicationState", NULL};
static TCHAR* tcVersionEventNames[] = {L"productVersion", L"hotFixes", L"componentVersions=module,version", NULL};

/**
*  Class to define the interface between the core and the Application functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Application behaviour.
*/
class CApplicationModule: public PBModule
{
public:
	/**
	*  Overrides the default constructor to store a pointer to the PB Core 
	*  structure.  Store a local pointer to the core structure which contains
	*  function pointers to restore, minimise and quit Browser.
	*/
	CApplicationModule(PPBCORESTRUCT pPBCoreStructure);

	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers to receive 
	*  notifications when the browser navigates.
	*  \todo Also want to register for Application minimise and restore messages
	*  here so can send the the callback out for each module.
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
	*  Creates instance data to be associated with the module.  Stores a 
	*  reference to the instance data in the pInstStruct, associating it with 
	*  the newly created instance.
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
	*  This Function is inherited from PBModule::OnBeforeNavigate.  
	*  Please see that function for further definition.
	*  Clears the registered ApplicationEvent (NavString) When the application
	*  navigates to a new page.
	*  \todo TEST THIS WORKS IN A BROWSER ENVIRONMENT
	*/
	BOOL onBeforeNavigate(int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Application module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

	/**
	*  This function is inherited from PBModule::onWindowChanged.
	*/
	BOOL onWindowChanged (int iEvent);

private:
	
	PPBCORESTRUCT m_pPBCoreStructure;		///< Pointer to callback functions provided by core
	bool isWindowMinimized;					///< To check whether the window is currently minimized or not.

	/**
	*  Data associated with each instance of the Application module.
	*/
	struct INSTANCE_DATA
	{
		int instanceID;						///< Unique Application Identifier.
		TCHAR tcApplicationEvent[MAX_URL];	///< URI to navigate to when a change of Application State occurs
		TCHAR tcVersionEvent[MAX_URL];		///< URI to navigate to when receive a GetVersion method
	};

};
