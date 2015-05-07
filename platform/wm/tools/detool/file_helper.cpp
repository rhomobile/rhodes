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

#include <rapi.h>
#include <strsafe.h>

#include "detool.h"
#include "LogServer.h"
#include "rapi_helper.h"

#define RHOSETUP_DLL "rhosetup.dll"
#define RE2_RUNTIME TEXT("\\Program Files\\RhoElements\\RhoElements.exe")

extern TCHAR *app_name;

namespace file
{

bool wcePutFile(const char *host_file, const char *wce_file)
{
	TCHAR tszSrcFile[MAX_PATH];
	WCHAR wszDestFile[MAX_PATH];
	BYTE  buffer[5120];
    WIN32_FIND_DATA wfd;
	HRESULT hr;
	DWORD dwAttr, dwNumRead, dwNumWritten;
	HANDLE hSrc, hDest, hFind;
	int nResult;

#ifdef UNICODE
	nResult = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
									host_file, strlen(host_file)+1,
									tszSrcFile, ARRAYSIZE(tszSrcFile));
	if(0 == nResult)
		return false;
#else
	hr = StringCchCopy(tszSrcFile, ARRAYSIZE(tszSrcFile), argv[1]);
	if(FAILED(hr))
		return false;
#endif
	nResult = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
									wce_file, strlen(wce_file)+1,
									wszDestFile, ARRAYSIZE(wszDestFile));
    if(0 == nResult)
        return false;

    hFind = FindFirstFile( tszSrcFile, &wfd);
    if (INVALID_HANDLE_VALUE == hFind) {
        _tprintf(TEXT("Host file does not exist\n"));
        return false;
    }
    FindClose( hFind);
	if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        _tprintf( TEXT("Host file specifies a directory\n"));
        return false;
    }
	
    if (rapi::wceConnect()) {
		dwAttr = CeGetFileAttributes( wszDestFile);
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
            hr = StringCchCatW(wszDestFile, ARRAYSIZE(wszDestFile), L"\\");
            if(FAILED(hr)) return false;
#ifdef UNICODE
            hr = StringCchCatW(wszDestFile, ARRAYSIZE(wszDestFile), wfd.cFileName);
            if(FAILED(hr)) return false;
#else
            nResult = MultiByteToWideChar(
                        CP_ACP,    
                        MB_PRECOMPOSED,
                        wfd.cFileName,
                        strlen(wfd.cFileName)+1,
                        wszDestFile+wcslen(wszDestFile),
                        ARRAYSIZE(wszDestFile)-wcslen(wszDestFile));
            if(0 == nResult)
            {
                return 1;
            }
#endif
		}
		hSrc = CreateFile(tszSrcFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hSrc) {
			_tprintf( TEXT("Unable to open host file\n"));
			return false;
		}

		hDest = CeCreateFile(wszDestFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hDest ) {
			_tprintf( TEXT("Unable to open target WinCE file\n"));
			return false;
		}

		//copy file
		do {
			if(ReadFile(hSrc, &buffer, sizeof(buffer), &dwNumRead, NULL)) {
				if (!CeWriteFile(hDest, &buffer, dwNumRead, &dwNumWritten, NULL)) {
					_tprintf( TEXT("Error !!! Writing WinCE file\n"));
					goto FatalError;
				}
			} else {
				_tprintf( TEXT("Error !!! Reading host file\n"));
				goto FatalError;
			}
			_tprintf( TEXT("."));                                        
		} while (dwNumRead);
		//_tprintf( TEXT("\n"));

		CeCloseHandle( hDest);
		CloseHandle (hSrc);
	}
    rapi::wceDisconnect();
	return true;

FatalError:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);
    rapi::wceDisconnect();
	return false;
}

bool wceRunProcess(const char *process, const char *args)
{
#ifndef UNICODE
	HRESULT hr;
#endif
	PROCESS_INFORMATION pi;    
	WCHAR wszProgram[MAX_PATH];
	WCHAR wszArgs[MAX_PATH];

#ifdef UNICODE
	int nResult = 0;
	nResult = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, process, strlen(process)+1, wszProgram, ARRAYSIZE(wszProgram));
	if(0 == nResult) { return false;}
	if (args) {
		nResult = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, args, strlen(args)+1, wszArgs, ARRAYSIZE(wszArgs));
		if(0 == nResult) 
			return false;
	}
