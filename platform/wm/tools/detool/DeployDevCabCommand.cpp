#include "StdAfx.h"
#include "DeployDevCabCommand.h"

extern TCHAR *app_name;

bool DeployDevCabCommand::checkParameters() const
{
    USES_CONVERSION;
    return (m_vecArgv.size() == 6) && (strcmp(T2A(m_vecArgv[2]), "wk-emu") != 0);
}

void DeployDevCabCommand::parseParameters()
{
    USES_CONVERSION;

    cab_file = m_vecArgv[3];
    app_name = m_vecArgv[4];

    if (strcmp(T2A(m_vecArgv[5]), "1") == 0)
        use_shared_runtime = true;
}

void DeployDevCabCommand::runObject()
{
    USES_CONVERSION;

	_tprintf( TEXT("Searching for Windows CE device..."));

    TCHAR params_buf[MAX_PATH + 16];
    EAppType app_type = getAppTypeFromCmdLine();

	_tprintf( TEXT("Loading cab file to device..."));
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
    TCHAR *noui = new TCHAR[100];
    _tcscpy(noui, L"/noui ");

	TCHAR *p1 = _tcsrchr (cab_file, '/');
    
    TCHAR *p = NULL;

    if (p1){
        p1++;
        p = _tcscat (noui, p1);
        _tcscpy(params_buf, p);
    }
    else {
        p = _tcscat (noui, cab_file);
        _tcscpy(params_buf, p);
    }
	
	//_tcscat(params_buf, p != NULL ? p : cab_file);

    if(!rapi::wceInvokeCabSetup(T2A(params_buf))) {
		_tprintf( TEXT("FAILED\n"));

		_tprintf( TEXT("Starting installator GUI ..."));
        if(!rapi::wceRunProcess ("\\windows\\wceload.exe", T2A(p))) {
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

	// establish network connectivity of the device from Windows Mobile Device Center (if applicable)
    wmdc::connectWMDC();

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

    if(!rapi::wceRunProcess (T2A(params_buf), T2A(params))) {
		_tprintf( TEXT("FAILED\n"));
		goto stop_emu_deploy;
	}
	_tprintf( TEXT("DONE\n"));

    return;

stop_emu_deploy:
    releaseDeploy();
}