#include "stdafx.h"
#include "PB_CardReader_PLG.h"
#include "CardReaderMod.h"
#include "CardReader.h"

/**
 * Pointer to the module used in this plugin
 */
CCardReaderMod *pCardReaderMod = NULL;

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

#pragma region DLL_Exports

////////////////////////////////////////////////////////////////////////
// Function:	CardReader_Preload
// Description:	Create an instance of a CardReaderMod object to hold the state
//				common to all card readers, register for focus change
//				events, and call the CardReaderMod Preload.
//
// Scope:		Exported DLL function
//
// Return:		FALSE if it cannot instantiate the CCardReader object or the
//				generic CardReader Preload fails. TRUE otherwise.
//								
//
// Author:		Mike Schuette
// Change History:
//				June 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CardReader_Preload(PBStruct *pPBStructure, PBCoreStruct *pPBCoreStructure)
{
	//If pCardReaderMod not already set, then create an instance of the CCardReaderMod 
	//class (the module) and register for focus change events.
	if (pCardReaderMod == NULL)
	{
		pCardReaderMod = new CCardReaderMod();

		if (pCardReaderMod == NULL)
			//Failed to allocate memory
			return FALSE;
	}

	return pCardReaderMod->Preload(pPBStructure, pPBCoreStructure);
}

////////////////////////////////////////////////////////////////////////
// Function:	CardReader_MetaProc
// Description:	If the pointer to the CardReaderMod object is non-NULL, 
//				call its Process function.
//
// Scope:		Exported DLL function
//
// Return:		FALSE, if pointer to CardReaderMod object is NULL. TRUE otherwise.
//								
//
// Author:		Mike Schuette
// Change History:
//				June 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CardReader_MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure)
{
	if (pCardReaderMod)
		return pCardReaderMod->Process(pPBMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////
// Function:	CardReader_Dispose
// Description:	If CardReaderMod object exists, then call the CardReaderMod 
//				Dispose, unallocate the CardReaderMod
//				object and set the pCardReaderMod pointer to NULL.
//
// Scope:		Exported DLL function
//
// Return:		FALSE if the generic dispose returns FALSE. True otherwise.
//								
//
// Author:		Mike Schuette
// Change History:
//				June 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CardReader_Dispose(PBStruct *pPBStructure)
{
	if (pCardReaderMod)
	{
		INITRET disposeResult = pCardReaderMod->Dispose(pPBStructure);

		if (disposeResult == FAILED)
			return FALSE;
		else if (disposeResult == DEINITIALISED)
		{
			delete pCardReaderMod;
			pCardReaderMod = NULL;
		}
	}

	//Return true regardless of whether the pCardReaderMod pointer is NULL or not,
	//if it is NULL then the resources weren't previously allocated
	return TRUE;
}

#pragma endregion
