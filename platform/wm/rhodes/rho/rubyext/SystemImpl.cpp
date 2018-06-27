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

#if defined(RHODES_QT_PLATFORM)

#include <windows.h>

#ifdef OS_WINDOWS_DESKTOP
#include <Shellapi.h>
#endif //OS_WINDOWS_DESKTOP

#include "common/RhoDefs.h"
#include "common/RhoStd.h"
#undef null
#include <qglobal.h>
#ifndef OS_SAILFISH
#if QT_VERSION > QT_VERSION_CHECK(5,7,0)
#include <QtWebEngine/qtwebengineversion.h>
#elif  QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWebKit/qtwebkitversion.h>
#else
#include <QtWebKit/qwebkitversion.h>
#endif
#endif
#include "../../qt/rhodes/impl/MainWindowImpl.h"

#else // RHODES_QT_PLATFORM

#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "MainWindow.h"

#if defined(OS_WINDOWS_DESKTOP)
#undef null
#include <QWebPage>
#include <qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWebKit/qtwebkitversion.h>
#endif
#endif

extern "C" HWND getMainWnd();
CMainWindow& getAppWindow();

#endif // RHODES_QT_PLATFORM

using namespace rho;
using namespace rho::common;

extern "C"
{

#if defined(OS_WINDOWS_DESKTOP)
const char* rho_sys_qt_getWebviewFramework();
#endif

bool rho_rhosim_window_closed()
{
/*#ifdef RHODES_EMULATOR
    return CMainWindow::mainWindowClosed;
#else*/
    return false;
//#endif
}

void rho_wmsys_run_app_with_show(const wchar_t* szPath, const wchar_t* szParams, bool bShow )
{
    SHELLEXECUTEINFO se = {0};
    se.cbSize = sizeof(SHELLEXECUTEINFO);
    se.fMask  = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    se.lpVerb = L"Open";
    se.nShow  = (bShow == true) ? SW_SHOWNORMAL : SW_HIDE;

    StringW strAppNameW = szPath;
    String_replace(strAppNameW, '/', '\\' );

    se.lpFile = strAppNameW.c_str();

    if ( szParams && *szParams )
        se.lpParameters = szParams;

    bool bError = false;
    if ( !ShellExecuteEx(&se) )
    {
        String strPath = convertToStringA(szPath);

        if ( String_startsWith(strPath, "http:") || String_startsWith(strPath, "ftp:") )
        {
            //Try to run internet explorer
			if(RHO_IS_CEDEVICE)
				se.lpFile = L"iesample.exe";
			else
				se.lpFile = L"iexplore.exe";

            StringW strParamsW = szPath;
            if ( szParams && *szParams )
            {
                strParamsW += L" ";
                strParamsW += szParams;
            }

            se.lpParameters = strParamsW.c_str();

            if ( !ShellExecuteEx(&se) )
                bError = true;
        }else
            bError = true;
    }

    if ( bError )
    {
        LOG(ERROR) + "Cannot execute: " + strAppNameW + ";Error: " + GetLastError();
        SetLastError(-1);
    }

    if(se.hProcess)
        CloseHandle(se.hProcess);
}

void rho_wmsys_run_appW(const wchar_t* szPath, const wchar_t* szParams )
{
    rho_wmsys_run_app_with_show(szPath, szParams, true);
}

#if !defined(RHODES_QT_PLATFORM)

void rho_sys_app_exit()
{
	::PostMessage(getMainWnd(), WM_COMMAND, MAKEWPARAM(IDM_EXIT,0), (LPARAM )0);
}

void rho_wmsys_run_appW(const wchar_t* szPath, const wchar_t* szParams );

void rho_wmsys_run_app(const char* szPath, const char* szParams )
{
    StringW strAppNameW;
    convertToStringW(szPath, strAppNameW);

    StringW strParamsW;
    if ( szParams && *szParams )
        convertToStringW(szParams, strParamsW);

    rho_wmsys_run_appW(strAppNameW.c_str(), strParamsW.c_str() );
}

#ifdef OS_WINDOWS_DESKTOP
void rho_win32sys_run_appW(const wchar_t* szPath, const wchar_t* szParams, const wchar_t* szDir)
{
	HINSTANCE result = ShellExecute(NULL, NULL, (WCHAR*)szPath, (WCHAR*)szParams, (WCHAR*)szDir, SW_SHOW);
	if ((int)result > 32) {
		LOG(INFO) + "Application started: " + szPath;
	} else {
		LOG(ERROR) + "Cannot start application: " + szPath;
	}
}
#endif

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

void rho_sys_bring_to_front()
{
    SetForegroundWindow(getMainWnd());
}

void rho_sys_report_app_started()
{
    LOG(INFO) + "rho_sys_report_app_started() has no implementation on Win Mobile.";
	RHODESAPP().initPushClients();
}


#if defined(OS_WINDOWS_DESKTOP)

namespace rho {

class CRhoWindow
{
public:
    class CParams
    {
	private:
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	public:
		CParams(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height) {}
		~CParams(){}
		int getX() { return m_x; }
		int getY() { return m_y; }
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
	};

public:
	CRhoWindow(void){}
	~CRhoWindow(void){}

    static void setFrame(CParams* params)
	{
	    CParams& frame = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.setFrame(frame.getX(), frame.getY(), frame.getWidth(), frame.getHeight());
	}
    static void setPosition(CParams* params)
	{
	    CParams& frame = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.setPosition(frame.getX(), frame.getY());
	}
    static void setSize(CParams* params)
	{
	    CParams& frame = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.setSize(frame.getWidth(), frame.getHeight());
	}
	static void lockSize(CParams* params)
	{
	    CParams& p = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.lockSize(p.getX());
	}
};

}

void rho_sys_set_window_frame(int x0, int y0, int width, int height)
{
	rho_callInUIThread(CRhoWindow::setFrame, new CRhoWindow::CParams(x0, y0, width, height));
}

void rho_sys_set_window_position(int x0, int y0)
{
	rho_callInUIThread(CRhoWindow::setPosition, new CRhoWindow::CParams(x0, y0, 0, 0));
}

void rho_sys_set_window_size(int width, int height)
{
	rho_callInUIThread(CRhoWindow::setSize, new CRhoWindow::CParams(0, 0, width, height));
}

void rho_sys_lock_window_size(int locked)
{
	rho_callInUIThread(CRhoWindow::lockSize, new CRhoWindow::CParams(locked, 0, 0, 0));
}
#endif

#endif // RHODES_QT_PLATFORM

}
#if 0
#include "stdafx.h"

