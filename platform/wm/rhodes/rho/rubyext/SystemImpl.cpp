#include "stdafx.h"

#include "common/RhoPort.h"
#include "common/StringConverter.h"
#include "ruby/ext/rho/rhoruby.h"
#include "MainWindow.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

static int has_camera()
{
#ifdef OS_WINCE
    DEVMGR_DEVICE_INFORMATION devInfo = {0};
    GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93,
                        0x3E, 0x4D, 0x7E, 0x3C, 0x86 };
    devInfo.dwSize = sizeof(devInfo);

    HANDLE hDevice = FindFirstDevice( DeviceSearchByGuid, &guidCamera, &devInfo);
    if ( hDevice != INVALID_HANDLE_VALUE )
    {
        FindClose(hDevice);
        return 1;
    }

    return 0;
#else
    return 0;
#endif
}

VALUE rho_sysimpl_get_property(char* szPropName)
{
	if (strcasecmp("has_camera",szPropName) == 0) 
        return rho_ruby_create_boolean(has_camera());

    return rho_ruby_get_NIL();
}

VALUE rho_sys_get_locale()
{
    wchar_t szLang[20];
    int nRes = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SABBREVLANGNAME , szLang, 20);
    szLang[2] = 0;
    wcslwr(szLang);

    return rho_ruby_create_string(convertToStringA(szLang).c_str());
}

int rho_sys_get_screen_width()
{
#ifdef _WIN32_WCE
	return GetSystemMetrics(SM_CXSCREEN);
#else
	return CMainWindow::getScreenWidth();
#endif
}

int rho_sys_get_screen_height()
{
#ifdef _WIN32_WCE
	return GetSystemMetrics(SM_CYSCREEN);
#else
	return CMainWindow::getScreenHeight();
#endif
}

VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values) 
{
	return rho_ruby_get_NIL();
}

static int g_rho_has_network = 1;

void rho_sysimpl_sethas_network(int nValue)
{
    g_rho_has_network = nValue;
}

VALUE rho_sys_has_network()
{
	return rho_ruby_create_boolean(g_rho_has_network!=0);
}

}