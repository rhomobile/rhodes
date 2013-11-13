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
#include "rapi_helper.h"

#define RHOSETUP_DLL "rhosetup.dll"
#define RE2_RUNTIME TEXT("\\Program Files\\RhoElements\\RhoElements.exe")

namespace rapi
{

void checkMDEstart(HRESULT hr) 
{
	if (FAILED(hr)) {
		if (hr == REGDB_E_CLASSNOTREG) {
			printf ("Microsoft Device Emulator is too old or corrupted. Please update it.\n");
			printf ("You could get the latest version on:\n");
			printf ("http://www.microsoft.com/downloads/details.aspx?familyid=a6f6adaf-12e3-4b2f-a394-356e2c2fb114\n");
		} else {
			wprintf_s(L"Error: Unable to instantiate DeviceEmulatorManager. ErrorCode=0x%08X\n", hr);
		}
	}
}

DWORD WINAPI startDEM(LPVOID lpParam)
{
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
        HRESULT hr = 0;

		CComPtr<IDeviceEmulatorManager> pDeviceEmulatorManager;
		hr = pDeviceEmulatorManager.CoCreateInstance(__uuidof(DeviceEmulatorManager));
		if (FAILED(hr)) {
			checkMDEstart(hr);
			ExitProcess(EXIT_FAILURE);
			return hr;
		}
		
		pDeviceEmulatorManager->ShowManagerUI(false);
		while (1) {
			Sleep(600 * 1000);
		}
		return hr;
        CoUninitialize();
    }
	return 0;
}

BOOL FindDevice(const CComBSTR& deviceIdentifier, IDeviceEmulatorManagerVMID** pDeviceVMID)
{
    HRESULT hr;

    CComPtr<IDeviceEmulatorManager> pDeviceEmulatorManager;
    hr = pDeviceEmulatorManager.CoCreateInstance(__uuidof(DeviceEmulatorManager));
    if (FAILED(hr)) {
        wprintf(L"Error: Unable to instantiate DeviceEmulatorManager. ErrorCode=0x%08X\n", hr);
        return FALSE;
    }

    for (; SUCCEEDED(hr); (hr = pDeviceEmulatorManager->MoveNext()))
    {
        CComPtr<IEnumManagerSDKs> pSDKEnumerator;

		hr = pDeviceEmulatorManager->EnumerateSDKs(&pSDKEnumerator);
        if (FAILED(hr)) {
            continue;
        }

		for (; SUCCEEDED(hr); (hr = pSDKEnumerator->MoveNext())) {
            CComPtr<IEnumVMIDs> pDeviceEnumerator;
            hr = pSDKEnumerator->EnumerateVMIDs(&pDeviceEnumerator);
            if (FAILED(hr)) {
                continue;
            }

            for (; SUCCEEDED(hr); (hr = pDeviceEnumerator->MoveNext())) {
                CComBSTR deviceName;
                CComBSTR deviceVMID;
                CComPtr<IDeviceEmulatorManagerVMID> pDevice;

                hr = pDeviceEnumerator->GetVMID(&pDevice);
                if (FAILED(hr)) {
                    continue;
                }

                hr = pDevice->get_Name(&deviceName);
                if (FAILED(hr)){
                    continue;
                }

                hr = pDevice->get_VMID(&deviceVMID);
                if (FAILED(hr)){
                    continue;
                }

                if (deviceIdentifier == deviceName || deviceIdentifier == deviceVMID){
                    *pDeviceVMID = pDevice;
                    (*pDeviceVMID)->AddRef();
                    return TRUE;
                }
            }
        }
    }

    wprintf(L"Error: Unable to locate the device '%s'", deviceIdentifier);
    return FALSE;
}

bool emuConnect(const CComBSTR& deviceIdentifier)
{
	CComPtr<IDeviceEmulatorManagerVMID> pDevice = NULL;

    BOOL bFound = FindDevice(deviceIdentifier, &pDevice);
    if (bFound && pDevice){
        HRESULT hr = pDevice->Connect();
        if (!SUCCEEDED(hr)) {
            wprintf(L"Error: Operation Connect failed. Hr=0x%x\n", hr);
            return false;
        }
        return true;
    }
    return false;
}