#include "common/RhoPort.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"
#ifdef OS_WINDOWS_DESKTOP
#include "common/RhoFile.h"
#endif
#include "ruby/ext/rho/rhoruby.h"
#include "common/app_build_capabilities.h"
#if defined(OS_WINDOWS_DESKTOP)
#undef null
#include <QWebPage>
#endif
#include "MainWindow.h"

#if defined( OS_WINCE ) && !defined( OS_PLATFORM_MOTCE )
#include <cfgmgrapi.h>
#include <getdeviceuniqueid.h>
#include <wininet.h>
#endif

#ifdef OS_WINCE__
#include <tapi.h>
#include <tsp.h>
//#include <sms.h>
#endif

using namespace rho;
using namespace rho::common;
extern "C" HWND getMainWnd();
extern "C" char* wce_wctomb(const wchar_t* w);

#if defined(OS_WINDOWS_DESKTOP)

//extern "C" CMainWindow* Rhodes_getMainWindow();
CMainWindow& getAppWindow();

namespace rho {

class CRhoWindow
{
public:
    class CParams
    {
	private:
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	public:
		CParams(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height) {}
		~CParams(){}
		int getX() { return m_x; }
		int getY() { return m_y; }
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
	};

public:
	CRhoWindow(void){}
	~CRhoWindow(void){}

    static void setFrame(CParams* params)
	{
	    CParams& frame = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.setFrame(frame.getX(), frame.getY(), frame.getWidth(), frame.getHeight());
	}
    static void setPosition(CParams* params)
	{
	    CParams& frame = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.setPosition(frame.getX(), frame.getY());
	}
    static void setSize(CParams* params)
	{
	    CParams& frame = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.setSize(frame.getWidth(), frame.getHeight());
	}
	static void lockSize(CParams* params)
	{
	    CParams& p = *((CParams*)params);
		CMainWindow& mainWin = getAppWindow();
		mainWin.lockSize(p.getX());
	}
};

}
#endif

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

