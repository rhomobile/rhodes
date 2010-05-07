#include "stdafx.h"

#include <rapi.h>
#include <strsafe.h>

#include "detool.h"

#define RHOSETUP_DLL "rhosetup.dll"


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
			Sleep(600 * 1000);
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
		//printf("Failed to setup cab!\r\n");
		return false;
	}
	wceDisconnect();

	return true ;
}


#define EMU "Windows Mobile 6 Professional Emulator"
//#define EMU "USA Windows Mobile 6.5 Professional Portrait QVGA Emulator"

/**
 * detool emu "<emu_name|vmid>" "app-name" rhobundle_path exe_name
 * or
 * detool emucab "<emu_name|vmid>" app.cab "app-name"
 * or
   detool dev "app-name" rhobundle_path exe_name
   or 
 * detool devcab app.cab "app-name"
 */
void usage(void)
{
printf 
	("Rhodes deployment tool for Windows Mobile:          \n  \
	  detool emu \"<emu_name|vmid>\" app.cab \"app-name\" \n  \
	  or                                                  \n  \
	  detool dev app.cab \"app-name\"                     \n"
	 );
}
enum {
	DEPLOY_EMUCAB,
	DEPLOY_DEVCAB,
	DEPLOY_EMU,
	DEPLOY_DEV
};

int copyExecutable (TCHAR *file_name, TCHAR *app_dir)
{
	TCHAR exe_fullpath[MAX_PATH];
	int retval = 0;
	HANDLE hDest, hSrc;
	BYTE  buffer[5120];
	DWORD dwNumRead, dwNumWritten;

	USES_CONVERSION;
	
	_tcscpy(exe_fullpath, app_dir);
	_tcscat(exe_fullpath, _T("\\"));
	_tcscat(exe_fullpath, _T("rhodes.exe"));


	hSrc = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hSrc) {
		_tprintf( TEXT("Unable to open host file\n"));
		return EXIT_FAILURE;
	}

	hDest = CeCreateFile(exe_fullpath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hDest ) {
		_tprintf( TEXT("Unable to open target WinCE file\n"));
		return CeGetLastError();
	}

	do {
		if(ReadFile(hSrc, &buffer, sizeof(buffer), &dwNumRead, NULL)) {
			if (!CeWriteFile(hDest, &buffer, dwNumRead, &dwNumWritten, NULL)) {
				_tprintf( TEXT("Error !!! Writing WinCE file\n"));
				goto copyFailure;
				}
			} else {
				_tprintf( TEXT("Error !!! Reading host file\n"));
				goto copyFailure;
		}
		_tprintf( TEXT("."));                                        
	} while (dwNumRead);
	_tprintf( TEXT("\n"));

	CeCloseHandle( hDest);
	CloseHandle (hSrc);

	return EXIT_SUCCESS;

copyFailure:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);
/*

	if (wcePutFile (T2A(file_name), T2A(exe_fullpath)))
		return EXIT_SUCCESS;
*/
	return EXIT_FAILURE;
}

