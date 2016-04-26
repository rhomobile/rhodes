/**
*  \file NotificationModule.h
*  \brief Notification Plug-in module interface.
*  File to house the CNotification Module class and all related includes / defines
*  to interface between Browser core and the Notification functionality.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"
#include "Notification.h"

/**
*  Class to define the interface between the core and the Notification functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Notification behaviour.
*/
class CNotificationModule: public PBModule
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
	*  Deregisters for OnAppFocus events.
	*/
	void onDeInit(PPBSTRUCT pPBStructure);

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
	*  This Function is inherited from PBModule::onBeforeNavigate.  
	*  Please see that function for further definition.
	*  Clears the Navigation string registered to EnumDeviceNotifications.
	*  \todo NEEDS TESTING IN A BROWSER ENVIRONMENT
	*/
	BOOL onBeforeNavigate	(int iInstID);

	/**
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Notification module and actions
	*  them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

	/**
	*  Provides backwards compatibility with PB2.x, the string to parse 
	*  is what used to be the content string for PB2.x.  The parameters contained
	*  therein are returned to the user via the DWORD [out] paramters.
	*  \param iInstanceID The owner Browser application.
	*  \param tcStringToParse Formally the PB2.x content string.
	*  \param dwNotification [out] The Type of Notification
	*  \param dwNotificationIndex [out] The Index specified for the Notification.
	*  \param dwParameter1 [out] The first paramter given by the user.
	*  \param dwParameter2 [out] The second paramter given by the user.
	*  \param dwParameter3 [out] The third parameter given by the user.
	*  \return Whether or not the parse was successful.
	*/
	BOOL ParseParams(int iInstanceID, LPCTSTR tcStringToParse, DWORD* dwNotificationType, 
		DWORD* dwNotificationIndex, DWORD* dwParameter1, DWORD* dwParameter2, DWORD* dwParameter3);
};