int rho_wmsys_has_touchscreen()
{
#if defined( OS_WINDOWS_DESKTOP ) || defined( OS_PLATFORM_MOTCE )
        return 1;
#else
        BOOL bRet;
        TCHAR oem[257];
#ifndef WIN32_PLATFORM_WFSP
        // fix for all supported platforms expect Smartphone 2003:
        // DeviceEmulator sould report it has a touch screen
        bRet = SystemParametersInfo(SPI_GETOEMINFO, sizeof(oem), oem, 0);
        if (bRet && wcsstr(oem, _T("DeviceEmulator"))!=NULL)
            return 1;
#endif

        bRet = SystemParametersInfo(SPI_GETPLATFORMTYPE, sizeof(oem), oem, 0);
        // SmartPhone has no touch screen
        if (bRet && wcsstr(oem, _T("SmartPhone"))!=NULL)
            return 0;
        // PocketPC has touch screen
        if (bRet && wcsstr(oem, _T("PocketPC"))!=NULL)
            return 1;

        // otherwise check mouse info
        int aMouseInfo[3] = {0};
        bRet = SystemParametersInfo(SPI_GETMOUSE, sizeof(aMouseInfo), aMouseInfo, 0);
        return (bRet && aMouseInfo[0] != 0) ? 1 : 0;
#endif
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

static void toHexString(int i, String& strRes, int radix)
{
    char buf[33];
	bool neg= false;
	int f, n;
	if(i<0) { neg= true; i= -i; };
	f= 32;
	buf[f--]= 0;
	do
	{
		n= i%radix;
		if(n<10) buf[f]= '0'+n;
		else buf[f]= 'a'+n-10;
		i= i/radix;
		f--;
	}
	while(i>0);

	if(neg) 
        buf[f--]= '-';

    strRes += (buf+f+1);
}

#if !defined(OS_WINDOWS_DESKTOP)
int get_msie_version(rho::String& msieVer)
// Return codes are as follows:
//    0  : Success
//    1  : Unable to open Registry Key
//    2  : Unable to read key value
{
#ifdef OS_WINDOWS_DESKTOP
    LONG lResult;
    HKEY hKey;
    DWORD dwSize=100,dwType;
    char szVAL[100];

    // Open the key for query access
    lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        LPCTSTR("SOFTWARE\\Microsoft\\Internet Explorer"),
        0,KEY_QUERY_VALUE,&hKey);

    if(lResult != ERROR_SUCCESS)   // Unable to open Key
    {
        return 1;
    }

    // OK, read the value
    lResult=::RegQueryValueEx(hKey,LPTSTR("Version"),NULL,
        &dwType, LPBYTE(szVAL),&dwSize);

    if(lResult != ERROR_SUCCESS)    // Unable to get value
    {
        // Close the key before quitting
        lResult=::RegCloseKey(hKey);
        return 2;
    }

    // Close the key
    lResult=::RegCloseKey(hKey);

    msieVer += "/";
    msieVer += szVAL;
#endif
    return 0;
}
#endif

static int g_rho_has_network = 1, g_rho_has_cellnetwork = 0;


int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
	if (strcasecmp("webview_framework",szPropName) == 0)
	{
#if defined(OS_WINDOWS_DESKTOP)
		*resValue = rho_ruby_create_string(rho_sys_qt_getWebviewFramework());
#elif defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
		*resValue = rho_ruby_create_string("WEBKIT/SYMBOL");
#else
		rho::String msieVer = "IE";
		get_msie_version(msieVer);
		*resValue = rho_ruby_create_string(msieVer.c_str());
#endif
		return 1;
	}

#ifdef RHODES_EMULATOR
    if (strcasecmp("main_window_closed",szPropName) == 0) 
        {*resValue = rho_ruby_create_boolean(CMainWindow::mainWindowClosed); return 1;}
#endif

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
#ifdef OS_WINDOWS_DESKTOP
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
#ifdef OS_WINDOWS_DESKTOP
        bEmulator = true;
