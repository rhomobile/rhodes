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

SOCKET Rho_WSASocket(
	_In_ int af,
	_In_ int type,
	_In_ int protocol,
	_In_opt_ LPWSAPROTOCOL_INFOA lpProtocolInfo,
	_In_ GROUP g,
	_In_ DWORD dwFlags
	)
{
	return WSASocketA(af, type, protocol, lpProtocolInfo, g, dwFlags);
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
	lpVersionInformation->dwMajorVersion = 8;
	lpVersionInformation->dwMinorVersion = 0;
	lpVersionInformation->dwBuildNumber = 0;
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