bool emuBringToFront(const CComBSTR& deviceIdentifier)
{
    CComPtr<IDeviceEmulatorManagerVMID> pDevice = NULL;

    BOOL bFound = FindDevice(deviceIdentifier, &pDevice);
    if (bFound && pDevice){
        HRESULT hr = pDevice->BringToFront();
        if (!SUCCEEDED(hr)) {
            wprintf(L"Error: Operation BringToFront failed. Hr=0x%x\n", hr);
            return false;
        }
        return true;
    } 		
    return false;
}

bool emuCradle(const CComBSTR& deviceIdentifier)
{
    CComPtr<IDeviceEmulatorManagerVMID> pDevice = NULL;

    BOOL bFound = FindDevice(deviceIdentifier, &pDevice);
    if (bFound && pDevice){
        HRESULT hr = pDevice->Cradle();
        if (!SUCCEEDED(hr)) {
            wprintf(L"Error: Operation Cradle failed. Hr=0x%x\n", hr);
            return false;
        }
        return true;
    }
    return false;
}

bool emuUncradle(const CComBSTR& deviceIdentifier)
{
    CComPtr<IDeviceEmulatorManagerVMID> pDevice = NULL;

    BOOL bFound = FindDevice(deviceIdentifier, &pDevice);

    if (bFound && pDevice){
        HRESULT hr = pDevice->UnCradle();
        if (!SUCCEEDED(hr)) {
            wprintf(L"Error: Operation UnCradle failed. Hr=0x%x\n", hr);
            return false;
        }
        return true;
    }

    return false;
}

bool emuReset(const CComBSTR& deviceIdentifier)
{
    CComPtr<IDeviceEmulatorManagerVMID> pDevice = NULL;

    BOOL bFound = FindDevice(deviceIdentifier, &pDevice);
    if (bFound && pDevice){
        HRESULT hr = pDevice->Reset(TRUE);
        if (!SUCCEEDED(hr)) {
            wprintf(L"Error: Operation ResetEmulator failed. Hr=0x%x\n", hr);
            return false;
        }
        return true;
    }
    return false;
}

bool emuShutdown(const CComBSTR& deviceIdentifier)
{
    CComPtr<IDeviceEmulatorManagerVMID> pDevice = NULL;

    BOOL bFound = FindDevice(deviceIdentifier, &pDevice);
    if (bFound && pDevice){
        HRESULT hr = pDevice->Shutdown(FALSE);
        if (!SUCCEEDED(hr)) {
            wprintf(L"Error: Operation Shutdown failed. Hr=0x%x\n", hr);
            return false;
        }
        return true;
    }
    return false;
}

bool wceConnect (void) 
{
	HRESULT hRapiResult;

	//_tprintf( TEXT("Connecting to Windows CE..."));
	hRapiResult = CeRapiInit();
	if (FAILED(hRapiResult)) {
        _tprintf( TEXT("Failed\n"));
        return false;
    }
    //_tprintf( TEXT("Success\n"));

	return true;

	/*
	RAPIINIT riCopy = {sizeof(RAPIINIT), 0, 0};
    bool fInitialized = false;
	DWORD dwTimeOut = 5000;

    CeRapiUninit();

    hRapiResult = CeRapiInitEx(&riCopy);

	if (FAILED(hRapiResult)) {
		return false;
	}

    DWORD dwRapiInit = 0;

    dwRapiInit = WaitForSingleObject(
		riCopy.heRapiInit,
		dwTimeOut);

    if (WAIT_OBJECT_0 == dwRapiInit) {
        // heRapiInit signaled:
        // set return error code to return value of RAPI Init function
        hRapiResult = riCopy.hrRapiInit;  
    } else if (WAIT_TIMEOUT == dwRapiInit) {
        // timed out: device is probably not connected
        // or not responding
        hRapiResult = HRESULT_FROM_WIN32(ERROR_TIMEOUT);
    } else {
        // WaitForSingleObject failed
        hRapiResult = HRESULT_FROM_WIN32(GetLastError());
    }

	if (FAILED(hRapiResult)) {
        CeRapiUninit();
		return false;
	}
	return true;
	*/
}

void wceDisconnect(void)
{
    //_tprintf( TEXT("Closing connection to Windows CE..."));
    CeRapiUninit();
    //_tprintf( TEXT("Done\n"));
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
    if (rapi::wceConnect()) {
        if (!CeCreateProcess(wszProgram, wszArgs, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi)) {
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

} //end of rapi