/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
		if(exit_code != EXIT_SUCCESS)
			return -1;
		return 1;
	}

	LocalFree(inBuf);
	printf ("Failed\n");
	return -1;
}