#else
	hr = StringCchCopy(wszProgram, ARRAYSIZE(wszProgram), argv[1]);
	if(FAILED(hr)) return true;
#endif
    if (rapi::wceConnect()) 
    {
		if (!CeCreateProcess(wszProgram, wszArgs, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi)) 
        {
			_tprintf( TEXT("CreateProcess failed with Errorcode = %ld\n"), CeGetLastError());
			return false;
		}
		
        CeCloseHandle( pi.hProcess);
		CeCloseHandle( pi.hThread);
	}

    rapi::wceDisconnect();
    return true;
}

bool wceInvokeCabSetup(const char *wceload_params)
{
	HRESULT hr = S_OK;
	WCHAR wszCabFile[MAX_PATH];

	//convert pathname
	int nResult = 0;
	int len = strlen(wceload_params)+1;
	nResult = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, wceload_params, len, wszCabFile, ARRAYSIZE(wszCabFile));
	if(0 == nResult)
		return false;

    rapi::wceConnect();

	DWORD dwInSize = sizeof(wszCabFile);
	DWORD dwOutSize = 0;
	BYTE *pInBuff = NULL;
	
	pInBuff = (BYTE *)LocalAlloc(LPTR, dwInSize);
	memcpy(pInBuff, &wszCabFile, dwInSize);

	hr = CeRapiInvoke(TEXT("\\rhosetup"), TEXT("rhoCabSetup"), dwInSize, pInBuff, &dwOutSize, NULL, NULL, 0);
	if(FAILED(hr)) {
		//printf("Failed to setup cab!\r\n");
		return false;
	}
    rapi::wceDisconnect();

	return true ;
}

int copyExecutable (TCHAR *file_name, TCHAR *app_dir, bool overwrite_existing_file, bool use_shared_runtime)
{
	TCHAR exe_fullpath[MAX_PATH];
	int retval = 0;
	HANDLE hDest, hSrc;
	BYTE  buffer[5120];
	DWORD dwNumRead, dwNumWritten;

	USES_CONVERSION;
	
	_tcscpy(exe_fullpath, app_dir);
	_tcscat(exe_fullpath, _T("\\"));
	_tcscat(exe_fullpath, app_name);
	_tcscat(exe_fullpath, (use_shared_runtime ? _T(".lnk") : _T(".exe")));

    if (doMakeCopyFile(exe_fullpath, file_name, overwrite_existing_file))
    {
	    hSrc = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	    if (INVALID_HANDLE_VALUE == hSrc) 
        {
		    _tprintf( TEXT("Unable to open host file\n"));
		    return EXIT_FAILURE;
	    }

	    hDest = CeCreateFile(exe_fullpath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	    if (INVALID_HANDLE_VALUE == hDest ) 
        {
		    _tprintf( TEXT("Unable to open target WinCE file\n"));
		    return CeGetLastError();
	    }

	    do 
        {
		    if(ReadFile(hSrc, &buffer, sizeof(buffer), &dwNumRead, NULL)) 
            {
			    if (!CeWriteFile(hDest, &buffer, dwNumRead, &dwNumWritten, NULL)) 
                {
				    _tprintf( TEXT("Error !!! Writing WinCE file\n"));
				    goto copyFailure;
			    }
		    }
            else 
            {
				    _tprintf( TEXT("Error !!! Reading host file\n"));
				    goto copyFailure;
		    }
		    _tprintf( TEXT("."));                                        
	    }
        while (dwNumRead);
	    _tprintf( TEXT("\n"));

	    CeCloseHandle( hDest);
	    CloseHandle (hSrc);

        changeFileTimeInDest(exe_fullpath, file_name);
    }

	return EXIT_SUCCESS;

copyFailure:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);

	return EXIT_FAILURE;
}

