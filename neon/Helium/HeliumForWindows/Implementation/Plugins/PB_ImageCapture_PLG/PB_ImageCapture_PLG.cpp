/**
 * \file PB_ImageCapture_PLG.cpp
 * \brief Defines the entry point for the Image Capture plugin DLL.
 */
#include "PB_ImageCapture_PLG.h"
#include <windows.h>
#include <commctrl.h>
#include "../../common/public/pbplugin.h"
#include "ImagerMod.h"

//Instance of Image capture Module
CImagerMod *g_ImagerMod;

BOOL bImagerStarted=FALSE;

//Enum value to indicate current active image capture device, initialized to NONE
EnumImageCapDevType g_ActiveImageCapDevice = NONE;

BOOL APIENTRY DllMain( HANDLE /* hModule  --unused*/, 
                       DWORD  ul_reason_for_call, 
                       LPVOID /* lpReserved --unused */
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
/*
// This is an example of an exported variable
PB_IMAGECAPTURE_PLG_API int nPB_ImageCapture_PLG=0;

// This is an example of an exported function.
PB_IMAGECAPTURE_PLG_API int fnPB_ImageCapture_PLG(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see PB_ImageCapture_PLG.h for the class definition
CPB_ImageCapture_PLG::CPB_ImageCapture_PLG()
{ 
	return; 
}
*/


////////////////////////////////////////////////////////////////////////
// Function:	Imager_Preload
// Description:	Called each time an instance is created
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL Imager_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{

	if(pPBStructure->hWnd == NULL)
		return FALSE;

	//m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	//Create an instance of CWlanRadioMod Class (The Module)
	// and  call preload on that instance
	if(g_ImagerMod == NULL)
	{
		g_ImagerMod = new CImagerMod();

		if(g_ImagerMod ==NULL){
			//FAILED to allocate memory
			return FALSE;
		}

	}

	return g_ImagerMod->Preload(pPBStructure, pPBCoreStructure);

}
////////////////////////////////////////////////////////////////////////
// Function:	Imager_MetaProc
// Description:	To invoke Meta Tag processing
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL Imager_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the FileTransfer object
	if (g_ImagerMod != NULL)
		return g_ImagerMod->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
	
	return TRUE;
}
////////////////////////////////////////////////////////////////////////
// Function:	Imager_Dispose
// Description:	Deallocate all memory associated with the Imager object
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL Imager_Dispose(PPBSTRUCT pPBStructure) 
{

	
	if (g_ImagerMod != NULL)
	{
		if(g_ImagerMod->Dispose(pPBStructure)== DEINITIALISED){//>the MODULE will return DEINITIALIZED when last object has been disposed off
			delete g_ImagerMod;
			g_ImagerMod = NULL;
		}
		return TRUE;
	}
	return FALSE;
}