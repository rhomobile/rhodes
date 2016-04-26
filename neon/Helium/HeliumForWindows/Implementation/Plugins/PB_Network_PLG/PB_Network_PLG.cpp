// PB_Network_PLG.cpp : Defines the entry point for the DLL application.
//
#include <windows.h>
#include "NetworkModule.h"

CNetworkModule *g_pNetworkModule;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


BOOL Network_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pNetworkModule)
	{
		g_pNetworkModule = new CNetworkModule ();

		if (!g_pNetworkModule)
			return false;
	}

	return g_pNetworkModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Network_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pNetworkModule)
		return g_pNetworkModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Network_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pNetworkModule)
	{
		iniRet = g_pNetworkModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pNetworkModule;
			g_pNetworkModule = 0;
		}
	}

	return true;
}
