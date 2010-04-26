// rhosetup.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

// Exported function prototypes
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) INT rhoCabSetup (DWORD, BYTE *, DWORD *, BYTE **, PVOID);
#ifdef __cplusplus
}
#endif



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

INT rhoCabSetup (DWORD inSize, BYTE *inBuf, DWORD *, BYTE **, PVOID)
{
	PROCESS_INFORMATION pi;
	BOOL res = FALSE;
	DWORD exit_code = -1;

	printf ("Trying to install cab\n");
	
	res = CreateProcess( _T("\\windows\\wceload.exe"), (LPWSTR )inBuf,  0, 0, 0, 0, 0, 0, 0, &pi);
	if (res) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		
		GetExitCodeProcess(pi.hProcess, &exit_code);
		printf ("Done. Exit code == %d\n", exit_code);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		LocalFree(inBuf);
		return 1;
	}

	LocalFree(inBuf);
	printf ("Failed\n");
	return -1;
}