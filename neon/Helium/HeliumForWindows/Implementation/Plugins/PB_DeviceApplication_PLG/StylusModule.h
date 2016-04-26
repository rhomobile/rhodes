/**
*  \file StylusModule.h
*  \brief Stylus Plug-in module interface.
*  File to house the CStylus Module class and all related includes / defines
*  to interface between Browser core and the Stylus functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include <pm.h>

/**
*  Class to define the interface between the core and the Stylus functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Stylus behaviour.
*/
class CStylusModule: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers to receive the 
	*  onAppFocus event.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onDeInit.  
	*  Please see that function for further definition.
	*  Deregisters OnAppFocus event.
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

	/**
	*  This Function is inherited from PBModule::onAttachInstance.  
	*  Please see that function for further definition.
	*  Creates an object to hold whether or not the stylus is currently 
	*  enabled which will be passed to this module when it receives 
	*  focus events.
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
	*  represents an application using this module then set whether or not 
	*  the Stylus should be enabled accordingly.
	*/
	BOOL onAppFocus(int iOldID, int iNewID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Stylus module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
private:

	/**
	*  Type to define the current state of the stylus, whether it is enabled
	*  or disabled.  This is to provide a friendly mapping to the 
	*  CEDEVICE_POWER_STATE enumeration in the Windows API.
	*/
	typedef enum _STYLUS_STATE
	{
		STYLUS_ENABLED	= D0,		///< Stylus is Enabled.
		STYLUS_DISABLED_D3 = D3,	///< Stulus disabled for CE7 devices
		STYLUS_DISABLED	= D4		///< Stylus is Disabled.
	} STYLUS_STATE;

	/**
	*  Data associated with each instance of the Stylus module.
	*/
	struct INSTANCE_DATA
	{
		STYLUS_STATE currentStylusState;	///< Whether or not the Stylus is currently enabled or Disabled
	};

};
