
/******************************************************************************/
#include <windows.h>
#include "..\..\..\Common\Public\PBplugin.h"
#include "..\..\..\Common\Public\PBUtil.h"
#include "ControlsModule.h"

/******************************************************************************/
CControlsModule *pControlsModule;

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
BOOL Controls_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!pControlsModule)
	{
		// Take copy of core struct so we have pointers to the browser functions
		// OK to do now as PBCoreStruct is static member
		memcpy (&(pControlsModule->PBCoreStruct), pPBCoreStructure, sizeof (PBCORESTRUCT));

		pControlsModule = new CControlsModule ();

		if (!pControlsModule)
			return false;

		return pControlsModule->Preload (pPBStructure, pPBCoreStructure);
	}

	return true;
}

/******************************************************************************/
BOOL Controls_Dispose (PPBSTRUCT pPBStructure)
{
	if (pControlsModule)
	{
		if (pControlsModule->Dispose(pPBStructure) == DEINITIALISED)
		{
			delete pControlsModule;
			pControlsModule = 0;
		}
	}

	return true;
}

/******************************************************************************/
BOOL Controls_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, LPCWSTR control)
{
	if (pControlsModule)
	{
		pControlsModule->SetControlType (control);
		return pControlsModule->Process (pbMetaStructure, pPBStructure);
	}
	else
		return false;
}

/******************************************************************************/
#define EXPORT_TYPE extern "C" __declspec(dllexport)

#define ADD_EXPORTS(x) \
	EXPORT_TYPE BOOL x##_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure) \
		{return Controls_Preload (pPBStructure, pPBCoreStructure);} \
	EXPORT_TYPE BOOL x##_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) \
{return Controls_MetaProc (pbMetaStructure, pPBStructure, L#x);} \
	EXPORT_TYPE BOOL x##_Dispose (PPBSTRUCT pPBStructure) \
		{return Controls_Dispose (pPBStructure);}

/******************************************************************************/
ADD_EXPORTS (QuitButton)
ADD_EXPORTS (ReloadButton)
ADD_EXPORTS (AddressBar)
ADD_EXPORTS (TopCommandArea)
ADD_EXPORTS (BottomCommandArea)
ADD_EXPORTS (BackButton)
ADD_EXPORTS (ForwardButton)
ADD_EXPORTS (GoButton)
ADD_EXPORTS (HomeButton)
ADD_EXPORTS (MinimizeButton)
ADD_EXPORTS (ZoomInPageButton)
ADD_EXPORTS (ZoomOutPageButton)
ADD_EXPORTS (SIPButton)
ADD_EXPORTS (StopButton)
ADD_EXPORTS (ZoomTextButton)
ADD_EXPORTS (KeyState)
ADD_EXPORTS (CustomButton)
ADD_EXPORTS (Scroll)
ADD_EXPORTS (Battery)
ADD_EXPORTS (Signal)