#else
		TCHAR buf[255];
		SystemParametersInfo(SPI_GETOEMINFO, 255, &buf, NULL);
		bEmulator = (wcsnicmp(buf, _T("Microsoft DeviceEmulator"), 24)==0);
#endif
        *resValue = rho_ruby_create_boolean(bEmulator?1:0);
        return 1;
    }

	if (strcasecmp("has_calendar",szPropName) == 0)
    {
        *resValue = rho_ruby_create_boolean(1);
        return 1;
    }

	if (strcasecmp("has_touchscreen",szPropName) == 0)
    {
        *resValue = rho_ruby_create_boolean( rho_wmsys_has_touchscreen() );
        return 1;
    }
	if (strcasecmp("screen_orientation",szPropName) == 0)
    {
        if (rho_sys_get_screen_width() <= rho_sys_get_screen_height()) 
        {
		    *resValue = rho_ruby_create_string("portrait");
	    }
	    else {
		    *resValue = rho_ruby_create_string("landscape");
	    }                                                          
        return 1;
    }

//TODO: this is phoneId not deviceId, deviceId is push regsitrtaion ID, getting from RhoConnect server
#if defined( OS_WINCE )&& !defined( OS_PLATFORM_MOTCE )
    if (strcasecmp("device_id",szPropName) == 0) 
    {
        rho::String strDeviceID = "";
        String strAppData = "RHODES_" + RHODESAPP().getAppName() + "_DEVICEID";

        BYTE rgDeviceId[20];
        DWORD cbDeviceId = sizeof(rgDeviceId);
        HRESULT hr = GetDeviceUniqueID( (PBYTE)(strAppData.c_str()),
           strAppData.length(),
           GETDEVICEUNIQUEID_V1,
           rgDeviceId,
           &cbDeviceId);

        if ( SUCCEEDED(hr) )
        {
            for(unsigned int i = 0; i < cbDeviceId; i++)
            {
                toHexString( rgDeviceId[i], strDeviceID, 16);
            }
        }

        *resValue = rho_ruby_create_string(strDeviceID.c_str());

        return 1;
    }
#endif

	if (strcasecmp("has_cell_network",szPropName) == 0) 
    {
        *resValue = rho_ruby_create_boolean( g_rho_has_cellnetwork != 0 );
        return 1;
    }

	if (strcasecmp("has_wifi_network",szPropName) == 0) 
    {
        *resValue = rho_ruby_create_boolean( g_rho_has_network != 0 );
        return 1;
    }

    return 0;
}

VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values) 
{
	return rho_ruby_get_NIL();
}

void rho_sysimpl_sethas_network(int nValue)
{
    g_rho_has_network = nValue >= 1 ? 1 : 0;
}

void rho_sysimpl_sethas_cellnetwork(int nValue)
{
    g_rho_has_cellnetwork = nValue;
}

VALUE rho_sys_has_network()
{
	return rho_ruby_create_boolean( g_rho_has_cellnetwork != 0 || g_rho_has_network!=0);
}

void rho_sys_app_exit()
{
	::PostMessage(getMainWnd(), WM_COMMAND, MAKEWPARAM(IDM_EXIT,0), (LPARAM )0);
}

void rho_wmsys_run_appW(const wchar_t* szPath, const wchar_t* szParams );

void rho_wmsys_run_app(const char* szPath, const char* szParams )
{
    StringW strAppNameW;
    convertToStringW(szPath, strAppNameW);

    StringW strParamsW;
    if ( szParams && *szParams )
        convertToStringW(szParams, strParamsW);

    rho_wmsys_run_appW(strAppNameW.c_str(), strParamsW.c_str() );
}

void rho_wmsys_run_appW(const wchar_t* szPath, const wchar_t* szParams )
{
	SHELLEXECUTEINFO se = {0};
    se.cbSize = sizeof(SHELLEXECUTEINFO);
    se.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    se.lpVerb = L"Open";
    se.nShow = SW_SHOWNORMAL;

    StringW strAppNameW = szPath;
    String_replace(strAppNameW, '/', '\\' );

    se.lpFile = strAppNameW.c_str();

    if ( szParams && *szParams )
        se.lpParameters = szParams;

    if ( !ShellExecuteEx(&se) )
	{
        LOG(ERROR) + "Cannot execute: " + strAppNameW + ";Error: " + GetLastError();
		SetLastError(-1);
	}

    if(se.hProcess)
        CloseHandle(se.hProcess); 
}