bool doMakeCopyFile(TCHAR *deviceFilePath, TCHAR* hostFilePath, bool overwriteExistingFile)
{
    HANDLE hSrc = CreateFile(hostFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hSrc) 
    {
        return false;
    }

    FILETIME hostCreateTime, hostLastAccessTime, hostLastWriteTime;
    GetFileTime(hSrc, &hostCreateTime, &hostLastAccessTime, &hostLastWriteTime);
    CloseHandle (hSrc);

    HANDLE hDest = CeCreateFile(deviceFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!overwriteExistingFile && INVALID_HANDLE_VALUE != hDest)
    {
        CeCloseHandle(hDest);
        return false;
    }

    FILETIME deviceCreateTime, deviceLastAccessTime, deviceLastWriteTime;
    CeGetFileTime(hDest, &deviceCreateTime, &deviceLastAccessTime, &deviceLastWriteTime);
    CeCloseHandle(hDest);

    if (INVALID_HANDLE_VALUE == hDest) 
    {
        return true;
    }

    SYSTEMTIME stDeviceLastWriteTime, stHostLastWriteTime;
    FileTimeToSystemTime(&deviceLastWriteTime, &stDeviceLastWriteTime);
    FileTimeToSystemTime(&hostLastWriteTime, &stHostLastWriteTime);

    if (stDeviceLastWriteTime.wYear >=  stHostLastWriteTime.wYear) {
        if (stDeviceLastWriteTime.wMonth >= stHostLastWriteTime.wMonth) {
            if (stDeviceLastWriteTime.wDay >= stHostLastWriteTime.wDay) {
                if (stDeviceLastWriteTime.wHour >= stHostLastWriteTime.wHour) {
                    if (stDeviceLastWriteTime.wMinute >= stHostLastWriteTime.wMinute) {
                        if ((stDeviceLastWriteTime.wSecond + 1 >= stHostLastWriteTime.wSecond) && 
                            (stDeviceLastWriteTime.wSecond - 1 <= stHostLastWriteTime.wSecond)) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool changeFileTimeInDest(TCHAR *deviceFilePath, TCHAR* hostFilePath)
{
    HANDLE hSrc = CreateFile(hostFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hSrc) 
    {
        _tprintf( TEXT("Unable to open host file\n"));
        return false;
    }

    FILETIME hostCreateTime, hostLastAccessTime, hostLastWriteTime;
    GetFileTime(hSrc, &hostCreateTime, &hostLastAccessTime, &hostLastWriteTime);

    HANDLE hDest = CeCreateFile(deviceFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hDest ) 
    {
        _tprintf( TEXT("Unable to open target WinCE file\n"));
        return false;
    }

    CeSetFileTime(hDest, &hostCreateTime, &hostLastAccessTime, &hostLastWriteTime);

    CeCloseHandle(hDest);
    CloseHandle(hSrc);

    return true;
}

int copyBundle (TCHAR *parent_dir, TCHAR *file, TCHAR *app_dir)
{
	HANDLE           fileHandle;
	WIN32_FIND_DATAW findData;
	//DWORD			 dwError;
	TCHAR			 new_app_item[MAX_PATH];
	TCHAR			 host_file[MAX_PATH];
	HANDLE hFind;
	CE_FIND_DATA wceFindData;

	USES_CONVERSION;

	TCHAR wildcard[MAX_PATH + 16];
	TCHAR fullpath[MAX_PATH];

	_tcscpy(fullpath, parent_dir);
	_tcscat(fullpath, _T("\\"));
	_tcscat(fullpath, file);

	//TODO: check for fullpath is a dir

	_tcscpy(wildcard, fullpath);
	_tcscat(wildcard, _T("\\*.*"));

	fileHandle = FindFirstFile(wildcard, &findData);

	if (fileHandle == INVALID_HANDLE_VALUE) {
		printf ("Failed to open file\n");
        rapi::wceDisconnect();
		return EXIT_FAILURE;
	}

	HANDLE hDest, hSrc;
	BYTE  buffer[5120];
	DWORD dwNumRead, dwNumWritten;

	while (FindNextFile(fileHandle, &findData)) 
    {
		if (_tcscmp (_T("."), findData.cFileName) == 0 || _tcscmp (_T(".."), findData.cFileName) == 0)
			continue;

		_tcscpy(new_app_item, app_dir);
		_tcscat(new_app_item, _T("\\"));
		_tcscat(new_app_item, findData.cFileName);

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{	
			//Check and create dir on device
			hFind = CeFindFirstFile(new_app_item, &wceFindData);
			if (INVALID_HANDLE_VALUE == hFind) 
            {
				if (!CeCreateDirectory(new_app_item, NULL)) {
					_tprintf( TEXT("Create directory \"%s\" on device\n"), new_app_item);
					printf ("Failed to create new directory on device\n");
					return EXIT_FAILURE;
				}
			}

			FindClose( hFind);
			
			copyBundle (fullpath, findData.cFileName, new_app_item);
		} 
		else 
		{
			_tcscpy(host_file, fullpath);
			_tcscat(host_file, _T("\\"));
			_tcscat(host_file, findData.cFileName);

			_tprintf( TEXT("Copy file \"%s\" to device"), new_app_item);

            if (doMakeCopyFile(new_app_item, host_file))
            {
			    hSrc = CreateFile(host_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			    if (INVALID_HANDLE_VALUE == hSrc) 
                {
				    _tprintf( TEXT("Unable to open host file\n"));
				    return false;
			    }

                hDest = CeCreateFile(new_app_item, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (INVALID_HANDLE_VALUE == hDest ) 
                {
                    _tprintf( TEXT("Unable to open target WinCE file\n"));
                    return false;
                }

                do
                {
                    if(ReadFile(hSrc, &buffer, sizeof(buffer), &dwNumRead, NULL)) 
                    {
                        if (!CeWriteFile(hDest, &buffer, dwNumRead, &dwNumWritten, NULL)) 
                        {
                            _tprintf( TEXT("Error !!! Writing WinCE file\n"));
                            goto copyBundleFailure;
                        }
                    } 
                    else 
                    {
                        _tprintf( TEXT("Error !!! Reading host file\n"));
                        goto copyBundleFailure;
                    }
                    _tprintf( TEXT("."));                                        
                } 
                while (dwNumRead);
                _tprintf( TEXT("\n"));

                CeCloseHandle(hDest);
                CloseHandle (hSrc);

                changeFileTimeInDest(new_app_item, host_file);
            }
            else
            {
                _tprintf( TEXT(" POSTPONE\n"));
            }
		}
	}

	return EXIT_SUCCESS;

copyBundleFailure:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);
	return EXIT_FAILURE;
}

int copyLicenseDll (TCHAR *file_name, TCHAR *app_dir)
{
	TCHAR fullpath[MAX_PATH];
	int retval = 0;
	HANDLE hDest, hSrc;
	BYTE  buffer[5120];
	DWORD dwNumRead, dwNumWritten;

	USES_CONVERSION;
	
	_tcscpy(fullpath, app_dir);
	_tcscat(fullpath, _T("\\license_rc.dll"));

    if (doMakeCopyFile(fullpath, file_name))
    {
        hSrc = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hSrc) 
        {
            _tprintf( TEXT("Unable to open host file\n"));
            return EXIT_FAILURE;
        }

        hDest = CeCreateFile(fullpath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hDest ) 
        {
            _tprintf( TEXT("Unable to open target WinCE file\n"));
            return CeGetLastError();
        }

        do 
        {
            if(ReadFile(hSrc, &buffer, sizeof(buffer), &dwNumRead, NULL)) 
            {
                if (!CeWriteFile(hDest, &buffer, dwNumRead, &dwNumWritten, NULL)) 
                {
                    _tprintf( TEXT("Error !!! Writing WinCE file\n"));
                    goto copyFailure;
                }
            } 
            else 
            {
                _tprintf( TEXT("Error !!! Reading host file\n"));
                goto copyFailure;
            }
            _tprintf( TEXT("."));                                        
        } 
        while (dwNumRead);

        _tprintf( TEXT("\n"));

        CeCloseHandle( hDest);
        CloseHandle (hSrc);

        changeFileTimeInDest(fullpath, file_name);
    }

	return EXIT_SUCCESS;

copyFailure:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);

	return EXIT_FAILURE;
}

} //end of file