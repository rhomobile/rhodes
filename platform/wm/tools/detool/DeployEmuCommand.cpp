#include "stdafx.h"
#include "DeployEmuCommand.h"

extern TCHAR *app_name;
extern void getAppDir(TCHAR **app_dir);
extern bool str_ends_with(const TCHAR* str, const TCHAR* suffix);

bool DeployEmuCommand::checkParameters() const
{
    USES_CONVERSION;
    return (m_vecArgv.size() > 7) && (strcmp(T2A(m_vecArgv[2]), "emu") == 0);
}

void DeployEmuCommand::parseParameters()
{
    exe_change  = _tcscmp(m_vecArgv[3], L"1") == 0 ? true : false;
    emu_name    = m_vecArgv[4];
    app_name    = m_vecArgv[5];
    bundle_path = m_vecArgv[6];
    app_exe     = m_vecArgv[7];
    log_port    = m_vecArgv[8];
    lcdll_path  = m_vecArgv[9];

    if ((!use_shared_runtime) && app_exe)
        use_shared_runtime = str_ends_with(app_exe, L".lnk");
}

void DeployEmuCommand::runObject()
{
    EAppType app_type = getAppTypeFromCmdLine();
    int new_copy = 0;
    TCHAR params_buf[MAX_PATH + 16];

    TCHAR *app_dir = new TCHAR[MAX_PATH];
    getAppDir(&app_dir);

    USES_CONVERSION;

    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
        HANDLE hFind;
        CE_FIND_DATA findData;

        CreateThread(NULL, 0, rapi::startDEM, NULL, 0, NULL);

        _tprintf( TEXT("Starting emulator... "));
        if (!rapi::emuConnect (emu_name)) {
            _tprintf( TEXT("FAILED\n"));
            releaseDeploy();
        }
        _tprintf( TEXT("DONE\n"));

        _tprintf( TEXT("Cradle emulator... "));
        if(!rapi::emuCradle (emu_name)) {
            _tprintf( TEXT("FAILED\n"));
            releaseDeploy();
        }
        _tprintf( TEXT("DONE\n"));

        if (!rapi::wceConnect ()) {
            printf ("Failed to connect to remote device.\n");
            releaseDeploy();
        } 
        else 
        {
            // start Windows Mobile Device Center for network connectivity of the device emulator (if applicable)
            wmdc::startWMDC();

            hFind = CeFindFirstFile(app_dir, &findData);
            if (INVALID_HANDLE_VALUE == hFind) {
                _tprintf( TEXT("Application directory on device was not found\n"));

                new_copy = 1;

                if (!CeCreateDirectory(app_dir, NULL)) {
                    printf ("Failed to create app directory\n");
                    releaseDeploy();
                }
            }
            FindClose( hFind);

            if ((!new_copy) && (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))) {
                _tprintf( TEXT("Error: target directory is file\n"));
                releaseDeploy();
            }

            TCHAR remote_bundle_path[MAX_PATH];

            _tcscpy(remote_bundle_path, app_dir);
            _tcscat(remote_bundle_path, _T("\\rho"));

            hFind = CeFindFirstFile(remote_bundle_path, &findData);
            if (INVALID_HANDLE_VALUE == hFind) {
                _tprintf( TEXT("Bundle directory on device was not found\n"));

                if (!CeCreateDirectory(remote_bundle_path, NULL)) {
                    printf ("Failed to create bundle directory\n");
                    releaseDeploy();
                }
            }
            FindClose( hFind);

            int retval = EXIT_SUCCESS;

            retval = file::copyExecutable (app_exe, app_dir, exe_change, use_shared_runtime);
            if (retval != EXIT_SUCCESS) {
                printf ("Failed to copy application executable\n");
                if (retval == 32) {
                    printf ("Please, stop application on device and try again.\n");
                }
                releaseDeploy();
            }

            if (lcdll_path) {
                if(!use_shared_runtime)
                    retval = file::copyLicenseDll(lcdll_path, app_dir);
                if (retval != EXIT_SUCCESS) {
                    printf ("Failed to copy license dll\n");
                    if (retval == 32) {
                        printf ("Please, stop application on device and try again.\n");
                    }
                    releaseDeploy();
                }
            }

            if (file::copyBundle(bundle_path, _T("/"), remote_bundle_path) == EXIT_FAILURE) {
                printf ("Failed to copy bundle\n");
                releaseDeploy();
            }

            // establish network connectivity of the device emulator from Windows Mobile Device Center (if applicable)
            wmdc::connectWMDC();

            rapi::emuBringToFront(emu_name);

            _tprintf( TEXT("Starting application..."));
            TCHAR params[MAX_PATH];
            params[0] = 0;
            if (use_shared_runtime) {
                _tcscpy(params_buf, RE2_RUNTIME);
                if (app_type == eRuby) {
                    _tcscpy(params, _T("-approot='\\Program Files\\"));
                }
                else {
                    _tcscpy(params, _T("-jsapproot='\\Program Files\\"));
                }
                _tcscat(params, app_name);
                _tcscat(params, _T("'"));
            } 
            else 
            {
                _tcscpy(params_buf, TEXT("\\Program Files\\"));
                _tcscat(params_buf, app_name);
                _tcscat(params_buf, _T("\\"));
                _tcscat(params_buf, app_name);
                _tcscat(params_buf, _T(".exe"));
            }

            _tprintf( TEXT("%s %s\n"), params_buf, params);

            if(!rapi::wceRunProcess(T2A(params_buf), T2A(params))) {
                _tprintf( TEXT("FAILED\n"));
                releaseDeploy();
            }
            _tprintf( TEXT("DONE\n"));

            rapi::wceDisconnect();
        }

        ExitProcess(EXIT_SUCCESS);
    }
}