#ifdef OS_WINDOWS_DESKTOP
void rho_win32sys_run_appW(const wchar_t* szPath, const wchar_t* szParams, const wchar_t* szDir)
{
	HINSTANCE result = ShellExecute(NULL, NULL, (WCHAR*)szPath, (WCHAR*)szParams, (WCHAR*)szDir, SW_SHOW);
	if ((int)result > 32) {
		LOG(INFO) + "Application started: " + szPath;
	} else {
		LOG(ERROR) + "Cannot start application: " + szPath;
	}
}
#endif

void rho_sys_open_url(const char* url)
{
    rho_wmsys_run_app(url, 0);
}

static LONG openRegAppPath(const char *appname, CRegKey& oKey, StringW& strKeyPath)
{
    CFilePath oPath(appname);
    String strAppName = oPath.getFolderName();

#ifdef OS_WINDOWS_DESKTOP
    strKeyPath = L"SOFTWARE\\";
#else
    strKeyPath = L"Software\\Apps\\";
#endif

    strKeyPath += convertToStringW(strAppName);
    String_replace(strKeyPath, '/', '\\' );

#ifdef OS_WINDOWS_DESKTOP
    return oKey.Open(HKEY_CURRENT_USER, strKeyPath.c_str(), KEY_READ|KEY_WOW64_64KEY );
#else
    return oKey.Open(HKEY_LOCAL_MACHINE, strKeyPath.c_str(), KEY_READ ); 
#endif
}

void rho_sys_run_app(const char *appname, VALUE params)
{
    CRegKey oKey;
    StringW strKeyPath;
    LONG res = openRegAppPath(appname, oKey, strKeyPath);

    if ( res != ERROR_SUCCESS )
    {
        LOG(ERROR) + "Cannot open registry key: " + strKeyPath + "; Code:" + res;
    } else
    {
        TCHAR szBuf[MAX_PATH+1];
        ULONG nChars = MAX_PATH;

        res = oKey.QueryStringValue(L"InstallDir", szBuf, &nChars);
        if ( res != ERROR_SUCCESS )
            LOG(ERROR) + "Cannot read registry key: InstallDir; Code:" + res;
        else
        {
            StringW strFullPath = szBuf;

            if ( strFullPath[strFullPath.length()-1] != '/' && strFullPath[strFullPath.length()-1] != '\\' )
                strFullPath += L"\\";

			StringW strBaseName;
            CFilePath oPath(appname);
            convertToStringW(oPath.getBaseName(), strBaseName);
            strFullPath += strBaseName;

            StringW strParamsW;
            if ( params && !rho_ruby_is_NIL(params) )
            {
                convertToStringW(getStringFromValue(params), strParamsW);
            }

            rho_wmsys_run_appW(strFullPath.c_str(), strParamsW.c_str());
        }
    }
}

void rho_sys_bring_to_front()
{
    LOG(INFO) + "rho_sys_bring_to_front() has no implementation on Win Mobile.";
}

void rho_sys_report_app_started()
{
    LOG(INFO) + "rho_sys_report_app_started() has no implementation on Win Mobile.";
	RHODESAPP().initPushClients();
}

int rho_sys_is_app_installed(const char *appname)
{
#ifdef OS_WINDOWS_DESKTOP
    CRegKey oKey;
    StringW strKeyPath;
    LONG res = openRegAppPath(appname, oKey, strKeyPath);

	return res == ERROR_SUCCESS ? 1 : 0;
#else
	int nRet = 0;
    CFilePath oPath(appname);
    String strAppName = oPath.getFolderName();
    
    StringW strRequest = 
        L"<wap-provisioningdoc><characteristic type=\"UnInstall\">"
        L"<characteristic-query type=\"";
    strRequest += convertToStringW(strAppName) + L"\"/>"
        L"</characteristic></wap-provisioningdoc>"; 

#if defined( OS_WINCE ) && !defined( OS_PLATFORM_MOTCE )
    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    hr = DMProcessConfigXML(strRequest.c_str(), CFGFLAG_PROCESS, &wszOutput);
    if (FAILED(hr) || !wszOutput )
        LOG(ERROR) + "DMProcessConfigXML failed: " + hr;
    else
    {
        StringW strResp = L"<characteristic type=\"";
        strResp += convertToStringW(strAppName) + L"\">";
        nRet = wcsstr(wszOutput, strResp.c_str()) != 0 ? 1 : 0;
    }

    if ( wszOutput )
        free( wszOutput );
#endif

    return nRet;
#endif
}

