// PB_SmartCradle_PLG.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PB_SmartCradle_PLG.h"
#include "../../../common/public/pbplugin.h"
#include "SmartCradleModule.h"
#include <windows.h>
#include <commctrl.h>



CSmartCradleModule *g_SmartCradleModule;

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


BOOL SmartCradle_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{

	if(pPBStructure->hWnd == NULL)
		return FALSE;

	if(g_SmartCradleModule == NULL)
	{
		g_SmartCradleModule = new CSmartCradleModule();

		if(g_SmartCradleModule ==NULL){
			return FALSE;
		}

	}
	BOOL bret =g_SmartCradleModule->Preload(pPBStructure, pPBCoreStructure);
	return bret;

}



BOOL SmartCradle_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc
	if (g_SmartCradleModule != NULL)
	{
		BOOL bret= g_SmartCradleModule->Process(pbMetaStructure, pPBStructure);
		return bret;
	}
	else
		return FALSE;
	
	return TRUE;
}


BOOL SmartCradle_Dispose(PPBSTRUCT pPBStructure) 
{

	
	if (g_SmartCradleModule != NULL)
	{
		if(g_SmartCradleModule->Dispose(pPBStructure)== DEINITIALISED){//>the MODULE will return DEINITIALIZED when last object has been disposed off
			delete g_SmartCradleModule;
			g_SmartCradleModule = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

