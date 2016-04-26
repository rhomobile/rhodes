/**
*  \file ZoomModule.h
*  \brief Zoom Plug-in module interface.
*  File to house the CZoom Module class and all related includes / defines
*  to interface between Browser core and the Zoom functionality.
*  This module does not need to consider changes in application focus as the 
*  text size is stored and maintained as part of the engine component.
*/

#pragma once

#include "../../common/Public/PBPlugin.h"

/**
*  Class to define the interface between the core and the Zoom functionality.
*  Inherits from PBModule to align with the Plug-in design.  Actions meta tags
*  to perform the appropriate Zoom behaviour.
*/
class CZoomModule: public PBModule
{
public:
	/**
	*  Overrides the default constructor to store a pointer to the PB Core 
	*  structure.  Store a local pointer to the core structure which contains
	*  a function pointer to set the text size.
	*/
	CZoomModule(PPBCORESTRUCT pPBCoreStructure);

	/**
	*  This Function is inherited from PBModule::onInit.  
	*  Please see that function for further definition.
	*  Copies the Module name into the Module.
	*/
	BOOL onInit(PPBSTRUCT pPBStructure);

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
	*  This Function is inherited from PBModule::MetaProc.  
	*  Please see that function for further definition.
	*  Accepts 'Meta tags' associated with the Zoom module and actions
	*  them appropriately.
	*  \todo Text Zoom Enumeration should come from PBPlugin.h (or PB_Defines.h)
	*  I think but at the moment it is coming from the pbengine_defs.h.
	*  \todo Test Setting the text size in a browser environment.  At the moment
	*  the functions are just stubbed in the test container.
	*/
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);

private:

	PPBCORESTRUCT m_pPBCoreStructure;		///< Pointer to callback functions provided by core

	/**
	*  Data associated with each instance of the Zoom module.
	*/
	struct INSTANCE_DATA
	{
		int instanceID;						///< Unique Application Identifier.
	};

};