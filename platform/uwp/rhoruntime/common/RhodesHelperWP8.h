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
#pragma once

#include <windows.h>
//#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

	void setLocalStorage(LPCWSTR path);
	void setLocalNullFile(LPCWSTR filename);
	void setLocalRhoFolder(LPCSTR path);
	LPCWSTR getLocalNullFile();
	LPCWSTR getLocalStorage();
	LPCSTR getLocalRhoFolder();

	void cSetOSVersion(int major, int minor, int build);

	HANDLE Rho_CreateFileW(
		_In_ LPCWSTR lpFileName,
		_In_ DWORD dwDesiredAccess,
		_In_ DWORD dwShareMode,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_ DWORD dwCreationDisposition,
		_In_ DWORD dwFlagsAndAttributes,
		_In_opt_ HANDLE hTemplateFile
	);

	BOOL Rho_GetVersionExW(
		_Inout_ LPOSVERSIONINFOW lpVersionInformation
	);

	DWORD Rho_GetFileSize(
		_In_ HANDLE hFile,
		_Out_opt_ LPDWORD lpFileSizeHigh
	);

	DWORD Rho_GetFileAttributesA(
		_In_ LPCSTR lpFileName
	);

	DWORD Rho_GetFileAttributesW(
		_In_ LPCWSTR lpFileName
	);

	DWORD Rho_GetModuleFileNameW(
		_In_opt_ HMODULE hModule,
		_Out_writes_to_(nSize, ((return < nSize) ? (return +1) : nSize)) LPWSTR lpFilename,
		_In_ DWORD nSize
	);

	DWORD Rho_SetFilePointer(
		_In_ HANDLE hFile,
		_In_ LONG lDistanceToMove,
		_Inout_opt_ PLONG lpDistanceToMoveHigh,
		_In_ DWORD dwMoveMethod
	);

	BOOL Rho_UnlockFile(
		_In_ HANDLE hFile,
		_In_ DWORD dwFileOffsetLow,
		_In_ DWORD dwFileOffsetHigh,
		_In_ DWORD nNumberOfBytesToUnlockLow,
		_In_ DWORD nNumberOfBytesToUnlockHigh
	);

	BOOL Rho_LocalFileTimeToFileTime(
		_In_ CONST FILETIME * lpLocalFileTime,
		_Out_ LPFILETIME lpFileTime
	);

	HANDLE Rho_OpenProcess(
		_In_ DWORD dwDesiredAccess,
		_In_ BOOL bInheritHandle,
		_In_ DWORD dwProcessId
	);

	HMODULE Rho_GetModuleHandleW(
		_In_opt_ LPCWSTR lpModuleName
	);

	HMODULE Rho_GetModuleHandleA(
		_In_opt_ LPCSTR lpModuleName
	);

#ifdef __cplusplus
}
#endif //__cplusplus

#if defined(OS_UWP)// && !defined(_WINRT_DLL)
#define CreateFileW Rho_CreateFileW
#define CreateFileA Rho_CreateFileW
//#define CreateFile Rho_CreateFileW
#define GetVersionEx Rho_GetVersionExW
#define GetVersionExW Rho_GetVersionExW
#define GetFileSize Rho_GetFileSize
#define GetFileAttributesA Rho_GetFileAttributesA
#define GetFileAttributesW Rho_GetFileAttributesW
#define GetModuleFileNameW Rho_GetModuleFileNameW
#define GetModuleFileName Rho_GetModuleFileNameW
#define SetFilePointer Rho_SetFilePointer
#define UnlockFile Rho_UnlockFile
#define LocalFileTimeToFileTime Rho_LocalFileTimeToFileTime
#define OpenProcess Rho_OpenProcess
#define GetModuleHandleW Rho_GetModuleHandleW
#define GetModuleHandleA Rho_GetModuleHandleA
#endif
