/******************************************************************************/
#include <windows.h>
#include "PushModule.h"

/******************************************************************************/
CPushModule *pPushModule;

/******************************************************************************/
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

/******************************************************************************/
BOOL Push_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!pPushModule)
	{
		pPushModule = new CPushModule ();

		if (!pPushModule)
			return false;
	}

	return pPushModule->Preload (pPBStructure, pPBCoreStructure);
}

/******************************************************************************/
BOOL Push_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (pPushModule)
		return pPushModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

/******************************************************************************/
BOOL Push_Dispose (PPBSTRUCT pPBStructure) 
{
	if (pPushModule)
	{
		if (pPushModule->Dispose(pPBStructure) == DEINITIALISED)
		{
			delete pPushModule;
			pPushModule = 0;
		}
	}

	return true;
}
