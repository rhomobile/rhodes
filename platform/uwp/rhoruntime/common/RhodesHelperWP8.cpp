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

#include "RhodesHelperWP8.h"

#include "../../shared/common/RhoPort.h"
#include "../../shared/common/RhoStd.h"
#include "../../shared/common/StringConverter.h"

LPWSTR UWPLocalFolder = L"";
LPWSTR UWPNullFile = L"nullfile";
LPSTR UWPLocalRhoFolder = "rho";

int versionMajor;
int versionMinor;
int versionBuild;

void cSetOSVersion(int major, int minor, int build) {
	versionMajor = major;
	versionMinor = minor;
	versionBuild = build;
}

LPCWSTR getLocalNullFile() {
	return UWPNullFile;
}
LPCWSTR getLocalStorage() {
	return UWPLocalFolder;
}
LPCSTR getLocalRhoFolder() {
	return UWPLocalRhoFolder;
}

void setLocalStorage(LPCWSTR path) {
	UWPLocalFolder = const_cast<LPWSTR>(path);
}
void setLocalNullFile(LPCWSTR filename) {
	UWPNullFile = const_cast<LPWSTR>(filename);
}
void setLocalRhoFolder(LPCSTR path) {
	UWPLocalRhoFolder = const_cast<LPSTR>(path);
}

HANDLE Rho_CreateFileW(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
	)
{
	CREATEFILE2_EXTENDED_PARAMETERS extendedParameters;
	extendedParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParameters.dwFileAttributes = dwFlagsAndAttributes & 0x0003FFF7;
	extendedParameters.dwFileFlags = dwFlagsAndAttributes & 0xFF3C0000;
	extendedParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParameters.lpSecurityAttributes = lpSecurityAttributes;
	extendedParameters.hTemplateFile = hTemplateFile;
	return CreateFile2(lpFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, &extendedParameters);
}


BOOL Rho_GetVersionExW(
	_Inout_ LPOSVERSIONINFOW lpVersionInformation
	)
{
	// TODO: use System.Environment.OSVersion.Version from C#
	lpVersionInformation->dwMajorVersion = versionMajor;
	lpVersionInformation->dwMinorVersion = versionMinor;
	lpVersionInformation->dwBuildNumber = versionBuild;
	return TRUE;
}

DWORD Rho_GetFileSize(
	_In_ HANDLE hFile,
	_Out_opt_ LPDWORD lpFileSizeHigh
	)
{
	FILE_STANDARD_INFO info;
	return GetFileInformationByHandleEx(hFile, FileStandardInfo, &info, sizeof(info)) ? info.EndOfFile.QuadPart : 0;
}

DWORD Rho_GetFileAttributesA(
	_In_ LPCSTR lpFileName
	)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInformation;
	if (GetFileAttributesExW(rho::common::convertToStringW(lpFileName).c_str(), GetFileExInfoStandard, &fileInformation))
		return fileInformation.dwFileAttributes;
	else
		return INVALID_FILE_ATTRIBUTES;
}

DWORD Rho_GetFileAttributesW(
	_In_ LPCWSTR lpFileName
	)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInformation;
	if (GetFileAttributesExW(lpFileName, GetFileExInfoStandard, &fileInformation))
		return fileInformation.dwFileAttributes;
	else
		return INVALID_FILE_ATTRIBUTES;
}

DWORD Rho_GetModuleFileNameW(
	_In_opt_ HMODULE hModule,
	_Out_writes_to_(nSize, ((return < nSize) ? (return +1) : nSize)) LPWSTR lpFilename,
	_In_ DWORD nSize
	)
{
	return 0;
}

DWORD Rho_SetFilePointer(
	_In_ HANDLE hFile,
	_In_ LONG lDistanceToMove,
	_Inout_opt_ PLONG lpDistanceToMoveHigh,
	_In_ DWORD dwMoveMethod
	)
{
	LARGE_INTEGER x;
	LARGE_INTEGER res;
	x.QuadPart = lDistanceToMove;
	return SetFilePointerEx(hFile, x, &res, dwMoveMethod) ? res.QuadPart : 0;
}

BOOL Rho_LocalFileTimeToFileTime(
	_In_ CONST FILETIME * lpLocalFileTime,
	_Out_ LPFILETIME lpFileTime
	)
{
	*lpFileTime = *lpLocalFileTime;
	return TRUE;
}

BOOL Rho_UnlockFile(
	_In_ HANDLE hFile,
	_In_ DWORD dwFileOffsetLow,
	_In_ DWORD dwFileOffsetHigh,
	_In_ DWORD nNumberOfBytesToUnlockLow,
	_In_ DWORD nNumberOfBytesToUnlockHigh
	)
{
	OVERLAPPED ovlp;
	memset(&ovlp, 0, sizeof(OVERLAPPED));
	ovlp.Offset = dwFileOffsetLow;
	ovlp.OffsetHigh = dwFileOffsetHigh;
	return UnlockFileEx(hFile, 0, nNumberOfBytesToUnlockLow, nNumberOfBytesToUnlockHigh, &ovlp);
}

HANDLE Rho_OpenProcess(
	_In_ DWORD dwDesiredAccess,
	_In_ BOOL bInheritHandle,
	_In_ DWORD dwProcessId
	)
{
	return INVALID_HANDLE_VALUE;
}

HMODULE Rho_GetModuleHandleW(
	_In_opt_ LPCWSTR lpModuleName
	)
{
	return 0;
}

HMODULE Rho_GetModuleHandleA(
	_In_opt_ LPCSTR lpModuleName
)
{
	return 0;
}