int copyBundle (TCHAR *parent_dir, TCHAR *file, TCHAR *app_dir)
{
	HANDLE           fileHandle;
	WIN32_FIND_DATAW findData;
	DWORD			 dwError;
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

	//wceConnect ();

	fileHandle = FindFirstFile(wildcard, &findData);

	if (fileHandle == INVALID_HANDLE_VALUE) {
		printf ("Failed to open file\n");
		wceDisconnect();
		return EXIT_FAILURE;
	}

	/*
	if (_tcscmp (_T("."), findData.cFileName) != 0 && _tcscmp (_T(".."), findData.cFileName) != 0) {
		printf("-- %s\n", T2A(findData.cFileName));
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			copyBundle (fullpath, findData.cFileName, NULL);
		}
	}
	*/

	HANDLE hDest, hSrc;
	BYTE  buffer[5120];
	DWORD dwNumRead, dwNumWritten;

	while (FindNextFile(fileHandle, &findData)) {
		if (_tcscmp (_T("."), findData.cFileName) == 0 || _tcscmp (_T(".."), findData.cFileName) == 0)
			continue;

		_tcscpy(new_app_item, app_dir);
		_tcscat(new_app_item, _T("\\"));
		_tcscat(new_app_item, findData.cFileName);

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{	
			//Check and create dir on device
			hFind = CeFindFirstFile(new_app_item, &wceFindData);
			if (INVALID_HANDLE_VALUE == hFind) {
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

			hSrc = CreateFile(host_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hSrc) {
				_tprintf( TEXT("Unable to open host file\n"));
				return false;
			}

			hDest = CeCreateFile(new_app_item, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hDest ) {
				_tprintf( TEXT("Unable to open target WinCE file\n"));
				return false;
			}

			do {
				if(ReadFile(hSrc, &buffer, sizeof(buffer), &dwNumRead, NULL)) {
					if (!CeWriteFile(hDest, &buffer, dwNumRead, &dwNumWritten, NULL)) {
						_tprintf( TEXT("Error !!! Writing WinCE file\n"));
						goto copyBundleFailure;
					}
				} else {
					_tprintf( TEXT("Error !!! Reading host file\n"));
					goto copyBundleFailure;
				}
				_tprintf( TEXT("."));                                        
			} while (dwNumRead);
			_tprintf( TEXT("\n"));

			CeCloseHandle( hDest);
			CloseHandle (hSrc);
			/*
			if(!wcePutFile (T2A(host_file), T2A(new_app_item))) {
				printf("Failed to copy file.");
				return EXIT_FAILURE;
			}
			*/
		}

	}

	return EXIT_SUCCESS;

copyBundleFailure:
	CeCloseHandle( hDest);
	CloseHandle (hSrc);
	return EXIT_FAILURE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR *emu_name = NULL;
	TCHAR *cab_file = NULL;
	TCHAR *app_name = NULL;
	TCHAR *bundle_path = NULL;
	TCHAR *app_exe = NULL;
	TCHAR params_buf[MAX_PATH + 16];
	WIN32_FIND_DATAW findData;
	int new_copy = 0;
	int deploy_type;

	USES_CONVERSION;

	if (argc >= 5) {        //assuming that need to start emulator
		if (strcmp(T2A(argv[1]), "emu") == 0) {
			emu_name    = argv[2];
			app_name    = argv[3];
			bundle_path = argv[4];
			app_exe     = argv[5];
			deploy_type = DEPLOY_EMU;
		}
		if (strcmp(T2A(argv[1]), "emucab") == 0) {
			emu_name = argv[2];
			cab_file = argv[3];
			app_name = argv[4];
			deploy_type = DEPLOY_EMUCAB;
		}

		if (strcmp(T2A(argv[1]), "dev") == 0) {
			app_name = argv[2];
			bundle_path = argv[3];
			app_exe = argv[4];
			deploy_type = DEPLOY_DEV;
		}
	} else if (argc == 4) { //assuming that need to deploy and start on device
		cab_file = argv[2];
		app_name = argv[3];
		deploy_type = DEPLOY_DEVCAB;
	} else {
		usage();
		return EXIT_FAILURE;
	}

	TCHAR app_dir[MAX_PATH];
	_tcscpy(app_dir, TEXT("\\Program Files\\"));
	_tcscat(app_dir, app_name);
	_tprintf( TEXT("%s\n"), app_dir);

	if (deploy_type == DEPLOY_EMU) {
		if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
			HANDLE hFind;
			CE_FIND_DATA findData;

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
	
			if (!wceConnect ()) {
				printf ("Failed to connect to remote device.\n");
				goto stop_emu_deploy;
			} else {

				hFind = CeFindFirstFile(app_dir, &findData);
				if (INVALID_HANDLE_VALUE == hFind) {
					_tprintf( TEXT("Application directory on device was no found\n"));
					
					new_copy = 1;

					if (!CeCreateDirectory(app_dir, NULL)) {
						printf ("Failed to create app directory\n");
						goto stop_emu_deploy;
					}
				}
				FindClose( hFind);

				if (!findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					_tprintf( TEXT("Error: target directory is file\n"));
					goto stop_emu_deploy;
				}
				

				TCHAR remote_bundle_path[MAX_PATH];

				_tcscpy(remote_bundle_path, app_dir);
				_tcscat(remote_bundle_path, _T("\\rho"));

				hFind = CeFindFirstFile(remote_bundle_path, &findData);
				if (INVALID_HANDLE_VALUE == hFind) {
					_tprintf( TEXT("Bundle directory on device was no found\n"));
	
					if (!CeCreateDirectory(remote_bundle_path, NULL)) {
						printf ("Failed to create bundle directory\n");
						goto stop_emu_deploy;
					}
				}
				FindClose( hFind);

				int retval = copyExecutable (app_exe, app_dir);
				if (retval != EXIT_SUCCESS) {
					printf ("Failed to copy application executable\n");
					if (retval == 32) {
						printf ("Please, stop application on device and try again.\n");
					}
					goto stop_emu_deploy;
				}

				if (copyBundle(bundle_path, _T("/"), remote_bundle_path) == EXIT_FAILURE) {
					printf ("Failed to copy bundle\n");
					goto stop_emu_deploy;
				}

				emuBringToFront(emu_name);

				_tprintf( TEXT("Starting application..."));
				_tcscpy(params_buf, TEXT("\\Program Files\\"));
				_tcscat(params_buf, app_name);
				_tcscat(params_buf, TEXT("\\rhodes.exe"));
				if(!wceRunProcess(T2A(params_buf), NULL)) {
					_tprintf( TEXT("FAILED\n"));
					goto stop_emu_deploy;
				}
				_tprintf( TEXT("DONE\n"));

				wceDisconnect();
			}
	
			CoUninitialize();
	
			ExitProcess(EXIT_SUCCESS);
		}
	}
	if (deploy_type == DEPLOY_EMUCAB) {
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
			if (!wcePutFile (T2A(cab_file), "")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));

			_tprintf( TEXT("Loading utility dll..."));
			if (!wcePutFile (RHOSETUP_DLL, "")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));
			
			_tprintf( TEXT("Setup application..."));

			//FIXME: rake gives pathname with unix-like '/' file separators,
			//so if we want to use this tool outside of rake, we should remember this
			//or check and convert cab_file
			TCHAR *p = _tcsrchr (cab_file, '/');
			if (p) p++;
			_tcscpy(params_buf, TEXT("/noui "));
			_tcscat(params_buf, p != NULL ? p : cab_file);

			if(!wceInvokeCabSetup(T2A(params_buf))) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));

			emuBringToFront(emu_name);

			_tprintf( TEXT("Starting application..."));
			_tcscpy(params_buf, TEXT("\\Program Files\\"));
			_tcscat(params_buf, app_name);
			_tcscat(params_buf, TEXT("\\rhodes.exe"));
			if(!wceRunProcess(T2A(params_buf), NULL)) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));


			CoUninitialize();
			ExitProcess(EXIT_SUCCESS);
	stop_emu_deploy:
			CoUninitialize();
			ExitProcess(EXIT_FAILURE);
		}
	}

	if (deploy_type == DEPLOY_DEV) {
		HANDLE hFind;
		CE_FIND_DATA findData;

		_tprintf( TEXT("Searching for Windows CE device..."));

		HRESULT hRapiResult;
		hRapiResult = CeRapiInit();
		if (FAILED(hRapiResult)) {
			_tprintf( TEXT("FAILED\n"));
			return false;
		}
		_tprintf( TEXT("DONE\n"));

		hFind = CeFindFirstFile(app_dir, &findData);
		if (INVALID_HANDLE_VALUE == hFind) {
			_tprintf( TEXT("Application directory on device was no found\n"));
					
			new_copy = 1;

			if (!CeCreateDirectory(app_dir, NULL)) {
				printf ("Failed to create app directory\n");
				goto stop_emu_deploy;
			}
		}
		FindClose( hFind);

		if (!findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			_tprintf( TEXT("Error: target directory is file\n"));
			goto stop_emu_deploy;
		}
				

		TCHAR remote_bundle_path[MAX_PATH];

		_tcscpy(remote_bundle_path, app_dir);
		_tcscat(remote_bundle_path, _T("\\rho"));

		hFind = CeFindFirstFile(remote_bundle_path, &findData);
		if (INVALID_HANDLE_VALUE == hFind) {
			_tprintf( TEXT("Bundle directory on device was no found\n"));
	
			if (!CeCreateDirectory(remote_bundle_path, NULL)) {
				printf ("Failed to create bundle directory\n");
				goto stop_emu_deploy;
			}
		}
		FindClose( hFind);

		
		int retval = copyExecutable (app_exe, app_dir);
		if (retval != EXIT_SUCCESS) {
			printf ("Failed to copy application executable\n");
			if (retval == 32) {
				printf ("Please, stop application on device and try again.\n");
			}
			goto stop_emu_deploy;
		}

		if (copyBundle(bundle_path, _T("/"), remote_bundle_path) == EXIT_FAILURE) {
			printf ("Failed to copy bundle\n");
			goto stop_emu_deploy;
		}

		Sleep(2 * 1000);

		_tprintf( TEXT("Starting application..."));
		_tcscpy(params_buf, TEXT("\\Program Files\\"));
		_tcscat(params_buf, app_name);
		_tcscat(params_buf, TEXT("\\rhodes.exe"));
		_tprintf( TEXT("%s\n"), params_buf);

		
		if(!wceRunProcess(T2A(params_buf), NULL)) {
			_tprintf( TEXT("FAILED\n"));
			goto stop_emu_deploy;
		}
		_tprintf( TEXT("DONE\n"));

		ExitProcess(EXIT_SUCCESS);
	}

	if (deploy_type == DEPLOY_DEVCAB) {
			_tprintf( TEXT("Searching for Windows CE device..."));

			HRESULT hRapiResult;
			hRapiResult = CeRapiInit();
			if (FAILED(hRapiResult)) {
					_tprintf( TEXT("FAILED\n"));
					return false;
			}
			_tprintf( TEXT("DONE\n"));

			_tprintf( TEXT("Loading cab file to device..."));
			USES_CONVERSION;
			if (!wcePutFile (T2A(cab_file), "")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));

			_tprintf( TEXT("Loading utility dll..."));
			if (!wcePutFile (RHOSETUP_DLL, "")) {
				_tprintf( TEXT("FAILED\n"));
				goto stop_emu_deploy;
			}
			_tprintf( TEXT("DONE\n"));
			
			_tprintf( TEXT("Setup application..."));

			//FIXME: rake gives pathname with unix-like '/' file separators,
			//so if we want to use this tool outside of rake, we should remember this
			//or check and convert cab_file
			TCHAR *p = _tcsrchr (cab_file, '/');
			if (p) p++;
			_tcscpy(params_buf, p != NULL ? p : cab_file);
			//_tcscat(params_buf, p != NULL ? p : cab_file);

			if(!wceInvokeCabSetup(T2A(params_buf))) {
				_tprintf( TEXT("FAILED\n"));

				_tprintf( TEXT("Starting installator GUI ..."));
				if(!wceRunProcess ("\\windows\\wceload.exe", T2A(p != NULL ? p : cab_file))) {
					_tprintf( TEXT("FAILED\n"));
					ExitProcess(EXIT_FAILURE);
				} else {
					_tprintf( TEXT("DONE\n"));
					_tprintf( TEXT("Please continue manually...\n"));
					ExitProcess(EXIT_SUCCESS);
				}

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
	}

	return EXIT_SUCCESS;
}
