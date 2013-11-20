#include "StdAfx.h"
#include "DeployDevWebKitCommand.h"

extern TCHAR *app_name;
extern void getAppDir(TCHAR **app_dir);

bool DeployDevWebKitCommand::checkParameters() const
{
    USES_CONVERSION;
    return (m_vecArgv.size() == 5) && (strcmp(T2A(m_vecArgv[2]), "wk-dev") == 0);
}

void DeployDevWebKitCommand::parseParameters()
{
    src_path = m_vecArgv[3];
    app_name = m_vecArgv[4];
}

void DeployDevWebKitCommand::runObject()
{    
    int new_copy = 0;
    HANDLE hFind;
    CE_FIND_DATA findData;

    TCHAR *app_dir = new TCHAR[MAX_PATH];
    getAppDir(&app_dir);

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

    _tprintf( TEXT("Load file to device..."));
    USES_CONVERSION;
    if (file::copyBundle(src_path, _T("/"), app_dir) == EXIT_FAILURE) {
        printf ("Failed to copy bundle\n");
        goto stop_emu_deploy;
    }

    _tprintf( TEXT("DONE\n"));

    return;

stop_emu_deploy:
    releaseDeploy();
}