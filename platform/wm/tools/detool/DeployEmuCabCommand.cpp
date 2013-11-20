#include "StdAfx.h"
#include "DeployEmuCabCommand.h"

extern TCHAR   *app_name;
extern void     getAppDir(TCHAR **app_dir);
extern EAppType getAppTypeFromCmdLine(int argc, TCHAR** argv);

bool DeployEmuCabCommand::checkParameters() const
{
    USES_CONVERSION;
    return (m_vecArgv.size() > 6) && (strcmp(T2A(m_vecArgv[2]), "emucab") == 0);
}

void DeployEmuCabCommand::parseParameters()
{
    USES_CONVERSION;

    emu_name = m_vecArgv[3];
    cab_file = m_vecArgv[4];
    app_name = m_vecArgv[5];

    if (strcmp(T2A(m_vecArgv[6]), "1") == 0)
        use_shared_runtime = true;
}

void DeployEmuCabCommand::runObject()
{
    USES_CONVERSION;

    EAppType app_type = getAppTypeFromCmdLine();
    int new_copy = 0;
    TCHAR params_buf[MAX_PATH + 16];

    TCHAR *app_dir = new TCHAR[MAX_PATH];
    getAppDir(&app_dir);

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

    // start Windows Mobile Device Center for network connectivity of the device emulator (if applicable)
    wmdc::startWMDC();

    _tprintf( TEXT("Loading cab file..."));		
    if (!rapi::wcePutFile (T2A(cab_file), "")) {
        _tprintf( TEXT("FAILED\n"));
        goto stop_emu_deploy;
    }
    _tprintf( TEXT("DONE\n"));

    _tprintf( TEXT("Loading utility dll..."));
    if (!rapi::wcePutFile (RHOSETUP_DLL, "")) {
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

    if(!rapi::wceInvokeCabSetup(T2A(params_buf))) {
        _tprintf( TEXT("FAILED\n"));
        goto stop_emu_deploy;
    }
    _tprintf( TEXT("DONE\n"));

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
    } else {
        _tcscpy(params_buf, TEXT("\\Program Files\\"));
        _tcscat(params_buf, app_name);
        _tcscat(params_buf, _T("\\"));
        _tcscat(params_buf, app_name);
        _tcscat(params_buf, _T(".exe"));
    }
    //_tcscat(params, _T("-log="));
    //_tcscat(params, log_port);
    _tprintf( TEXT("%s %s\n"), params_buf, params);

    if(!rapi::wceRunProcess(T2A(params_buf), T2A(params))) {
        _tprintf( TEXT("FAILED\n"));
        goto stop_emu_deploy;
    }
    _tprintf( TEXT("DONE\n"));

    CoUninitialize();
    ExitProcess(EXIT_SUCCESS);
    return;

stop_emu_deploy:
    releaseDeploy();
}