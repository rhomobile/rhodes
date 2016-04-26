/**
* Light Sensor Module header file
*/

/******************************************************************************/
#pragma once

/******************************************************************************/

#include "..\..\Common\Public\pbplugin.h"
#include "LightSensor.h"

/**
Raw Sensor Module class derived from PBModule
*/
class CLightSensorModule: public PBModule
{
public:
	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module and registers to receive 
	*  navigation events.  
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
	*  Creates and Initialises a new CRawSensors Object.  Stores a reference to 
	*  the new object in the pInstStruct, associating it with the newly 
	*  created instance.
	*/
	BOOL onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::onReleaseInstance.  
	*  Please see that function for further definition.
	*  Deletes the CRawSensorsCapture object associated with the instance being
	*  released.
	*/
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct);

	/**
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Raw Sensor module and 
	*  actions them appropriately.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam);

	/**
	*  This Function is inherited from PBModule::OnBeforeNavigate.  
	*  Please see that function for further definition.
	*  Stops showing the Raw Sensor box.
	*/
	BOOL onBeforeNavigate(int iInstID);
};
