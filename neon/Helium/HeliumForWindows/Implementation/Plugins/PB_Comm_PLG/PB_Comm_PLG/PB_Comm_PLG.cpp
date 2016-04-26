//////////////////////////////////////////////////////////////////////////////
// Authors:		Phil May (MAY)
//
// Change History:
//		July 2009 - MAY First Created
//		Sept 2009 - MAY Multi-instance mods
//////////////////////////////////////////////////////////////////////////////

//

#include "PB_Comm_PLG.h"
#include "CommModule.h"


/**
 * DLL entry point.  Currently does nothing but return TRUE.
 */
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
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

// pointer to the instance of the CCommsModule class that is created 
// when Comms_Preload is called
CCommModule *pCommModule = NULL;

/*****************************************************************************/
BOOL Comm_Preload(PBStruct *pPBStructure, PBCoreStruct *pPBCoreStructure)
{
	// Check to see if the Comms module has already been instatitated
	if (pCommModule == NULL)
		pCommModule = new CCommModule();

	if (pCommModule)
		return pCommModule->Preload(pPBStructure, pPBCoreStructure);
	else
		return FALSE;
}

/*****************************************************************************/
BOOL Comm_MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure)
{
	// If the Comms object has been instantiated, call its MetaProc method
	if (pCommModule)
		return pCommModule->Process(pPBMetaStructure, pPBStructure);
	else
		return FALSE;
}

/*****************************************************************************/
BOOL Comm_Dispose(PBStruct *pPBStructure)
{
	// if the Comms object is instantiated, call its Dispose method, delete it,
	// and then set the pointer to NULL
	if (pCommModule)
	{
		INITRET disposeResult = pCommModule->Dispose(pPBStructure);

		if (disposeResult == FAILED)
			return FALSE;
		else if (disposeResult == DEINITIALISED)
		{
			delete pCommModule;
			pCommModule = NULL;
		}
	}

	return TRUE;
}
