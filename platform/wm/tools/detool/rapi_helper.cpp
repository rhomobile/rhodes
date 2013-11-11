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


} //end of rapi