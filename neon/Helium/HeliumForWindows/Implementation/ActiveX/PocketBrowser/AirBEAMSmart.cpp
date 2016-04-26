////////////////////////////////////////////////////////////////////////////////
// Filename: AirBEAMSmart.cpp
// Author:   PB2.x
// Date:     PB2.x
//
// Description: Implementation of the CAirBEAMSmart class, this has been 
//				unmodified from PB2.x
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AirBEAMSmart.h"


/////////////////////////////////////////////////////////////////////////////
// CAirBEAMSmart

//#define WM_SHOWHG		(WM_APP + 1006)
//#define WM_HIDEHG		(WM_APP + 1007)

#define APP_REG_KEY L"Software\\Symbol\\SymbolPB"		//the symbolpb reg key

//extern HWND g_hwndBrowser;

STDMETHODIMP CAirBEAMSmart::get_AutoLoadMode(long *pVal)
{
	*pVal = m_lAutoLoadMode;
	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::put_AutoLoadMode(long newVal)
{
	if(newVal >= 0 && newVal < 4){
		m_lAutoLoadMode = newVal;
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CAirBEAMSmart::get_RAMManagement(BOOL *pVal)
{
	*pVal = m_bRAMManagement;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::put_RAMManagement(BOOL newVal)
{
	m_bRAMManagement = newVal;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::AddPackage(BSTR strPackageName, int *iRetVal)
{
	if(m_iPackageCount >= 4)
		*iRetVal = FALSE;
	else
	{
		m_bstrPackages[m_iPackageCount++] = strPackageName;
		*iRetVal = TRUE;
	}

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::get_AutoReboot(BOOL *pVal)
{
	*pVal = m_bAutoReboot;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::put_AutoReboot(BOOL newVal)
{
	m_bAutoReboot = newVal;

	return S_OK;
}



STDMETHODIMP CAirBEAMSmart::get_TestMode(BOOL *pVal)
{
	*pVal = m_bTestMode;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::put_TestMode(BOOL newVal)
{
	m_bTestMode = newVal;

	return S_OK;
}


STDMETHODIMP CAirBEAMSmart::Run(int *iRetVal)
{

//	if(!g_hwndBrowser){
//		
//		DWORD dwRet = sizeof(HWND);
//		CRegistry::GetSetting(HKEY_CURRENT_USER, APP_REG_KEY, L"__wnd", 
//			(BYTE*) &g_hwndBrowser, &dwRet);
//	}
	
//	if(g_hwndBrowser)
//		::SendMessage(g_hwndBrowser, WM_SHOWHG, 0,0);


	TCHAR tcCommandLine[MAX_PATH*10] = {0};

	if(m_lAutoLoadMode > -1)
		wsprintf(tcCommandLine, L" -L%d", m_lAutoLoadMode);

	if(m_bRAMManagement)
		wcscat(tcCommandLine, L" -M");

	for(int i=0; i<m_iPackageCount; i++)
	{
		wcscat(tcCommandLine, L" -P");
		wcscat(tcCommandLine, m_bstrPackages[i]);
	}

	if(!m_bAutoReboot)
		wcscat(tcCommandLine, L" -R");
	
	if(wcslen(m_bstrUploadPackage))
	{
		wcscat(tcCommandLine, L" -U");
		wcscat(tcCommandLine, m_bstrUploadPackage);
	}

	if(m_bTestMode)
		wcscat(tcCommandLine, L" -T");
	
	if(wcslen(m_bstrDeletePackage))
	{
		wcscat(tcCommandLine, L" -D");
		wcscat(tcCommandLine, m_bstrDeletePackage);
	}

	PROCESS_INFORMATION ProcInfo;

	memset(&ProcInfo, 0, sizeof(ProcInfo));

	CreateProcess( L"abclient.exe", tcCommandLine, 0,0, 0, 0, 0,0,0, &ProcInfo);

	DWORD dwExitCode = 0;
	do{
		if(!GetExitCodeProcess( ProcInfo.hProcess, &dwExitCode))
			break;

	} while(dwExitCode == 259/*STILL_ALIVE*/);

	*iRetVal = dwExitCode;

//	if(g_hwndBrowser)
//		::SendMessage(g_hwndBrowser, WM_HIDEHG, 0,0);


	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::get_UploadPackage(BSTR *pVal)
{
	*pVal = m_bstrUploadPackage;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::put_UploadPackage(BSTR newVal)
{
	m_bstrUploadPackage = newVal;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::get_DeletePackage(BSTR *pVal)
{
	*pVal = m_bstrDeletePackage;

	return S_OK;
}

STDMETHODIMP CAirBEAMSmart::put_DeletePackage(BSTR newVal)
{
	m_bstrDeletePackage = newVal;

	return S_OK;
}
