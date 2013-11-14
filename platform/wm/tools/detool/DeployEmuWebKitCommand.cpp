#include "StdAfx.h"
#include "DeployEmuWebKitCommand.h"

extern TCHAR *app_name;
extern void getAppDir(TCHAR **app_dir);
extern EAppType getAppTypeFromCmdLine(int argc, TCHAR** argv);

bool DeployEmuWebKitCommand::checkParameters() const
{
    USES_CONVERSION;
    return (m_vecArgv.size() == 6) && (strcmp(T2A(m_vecArgv[2]), "wk-emu") == 0);
}

void DeployEmuWebKitCommand::parseParameters()
{
    emu_name = m_vecArgv[3];
    src_path = m_vecArgv[4];
    app_name = m_vecArgv[5];     
}

void DeployEmuWebKitCommand::runObject()
{    
    int new_copy = 0;

    TCHAR *app_dir = new TCHAR[MAX_PATH];
    getAppDir(&app_dir);
    
    HANDLE hFind;
    CE_FIND_DATA findData;

    CreateThread(NULL, 0, rapi::startDEM, NULL, 0, NULL);

    _tprintf( TEXT("Starting emulator... "));
    if (!rapi::emuConnect (emu_name)) {
        _tprintf( TEXT("FAILED\n"));
        goto stop_emu_deploy;
    }
    _tprintf( TEXT("DONE\n"));

    _tprintf( TEXT("Cradle emulator... "));
    if(!rapi::emuCradle (emu_name)) {
        _tprintf( TEXT("FAILED\n"));
        goto stop_emu_deploy;
    }
    _tprintf( TEXT("DONE\n"));

    if (!rapi::wceConnect ()) {
        printf ("Failed to connect to remote device.\n");
        goto stop_emu_deploy;
    } else {
        hFind = CeFindFirstFile(app_dir, &findData);
        if (INVALID_HANDLE_VALUE == hFind) {
            _tprintf( TEXT("Application directory on device was not found\n"));

            new_copy = 1;

            if (!CeCreateDirectory(app_dir, NULL)) {
                printf ("Failed to create app directory\n");
                goto stop_emu_deploy;
            }
        }
        FindClose( hFind);

        if ((!new_copy) && (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))) {
            _tprintf( TEXT("Error: target directory is file\n"));
            goto stop_emu_deploy;
        }

        _tprintf( TEXT("Load files to device..."));
        USES_CONVERSION;
        if (file::copyBundle(src_path, _T("/"), app_dir) == EXIT_FAILURE) {
            printf ("Failed to copy bundle\n");
            goto stop_emu_deploy;
        }

        _tprintf( TEXT("DONE\n"));

        rapi::emuBringToFront(emu_name);
    }

    return;
stop_emu_deploy:
    releaseDeploy();
}