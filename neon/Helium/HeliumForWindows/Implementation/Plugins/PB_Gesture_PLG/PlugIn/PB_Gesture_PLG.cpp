/******************************************************************************/
#include <windows.h>
#include "GestureModule.h"

/******************************************************************************/
CGestureModule *pGestureModule;

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
		break; 
		// Application crach observed during Tilt and Shake gesture execution
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if (pGestureModule)
			pGestureModule->DisableIST();
		break;
	}
    return TRUE;
}

/******************************************************************************/
BOOL Gesture_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!pGestureModule)
	{
		pGestureModule = new CGestureModule (pPBCoreStructure);

		if (!pGestureModule)
			return false;
	}

	return pGestureModule->Preload (pPBStructure, pPBCoreStructure);
}

/******************************************************************************/
BOOL Gesture_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (pGestureModule)
		return pGestureModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

/******************************************************************************/
BOOL Gesture_Dispose (PPBSTRUCT pPBStructure) 
{
	if (pGestureModule)
	{
		if (pGestureModule->Dispose(pPBStructure) == DEINITIALISED)
		{
			delete pGestureModule;
			pGestureModule = 0;
		}
	}

	return true;
}
