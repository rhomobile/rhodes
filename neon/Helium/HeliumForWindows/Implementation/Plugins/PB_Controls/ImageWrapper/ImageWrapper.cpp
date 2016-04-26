/******************************************************************************/
#include <windows.h>
#include "aygshell.h"

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
extern "C" __declspec(dllexport) HBITMAP LoadImageFromFile (LPCWSTR filename)
{
	return SHLoadImageFile (filename);
}
