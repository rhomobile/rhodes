#include "stdafx.h"
#include "logging/RhoLog.h"

#include "MainWindow.h"
#include "Utils.h"
#include "DateTimePicker.h"


extern "C" HWND getMainWnd();

CDateTimePickerDialog::CDateTimePickerDialog (int format)
{
	m_time = 0;
	m_format = format;
}

CDateTimePickerDialog::~CDateTimePickerDialog ()
{
}

LRESULT CDateTimePickerDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)
    SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
    RHO_ASSERT(SHInitDialog(&shidi));


    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_GETURL_MENUBAR;
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    RHO_ASSERT(SHCreateMenuBar(&mbi));
	
	if (m_format == FORMAT_TIME)
		GetDlgItem(IDC_DATE_CTRL).ShowWindow(SW_HIDE);

	if (m_format == FORMAT_DATE)
		GetDlgItem(IDC_TIME_CTRL).ShowWindow(SW_HIDE);


#endif
    return 1;
}

LRESULT CDateTimePickerDialog::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	SYSTEMTIME sysTime;

	if (m_format == FORMAT_TIME)
	{
		DateTime_GetSystemtime (GetDlgItem(IDC_TIME_CTRL), &sysTime);
	} 
	else if (m_format == FORMAT_DATE)
	{
		DateTime_GetSystemtime (GetDlgItem(IDC_DATE_CTRL), &sysTime);
	} 
	else if (m_format == FORMAT_DATE_TIME)
	{
		SYSTEMTIME time, date;

		DateTime_GetSystemtime (GetDlgItem(IDC_TIME_CTRL), &time);
		DateTime_GetSystemtime (GetDlgItem(IDC_DATE_CTRL), &date);

		sysTime.wYear		= date.wYear; 
		sysTime.wMonth		= date.wMonth; 
		sysTime.wDayOfWeek	= date.wDayOfWeek; 
		sysTime.wDay		= date.wDay; 
		sysTime.wHour		= time.wHour; 
		sysTime.wMinute		= time.wMinute;
		sysTime.wSecond		= time.wSecond; 
	} 
	else
	{
		LOG(ERROR) + "invalid format";
		m_time = 0;
	}

	m_time = SystemTimeToUnixTime (&sysTime);

    EndDialog(wID);
    return 0;
}

time_t CDateTimePickerDialog::GetTime()
{
	return m_time;
}

LRESULT CDateTimePickerDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

void  choose_datetime(char* callback, char* title, 
					  long initial_time, int format, char* data)
{
	LOG(INFO) + __FUNCTION__ + "callback = " + callback + " title = " + title;

	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_DATETIME_PICKER, 0, 
					(LPARAM)new CDateTimeMessage(callback, title, initial_time, format, data));
}

LRESULT CDateTimePickerDialog::OnDtnDatetimechangeTimeCtrl(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here

	return 0;
}

LRESULT CDateTimePickerDialog::OnMcnSelchangeDateCtrl(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here

	return 0;
}
