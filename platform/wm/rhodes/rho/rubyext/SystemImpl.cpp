#include "stdafx.h"

#include "common/RhoPort.h"
#include "common/StringConverter.h"
#include "ruby/ext/rho/rhoruby.h"
#include "MainWindow.h"

//#ifdef OS_WINCE
//#include <cfgmgrapi.h>
//#endif

#ifdef OS_WINCE__
#include <tapi.h>
#include <tsp.h>
//#include <sms.h>
#endif

using namespace rho;
using namespace rho::common;
extern "C" HWND getMainWnd();
extern "C" char* wce_wctomb(const wchar_t* w);

extern "C"
{
#ifdef OS_WINCE__

static const int PHONE_NUMBER_BUFFER_SIZE = 512;

bool getPhoneNumFromSIMCard (String &number) 
{

#define EXIT_ON_NULL(_p) if (_p == NULL){ hr = E_OUTOFMEMORY; goto FuncExit; }
#define EXIT_ON_FALSE(_f) if (!(_f)) { hr = E_FAIL; goto FuncExit; }
#define MAX(i, j)   ((i) > (j) ? (i) : (j))

	const int TAPI_API_LOW_VERSION  = 0x00020000;
	const int TAPI_API_HIGH_VERSION = 0x00020000;
	const int LINE_NUMBER = 1;

    HRESULT  hr = E_FAIL;
    LRESULT  lResult = 0;
    HLINEAPP hLineApp;
    DWORD    dwNumDevs;
    DWORD    dwAPIVersion = TAPI_API_HIGH_VERSION;
    LINEINITIALIZEEXPARAMS liep;

    DWORD dwTAPILineDeviceID;
    const DWORD dwAddressID = LINE_NUMBER - 1;

    liep.dwTotalSize = sizeof(liep);
    liep.dwOptions   = LINEINITIALIZEEXOPTION_USEEVENT;

    if (SUCCEEDED(lineInitializeEx(&hLineApp, 0, 0, TEXT("ExTapi_Lib"), &dwNumDevs, &dwAPIVersion, &liep))) {
        BYTE* pCapBuf = NULL;
        DWORD dwCapBufSize = PHONE_NUMBER_BUFFER_SIZE;
        LINEEXTENSIONID  LineExtensionID;
        LINEDEVCAPS*     pLineDevCaps = NULL;
        LINEADDRESSCAPS* placAddressCaps = NULL;

        pCapBuf = new BYTE[dwCapBufSize];
        EXIT_ON_NULL(pCapBuf);

        pLineDevCaps = (LINEDEVCAPS*)pCapBuf;
        pLineDevCaps->dwTotalSize = dwCapBufSize;

        // Get TSP Line Device ID
        dwTAPILineDeviceID = 0xffffffff;
        for (DWORD dwCurrentDevID = 0 ; dwCurrentDevID < dwNumDevs ; dwCurrentDevID++) {
            if (0 == lineNegotiateAPIVersion(hLineApp, dwCurrentDevID, TAPI_API_LOW_VERSION, TAPI_API_HIGH_VERSION,
                &dwAPIVersion, &LineExtensionID)) {
                lResult = lineGetDevCaps(hLineApp, dwCurrentDevID, dwAPIVersion, 0, pLineDevCaps);

                if (dwCapBufSize < pLineDevCaps->dwNeededSize) {
                    delete[] pCapBuf;
                    dwCapBufSize = pLineDevCaps->dwNeededSize;
                    pCapBuf = new BYTE[dwCapBufSize];
                    EXIT_ON_NULL(pCapBuf);

                    pLineDevCaps = (LINEDEVCAPS*)pCapBuf;
                    pLineDevCaps->dwTotalSize = dwCapBufSize;

                    lResult = lineGetDevCaps(hLineApp, dwCurrentDevID, dwAPIVersion, 0, pLineDevCaps);
                }

                if ((0 == lResult) &&
                    (0 == _tcscmp((TCHAR*)((BYTE*)pLineDevCaps+pLineDevCaps->dwLineNameOffset), CELLTSP_LINENAME_STRING))) {
                    dwTAPILineDeviceID = dwCurrentDevID;
                    break;
                }
            }
        }

        placAddressCaps = (LINEADDRESSCAPS*)pCapBuf;
        placAddressCaps->dwTotalSize = dwCapBufSize;

        lResult = lineGetAddressCaps(hLineApp, dwTAPILineDeviceID, dwAddressID, dwAPIVersion, 0, placAddressCaps);

        if (dwCapBufSize < placAddressCaps->dwNeededSize) {
            delete[] pCapBuf;
            dwCapBufSize = placAddressCaps->dwNeededSize;
            pCapBuf = new BYTE[dwCapBufSize];
            EXIT_ON_NULL(pCapBuf);

            placAddressCaps = (LINEADDRESSCAPS*)pCapBuf;
            placAddressCaps->dwTotalSize = dwCapBufSize;

            lResult = lineGetAddressCaps(hLineApp, dwTAPILineDeviceID, dwAddressID, dwAPIVersion, 0, placAddressCaps);
        }

        if (0 == lResult) {
			EXIT_ON_FALSE(0 != placAddressCaps->dwAddressSize);

			// A non-zero dwAddressSize means a phone number was found
			ASSERT(0 != placAddressCaps->dwAddressOffset);    
			PWCHAR tsAddress = (WCHAR*)(((BYTE*)placAddressCaps)+placAddressCaps->dwAddressOffset);
			number = convertToStringA (tsAddress);

            hr = S_OK;
        }

        delete[] pCapBuf;
    } // End if ()

FuncExit:
    lineShutdown(hLineApp);
	
	if (hr != S_OK) {
		LOG(ERROR) + "failed to get phone number from SIM";
		return false;
	}

    return true;

#undef EXIT_ON_NULL
#undef EXIT_ON_FALSE 
#undef MAX

}
/*
bool getPhoneNumFromSMSBearer (String &number)
{
	SMS_ADDRESS psmsaAddress;
	
	if (SmsGetPhoneNumber (&psmsaAddress) != S_OK) {
		LOG(ERROR) + "failed to get phone number using SMS bearer";
		return false;
	}

	number = convertToStringA(psmsaAddress.ptsAddress);
	return true;
} */

bool getPhoneNumFromOwnerInfo (String &number)
{
	HKEY	hKey;
	DWORD	dwType, dwCount = PHONE_NUMBER_BUFFER_SIZE;
	TCHAR   strValue [PHONE_NUMBER_BUFFER_SIZE];
	LONG    res;
	TCHAR   errMsg[1024];

	if ((res = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("ControlPanel\\Owner"),  NULL, KEY_EXECUTE , &hKey)) == 0) 
	{
		if ((res = RegQueryValueEx (hKey, TEXT("Telephone"), NULL,  &dwType, (LPBYTE )strValue, &dwCount)) == 0) 
		{
			if (dwType != REG_SZ) 
			{
				LOG(ERROR) + "Settings/Owner Information/Telephone has invalid type";
				RegCloseKey(hKey);
				return false;
			}

			if (dwCount > 0) 
			{
				strValue[dwCount + 1] = '\0';

				if (_tcslen((strValue))  == 0) 
				{
					LOG(INFO) + "Settings/Owner Information/Telephone is empty";

					RegCloseKey(hKey);
					return false;
				}

				number = convertToStringA(strValue);

				RegCloseKey(hKey);
				return true;
			}
		}
	}

	RegCloseKey(hKey);
	FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, errMsg, sizeof(errMsg), NULL);
	LOG(ERROR) + errMsg;

	return false;
}

