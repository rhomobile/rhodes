// PB_EMMP_PLG.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PB_EMMP_PLG.h"
#include "EMMPMod.h"

CEMMPMod	*pEMMPMod;

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

BOOL EMMLProfile_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
		
	if (pPBStructure->hWnd == NULL)
		return FALSE;
	///>  Create an instance of the CEMMPMod Class (The Module)
	///>  and call preload on that instance.
	
	if (pEMMPMod == NULL)
	{
		pEMMPMod = new CEMMPMod();
		
		if(pEMMPMod == NULL){
			///>  Failed to allocate memory.
			return FALSE;
		}
	}
	return pEMMPMod->Preload(pPBStructure, pPBCoreStructure);
	
}

BOOL EMMLProfile_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the EMMP object
	if (pEMMPMod != NULL)
		return pEMMPMod->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

BOOL EMMLProfile_Dispose(PPBSTRUCT pPBStructure) 
{
	
	if (pEMMPMod != NULL){
		if(pEMMPMod->Dispose(pPBStructure)== DEINITIALISED){
			delete pEMMPMod;
			pEMMPMod = NULL;
		}
	}
	return TRUE;
}

