/**
*  \file SIPModule.h
*  \brief MoveSIP Plug-in module interface.
*  File to house the CMoveSIP Module class and all related includes / defines
*  to interface between Browser core and the MoveSIP functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "../../Common/Private/SIP.h"
#include "sipapi.h"

/**
*  Class to define the interface between the core and the MoveSIP functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate MoveSIP behaviour.
*/
class CSIPModule: public PBModule
{
public:
	/**
	*  Overrides the default constructor to store a pointer to the PB Core 
	*  structure.  Store a local pointer to the core structure which contains
	*  function pointer to inform core of screen orientation change.
	*/
	CSIPModule(PPBCORESTRUCT pPBCoreStructure);

	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onDeInit.  
	*  Please see that function for further definition.
	*  Deregisters the OnAppFocus event.
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates an object to hold the current instance identifier.
	*  \todo POPULATE THE INSTANCE DATA WITH THE HTML HWND... CURRENTLY USING
	*  THE OVERALL WINDOW HANDLE
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
	*  This Function is inherited from PBModule::onAppFocus.
	*  Please see that function for further definition.
	*  Receive change of application focus notifications, if the new ID 
	*  represents an application using this module move the SIP as necessary.
	*/
	BOOL onAppFocus(int iOldID, int iNewID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the MoveSIP module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

	/**
	*  This function is inherited from PBModule::onPrimaryMessage.
	*  Please see that function for further definition.
	*  WM_SETTINGCHANGE is registered for, this message is sent when the SIP is 
	*  displayed or hidden.
	*/
	BOOL onPrimaryMessage (MSG *pmsg, int ncurrid);

	
private:
	/**
	*  Data associated with each instance of the ScreenOrientation
	*  module.
	*/
	struct INSTANCE_DATA
	{
		int instanceID;			///< Unique Application Identifier.
		CSIP* pSIPController;	///< Instance of CSIP Associated with this module
		SIP_CONTROL_VALUES sipControl;	///< Current Value of the SIP Control Variable
		HWND hwndHTML;			///< HWND of the HTML Window we're associated with (For Use in OnAppFocus)
		int left;				///< The Left position of the SIP as requested by this module
		int top;				///< The Top Position of the SIP as requested by this module
		DWORD dwLastSettingChangeTime;
	};
	PB_ScreenMode m_bFullScreen;	///< The value of the 'FullScreen' Configuration Setting
	bool	m_bFingerScrollingEnabled;	///<  True if we are using finger scrolling
	PPBCORESTRUCT m_pPBCoreStructure;		///< Pointer to callback functions provided by core

};