VALUE phone_number()
{
	String number;

	if (getPhoneNumFromSIMCard(number))
		return rho_ruby_create_string(number.c_str());
	
//	if (getPhoneNumFromSMSBearer(number))
//		return rho_ruby_create_string(number.c_str());

	if (getPhoneNumFromOwnerInfo(number))
		return rho_ruby_create_string(number.c_str());

	return rho_ruby_get_NIL();
}
#else
VALUE phone_number()
{
	return rho_ruby_get_NIL();
}
#endif

static int has_camera()
{
#ifdef OS_WINCE
/*    DEVMGR_DEVICE_INFORMATION devInfo = {0};
    GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93,
                        0x3E, 0x4D, 0x7E, 0x3C, 0x86 };
    devInfo.dwSize = sizeof(devInfo);

    HANDLE hDevice = FindFirstDevice( DeviceSearchByGuid, &guidCamera, &devInfo);
    if ( hDevice != INVALID_HANDLE_VALUE )
    {
        FindClose(hDevice);
        return 1;
    }

    return 0;*/

    return 1;
#else
    return 0;
#endif
}

static double get_screen_ppi_x()
{
	HWND hWndDesktop = GetDesktopWindow();
	HDC hdcDesktop = GetDC(hWndDesktop);
	int mms = GetDeviceCaps(hdcDesktop, HORZSIZE);
	int pixels = GetDeviceCaps(hdcDesktop, HORZRES);
	double ret = (pixels*25.4)/mms;
	return ret;
}

