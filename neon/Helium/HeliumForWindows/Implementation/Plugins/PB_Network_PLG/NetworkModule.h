/**
 *  \file NetworkModule.h
 *  \brief Network Plug-in module interface.  
 *  File to house the CNetwork Module class and all related includes / defines
 *  to interface between Browser core and the Network functionality.
 *  (Polling to see if an external server exists)
 */

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "Network.h"
#include "WAN.h"

class CNetworkInstanceData
{
public:
	CNetworkInstanceData();
	CNetwork*	m_pNetwork;
	CWAN*		m_pWAN;
};

/**
 *  Class to define the interface between the core and the Network Plug-in.
 *  Wraps the Functionality of CNetwork and inherits from PBModule to 
 *  align with the Plug-in design.  Instantiates / destroys CNetwork objects
 *  as appropriate and calls meta functions upon them.
 */
class CNetworkModule: public PBModule
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
	*  No Functionality for Telemetry, all closing down is done in onReleaseInstance
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates and Initialises a new CTelemetry Object.  Stores a reference to 
	*  the new object in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the CTelemetry object associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onBrowserNav.  
	*  Please see that function for further definition.
	*  Clears the registered Event (NavString) When the application
	*  navigates to a new page.
	*/
	BOOL onBeforeNavigate(int iInstID);

	/**
	*  This function is inherited from PBModule::onPrimaryMessage.
	*  Please see that function for further definition.
	*/
	BOOL onPrimaryMessage (MSG *pmsg, int ncurrid);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Telemetry module and invokes
	*  the appropriate functions on the associated CTelemetry object associated
	*  with this instance.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
};


