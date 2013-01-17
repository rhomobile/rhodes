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

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "common/RhoPort.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/app_build_capabilities.h"

using namespace Windows::Graphics::Display;
using namespace Windows::Devices::Input;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;

using namespace rho;
using namespace rho::common;
extern "C" HWND getMainWnd();
extern "C" char* wce_wctomb(const wchar_t* w);

VALUE phone_number()
{
	return rho_ruby_get_NIL();
}

static int has_camera()
{
    return 1;
}

static double get_screen_ppi_x()
{
	return DisplayProperties::LogicalDpi;
}

static double get_screen_ppi_y()
{
	return DisplayProperties::LogicalDpi;
}

VALUE rho_sys_get_locale()
{
	wchar_t szLang[20];
	int nRes = GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,LOCALE_SABBREVLANGNAME , szLang, sizeof(szLang)/sizeof(szLang[0]));
	szLang[2] = 0;
	wcslwr(szLang);

	return rho_ruby_create_string(convertToStringA(szLang).c_str());
}

int rho_wmsys_has_touchscreen()
{
	return 1;
}


int rho_sys_get_screen_width()
{
	Rect rc = CoreWindow::GetForCurrentThread()->Bounds;
	return rc.Width;
}

int rho_sys_get_screen_height()
{
	Rect rc = CoreWindow::GetForCurrentThread()->Bounds;
	return rc.Height;
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

int get_msie_version(rho::String& msieVer)
{
    return 0;
}

static int g_rho_has_network = 1, g_rho_has_cellnetwork = 0;

int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
	if (strcasecmp("webview_framework",szPropName) == 0)
	{
		rho::String msieVer = "IE";
		get_msie_version(msieVer);
		*resValue = rho_ruby_create_string(msieVer.c_str());
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
		int nRes = GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,LOCALE_SISO3166CTRYNAME , szCountry, sizeof(szCountry)/sizeof(szCountry[0]));
		szCountry[2] = 0;

		*resValue = rho_ruby_create_string(convertToStringA(szCountry).c_str());
		return 1;
	}

	if (strcasecmp("device_name",szPropName) == 0)
	{
		HKEY hKey;
        int nRes = 0;
		return nRes;
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
        bool bEmulator = true;
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
    g_rho_has_network = nValue > 1 ? 1 : 0;
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
}

void rho_sys_open_url(const char* url)
{
    rho_wmsys_run_app(url, 0);
}

/*static LONG openRegAppPath(const char *appname, CRegKey& oKey, StringW& strKeyPath)
{
}*/

void rho_sys_run_app(const char *appname, VALUE params)
{
}

void rho_sys_bring_to_front()
{
    LOG(INFO) + "rho_sys_bring_to_front() has no implementation on Win Mobile.";
}

void rho_sys_report_app_started()
{
    LOG(INFO) + "rho_sys_report_app_started() has no implementation on Win Mobile.";
}

int rho_sys_is_app_installed(const char *appname)
{
	int nRet = 0;
    CFilePath oPath(appname);
    String strAppName = oPath.getFolderName();
    
    StringW strRequest = 
        L"<wap-provisioningdoc><characteristic type=\"UnInstall\">"
        L"<characteristic-query type=\"";
    strRequest += convertToStringW(strAppName) + L"\"/>"
        L"</characteristic></wap-provisioningdoc>"; 

    return nRet;
}

void rho_sys_app_install(const char *url)
{
    rho_sys_open_url(url);
}

void rho_sys_app_uninstall(const char *appname)
{
	CFilePath oPath(appname);
    String strAppName = oPath.getFolderName();
    
    StringW strRequest = 
        L"<wap-provisioningdoc><characteristic type=\"UnInstall\">"
        L"<characteristic type=\"";
    strRequest += convertToStringW(strAppName) + L"\">"
        L"<parm name=\"uninstall\" value=\"1\"/>"
        L"</characteristic>"
        L"</characteristic></wap-provisioningdoc>";
}

void rho_sys_set_application_icon_badge(int badge_number) {
    //unsupported on WP8
}