static double get_screen_ppi_y()
{
	HWND hWndDesktop = GetDesktopWindow();
	HDC hdcDesktop = GetDC(hWndDesktop);
	int mms = GetDeviceCaps(hdcDesktop, VERTSIZE);
	int pixels = GetDeviceCaps(hdcDesktop, VERTRES);
	double ret = (pixels*25.4)/mms;
	return ret;
}

VALUE rho_sys_get_locale()
{
	wchar_t szLang[20];
	int nRes = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SABBREVLANGNAME , szLang, sizeof(szLang)/sizeof(szLang[0]));
	szLang[2] = 0;
	wcslwr(szLang);

	return rho_ruby_create_string(convertToStringA(szLang).c_str());
}

int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
	if (strcasecmp("has_camera",szPropName) == 0) 
        {*resValue = rho_ruby_create_boolean(has_camera()); return 1;}

	if (strcasecmp("phone_number",szPropName) == 0)
        {*resValue = phone_number();return 1;}

	if (strcasecmp("ppi_x",szPropName) == 0)
        {*resValue = rho_ruby_create_double(get_screen_ppi_x()); return 1;}

	if (strcasecmp("ppi_y",szPropName) == 0)
        {*resValue = rho_ruby_create_double(get_screen_ppi_y()); return 1; }

	if (strcasecmp("locale",szPropName) == 0)
		{*resValue = rho_sys_get_locale(); return 1;}

	if (strcasecmp("country",szPropName) == 0)
	{
		wchar_t szCountry[20];
		int nRes = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SISO3166CTRYNAME , szCountry, sizeof(szCountry)/sizeof(szCountry[0]));
		szCountry[2] = 0;

		*resValue = rho_ruby_create_string(convertToStringA(szCountry).c_str());
		return 1;
	}

	if (strcasecmp("device_name",szPropName) == 0)
	{
#ifdef OS_WINDOWS
		*resValue = rho_ruby_create_string("Win32");
        return 1;
#else
		HKEY hKey;
        int nRes = 0;
		if (RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("Ident"),
			0, KEY_READ, &hKey ) != ERROR_SUCCESS)
			return nRes;

		DWORD dwType = REG_SZ;
		DWORD dwDataSize = 0;
		if ( RegQueryValueEx( hKey, _T("name"), 0, &dwType, (PBYTE)NULL, &dwDataSize ) == ERROR_SUCCESS)
        {
		    CAtlString deviceName;
		    RegQueryValueEx( hKey, _T("name"), 0, &dwType, 
                (PBYTE)deviceName.GetBuffer((dwDataSize/sizeof(TCHAR) + sizeof(TCHAR))), &dwDataSize );
            deviceName.ReleaseBuffer();

		    *resValue = rho_ruby_create_string( convertToStringA(deviceName.GetString()).c_str() );

            nRes = 1;
        }

		RegCloseKey(hKey);
		return nRes;
