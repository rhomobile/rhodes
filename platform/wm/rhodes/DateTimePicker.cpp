#include "stdafx.h"
#include "logging/RhoLog.h"

#include "MainWindow.h"
#include "Utils.h"
#include "DateTimePicker.h"

/*
 * TODO: title and initial time, flexible behavour f
 */

extern "C" HWND getMainWnd();

CDateTimePickerDialog::CDateTimePickerDialog (const CDateTimeMessage *msg)
{
	m_returnTime  = 0;
	m_format       = msg->m_format;
	m_title        = msg->m_title;
	m_initialTime  = msg->m_initialTime;
}

CDateTimePickerDialog::~CDateTimePickerDialog ()
{
}

LRESULT CDateTimePickerDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)
	//USES_CONVERSION;
	SetWindowText(_T("Date"));

    SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
    RHO_ASSERT(SHInitDialog(&shidi));


    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_GETURL_MENUBAR;
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    RHO_ASSERT(SHCreateMenuBar(&mbi));
	
	if (m_format == CDateTimeMessage::FORMAT_TIME) {
		GetDlgItem(IDC_DATE_CTRL).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATE_STATIC).ShowWindow(SW_HIDE);
	}

	if (m_format == CDateTimeMessage::FORMAT_DATE) {
		GetDlgItem(IDC_TIME_CTRL).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TIME_STATIC).ShowWindow(SW_HIDE);
	}

#endif
    return 1;
}

LRESULT CDateTimePickerDialog::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	SYSTEMTIME sysTime;

	if (m_format == CDateTimeMessage::FORMAT_TIME)
	{
		DateTime_GetSystemtime (GetDlgItem(IDC_TIME_CTRL), &sysTime);
	} 
	else if (m_format == CDateTimeMessage::FORMAT_DATE)
	{
		DateTime_GetSystemtime (GetDlgItem(IDC_DATE_CTRL), &sysTime);
	} 
	else if (m_format == CDateTimeMessage::FORMAT_DATE_TIME)
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
		m_returnTime = 0;
	}

	m_returnTime = SystemTimeToUnixTime (&sysTime);

    EndDialog(wID);
    return 0;
}

time_t CDateTimePickerDialog::GetTime()
{
	return m_returnTime;
}

LRESULT CDateTimePickerDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}


CDatePickerDialog::CDatePickerDialog (const CDateTimeMessage *msg)
{
	m_returnTime  = 0;
	m_format       = msg->m_format;
	m_title        = msg->m_title;
	m_initialTime  = msg->m_initialTime;
}

CDatePickerDialog::~CDatePickerDialog ()
{
}

LRESULT CDatePickerDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)
	//USES_CONVERSION;
	SetWindowText(_T("Date"));

    SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
    RHO_ASSERT(SHInitDialog(&shidi));


    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_GETURL_MENUBAR;
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    RHO_ASSERT(SHCreateMenuBar(&mbi));
#endif

    return 1;
}

LRESULT CDatePickerDialog::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	SYSTEMTIME sysTime;
	DateTime_GetSystemtime (GetDlgItem(IDC_DATE_CTRL), &sysTime);
	m_returnTime = SystemTimeToUnixTime (&sysTime);

    EndDialog(wID);
    return 0;
}

time_t CDatePickerDialog::GetTime()
{
	return m_returnTime;
}

LRESULT CDatePickerDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
