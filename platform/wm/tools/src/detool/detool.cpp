#include "stdafx.h"

#include <rapi.h>
#include <strsafe.h>

#include "detool.h"

DWORD WINAPI startDEM(LPVOID lpParam)
{
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
        HRESULT hr;

		CComPtr<IDeviceEmulatorManager> pDeviceEmulatorManager;
		hr = pDeviceEmulatorManager.CoCreateInstance(__uuidof(DeviceEmulatorManager));
		if (FAILED(hr)) {
			wprintf_s(L"Error: Unable to instantiate DeviceEmulatorManager. ErrorCode=0x%08X\n", hr);
			return false;
		}
		
		pDeviceEmulatorManager->ShowManagerUI(false);
		while (1) {
			Sleep(300 * 1000);
		}
		return true;
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
}

void wceDisconnect(void)
{
    //_tprintf( TEXT("Closing connection to Windows CE..."));
    CeRapiUninit();
    //_tprintf( TEXT("Done\n"));
}

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

bool wcePutFile(const char *host_file, const char *wce_file)
{
	TCHAR tszSrcFile[MAX_PATH];
	WCHAR wszDestFile[MAX_PATH];
	BYTE  buffer[4096];
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
	
	if (wceConnect()) {
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
	wceDisconnect();
	return true;

FatalError:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);
	wceDisconnect();
	return false;
}

bool wceRunProcess(const char *process, const char *args)
{
	HRESULT hRapiResult;
	HRESULT hr;
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
	if (wceConnect()) {
		if (!CeCreateProcess(wszProgram, wszArgs, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi)) {
			_tprintf( TEXT("CreateProcess failed with Errorcode = %ld\n"), CeGetLastError());
			return false;
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
		CeCloseHandle( pi.hProcess);
		CeCloseHandle( pi.hThread);
	}
	wceDisconnect();
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

	wceConnect();

	DWORD dwInSize = sizeof(wszCabFile);
	DWORD dwOutSize = 0;
	BYTE *pInBuff = NULL;
	
	pInBuff = (BYTE *)LocalAlloc(LPTR, dwInSize);
	memcpy(pInBuff, &wszCabFile, dwInSize);

	hr = CeRapiInvoke(TEXT("\\rhosetup"), TEXT("rhoCabSetup"), dwInSize, pInBuff, &dwOutSize, NULL, NULL, 0);
	if(FAILED(hr)) {
		printf("Failed to setup cab!\r\n");
		return false;
	}
	wceDisconnect();

	return true ;
}


#define EMU "Windows Mobile 6 Professional Emulator"
//#define EMU "USA Windows Mobile 6.5 Professional Portrait QVGA Emulator"

/**
 * detool emu "<emu_name|vmid>" app.cab "app-name"
 * or
 * detool dev app.cab "app-name"
 */
void usage(void)
{
}
enum {
	DEPLOY_EMU,
	DEPLOY_DEV
};

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR *emu_name = NULL;
	TCHAR *cab_file = NULL;
	TCHAR *app_name = NULL;
	TCHAR params_buf[MAX_PATH + 16];
	int deploy_type;

	if (argc == 5) {        //assuming that need to start emulator
		emu_name = argv[2];
		cab_file = argv[3];
		app_name = argv[4];
		deploy_type = DEPLOY_EMU;
	} else if (argc == 4) { //assuming that need to deploy and start on device
		cab_file = argv[2];
		app_name = argv[3];
		deploy_type = DEPLOY_DEV;
	} else {
		return EXIT_FAILURE;
	}

	if (deploy_type == DEPLOY_EMU) {
		if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
			CreateThread(NULL, 0, startDEM, NULL, 0, NULL);

			_tprintf( TEXT("Starting emulator... "));
			if (!emuConnect (emu_name)) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));
			
			_tprintf( TEXT("Cradle emulator... "));
			if(!emuCradle (emu_name)) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));
			
			_tprintf( TEXT("Loading cab file..."));
			
			USES_CONVERSION;
			if (!wcePutFile (T2A(cab_file), "")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));

			_tprintf( TEXT("Loading utility dll..."));
			if (!wcePutFile ("rhosetup.dll", "")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));
			
			_tprintf( TEXT("Setup application..."));
			_tcscpy(params_buf, TEXT("/noui "));
			_tcscat(params_buf, cab_file);
			if(!wceInvokeCabSetup("/noui \\rhodes.cab")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));

			_tprintf( TEXT("Starting application..."));
			_tcscpy(params_buf, TEXT("\\Program Files\\"));
			_tcscat(params_buf, app_name);
			_tcscat(params_buf, TEXT("\\rhodes.exe"));
			if(!wceRunProcess (T2A(params_buf), NULL)) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));


			CoUninitialize();

			ExitProcess(EXIT_SUCCESS);
	stop_emu_deploy:
			ExitProcess(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}