#endif
	}

	if (strcasecmp("os_version",szPropName) == 0)
	{
		OSVERSIONINFO osv;
		osv.dwOSVersionInfoSize = sizeof(osv);
		if (!GetVersionEx(&osv))
			return 0;
		char buf[50];
		buf[sizeof(buf) - 1] = '\0';
		snprintf(buf, sizeof(buf) - 1, "%u.%u.%u", (unsigned)osv.dwMajorVersion,
			(unsigned)osv.dwMinorVersion, (unsigned)osv.dwBuildNumber);
		*resValue = rho_ruby_create_string(&buf[0]);
		return 1;
	}

	if (strcasecmp("is_emulator",szPropName) == 0)
    {
        bool bEmulator = false;
#ifdef OS_WINDOWS
        bEmulator = true;
#else

#endif
        *resValue = rho_ruby_create_boolean(bEmulator?1:0);
        return 1;
    }

    return 0;
}

int rho_sys_get_screen_width()
{
#ifdef OS_WINCE
	return GetSystemMetrics(SM_CXSCREEN);
#else
	return CMainWindow::getScreenWidth();
#endif
}

int rho_sys_get_screen_height()
{
#ifdef OS_WINCE
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

void rho_sys_app_exit()
{
	::PostMessage(getMainWnd(), WM_COMMAND, MAKEWPARAM(IDM_EXIT,0), (LPARAM )0);
}

void rho_wmsys_run_app(const char* szPath, const char* szParams );
void rho_sys_run_app(const char *appname, VALUE params)
{
    rho_wmsys_run_app(appname, 0);
}

void rho_sys_open_url(const char* url)
{
    rho_wmsys_run_app(url, 0);
}

void rho_wmsys_run_app(const char* szPath, const char* szParams )
{
    SHELLEXECUTEINFO se = {0};
    se.cbSize = sizeof(SHELLEXECUTEINFO);
    se.fMask = SEE_MASK_NOCLOSEPROCESS;
    se.lpVerb = L"Open";

    StringW strAppNameW;
    convertToStringW(szPath, strAppNameW);
    for(int i = 0; i<(int)strAppNameW.length();i++)
    {
        if ( strAppNameW.at(i) == '/' )
            strAppNameW.at(i) = '\\';
    }
    se.lpFile = strAppNameW.c_str();

    StringW strParamsW;
    if ( szParams && *szParams )
    {
        convertToStringW(szParams, strParamsW);
        se.lpParameters = strParamsW.c_str();
    }

    ShellExecuteEx(&se);
}

int rho_sys_is_app_installed(const char *appname)
{
#ifdef OS_WINCE
/*    LPCWSTR wszFavoriteXml = 
L"<wap-provisioningdoc>"
L"<characteristic-query type=\"UnInstall\" recursive=\"true\">"
L"</characteristic-query>"
L"</wap-provisioningdoc>";

<wap-provisioningdoc> 
   <characteristic type="UnInstall"> 
      <characteristic type="My Application"> 
         <parm name="uninstall" value="1"/>    
      </characteristic> 
   </characteristic> 
</wap-provisioningdoc>


    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    // Process the XML.
    hr = DMProcessConfigXML(wszFavoriteXml, CFGFLAG_PROCESS, &wszOutput);
    
    // The caller must delete the XML returned from DMProcessConfigXML.
    delete [] wszOutput;    */
#endif

    return 0;
}

void rho_sys_app_uninstall(const char *appname)
{
}

} //extern "C"