void rho_sys_app_install(const char *url)
{
#ifdef OS_WINDOWS_DESKTOP
	String sUrl = url;
    CFilePath oFile(sUrl);
	String filename = RHODESAPP().getRhoUserPath()+ oFile.getBaseName();
	if (CRhoFile::isFileExist(filename.c_str()) && (CRhoFile::deleteFile(filename.c_str()) != 0)) {
		LOG(ERROR) + "rho_sys_app_install() file delete failed: " + filename;
	} else {
		NetRequest NetRequest;
		NetResponse resp = getNetRequest(&NetRequest).pullFile(sUrl, filename, NULL, NULL);
		if (resp.isOK()) {
			StringW filenameW = convertToStringW(filename);
			LOG(INFO) + "Downloaded " + sUrl + " to " + filename;
			rho_wmsys_run_appW(filenameW.c_str(), L"");
		} else {
			LOG(ERROR) + "rho_sys_app_install() download failed: " + sUrl;
		}
	}
#else
    rho_sys_open_url(url);
#endif
}

void rho_sys_app_uninstall(const char *appname)
{
#ifdef OS_WINDOWS_DESKTOP
    CRegKey oKey;
    StringW strKeyPath;
    LONG res = openRegAppPath(appname, oKey, strKeyPath);
    if ( res != ERROR_SUCCESS )
    {
        LOG(ERROR) + "Cannot open registry key: " + strKeyPath + "; Code:" + res;
    } else
    {
        TCHAR szBuf[MAX_PATH+1];
        ULONG nChars = MAX_PATH;

        res = oKey.QueryStringValue(L"Uninstaller", szBuf, &nChars);
        if ( res != ERROR_SUCCESS )
            LOG(ERROR) + "Cannot read registry key: Uninstaller; Code:" + res;
        else
        {
            StringW strFullPath = szBuf;
			rho_wmsys_run_appW( strFullPath.c_str(), L"" );
		}
	}
#else
	CFilePath oPath(appname);
    String strAppName = oPath.getFolderName();
    
    StringW strRequest = 
        L"<wap-provisioningdoc><characteristic type=\"UnInstall\">"
        L"<characteristic type=\"";
    strRequest += convertToStringW(strAppName) + L"\">"
        L"<parm name=\"uninstall\" value=\"1\"/>"
        L"</characteristic>"
        L"</characteristic></wap-provisioningdoc>";

#if defined( OS_WINCE )&& !defined( OS_PLATFORM_MOTCE )
    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    hr = DMProcessConfigXML(strRequest.c_str(), CFGFLAG_PROCESS, &wszOutput);
    if (FAILED(hr) || !wszOutput )
        LOG(ERROR) + "DMProcessConfigXML failed: " + hr;
    else
    {
    }

    if ( wszOutput )
        free( wszOutput );
#endif
#endif
}

void rho_sys_set_application_icon_badge(int badge_number) {
    //unsupported on WM
}

#if defined(OS_WINDOWS_DESKTOP)
void rho_sys_set_window_frame(int x0, int y0, int width, int height)
{
	rho_callInUIThread(CRhoWindow::setFrame, new CRhoWindow::CParams(x0, y0, width, height));
}

void rho_sys_set_window_position(int x0, int y0)
{
	rho_callInUIThread(CRhoWindow::setPosition, new CRhoWindow::CParams(x0, y0, 0, 0));
}

void rho_sys_set_window_size(int width, int height)
{
	rho_callInUIThread(CRhoWindow::setSize, new CRhoWindow::CParams(0, 0, width, height));
}

void rho_sys_lock_window_size(int locked)
{
	rho_callInUIThread(CRhoWindow::lockSize, new CRhoWindow::CParams(locked, 0, 0, 0));
}
#endif

} //extern "C"
#endif //0
