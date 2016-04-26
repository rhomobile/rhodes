/******************************************************************************/
#include <windows.h>
#include "KeyModule.h"

/******************************************************************************/
CKeyModule *pKeyModule;

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
EXPORT_TYPE BOOL KeyCapture_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	CKeyModule::pBrowserHome = pPBCoreStructure->pBrowserHome;

	BOOL retVal = TRUE;
	if (!pKeyModule)
	{
		pKeyModule = new CKeyModule (pPBCoreStructure);

		if (!pKeyModule)
			return false;

		retVal = pKeyModule->Preload (pPBStructure, pPBCoreStructure);
	}

	return retVal;
}

/******************************************************************************/
EXPORT_TYPE BOOL KeyCapture_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (pKeyModule)
		return pKeyModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

/******************************************************************************/
EXPORT_TYPE BOOL KeyCapture_Dispose (PPBSTRUCT pPBStructure) 
{
	if (pKeyModule)
	{
		if (pKeyModule->Dispose(pPBStructure) == DEINITIALISED)
		{
			delete pKeyModule;
			pKeyModule = 0;
		}
	}

	return true;
}

/******************************************************************************/
// OnKey module for backward compatibility
/******************************************************************************/
EXPORT_TYPE BOOL OnKey_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	CKeyModule::pBrowserHome = pPBCoreStructure->pBrowserHome;

	return KeyCapture_Preload (pPBStructure, pPBCoreStructure);
}

/******************************************************************************/
EXPORT_TYPE BOOL OnKey_Dispose (PPBSTRUCT pPBStructure) 
{
	return KeyCapture_Dispose (pPBStructure);
}

/******************************************************************************/
EXPORT_TYPE BOOL OnKey_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	return KeyCapture_MetaProc (pbMetaStructure, pPBStructure);
}
