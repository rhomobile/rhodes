// WTG_Scanner_PLG.cpp : Defines the entry point for the DLL application.
//
#include <windows.h>
#include "ScannerModule.h"
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
*/

CScannerModule *g_pScannerModule;

BOOL Scanner_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pScannerModule)
	{
		g_pScannerModule = new CScannerModule (pPBCoreStructure);

		if (!g_pScannerModule)
			return false;
	}

	return g_pScannerModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Scanner_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pScannerModule)
		return g_pScannerModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Scanner_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pScannerModule)
	{
		iniRet = g_pScannerModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pScannerModule;
			g_pScannerModule = 0;
		}
	}

	return true;
}

