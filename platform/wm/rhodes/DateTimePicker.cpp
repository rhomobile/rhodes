#include "stdafx.h"
#include "logging/RhoLog.h"

#include "MainWindow.h"
#include "Utils.h"
#include "DateTimePicker.h"

extern "C" int rho_sys_get_screen_height();
extern "C" int rho_sys_get_screen_width();

#define DLG_ITEM_SET_FONT_BOLD(ITEM_ID)							\
{																\
	HFONT hFont = GetDlgItem((ITEM_ID)).GetFont();				\
	LOGFONT fontAttributes = { 0 };								\
    ::GetObject(hFont, sizeof(fontAttributes), &fontAttributes);\
    fontAttributes.lfWeight = FW_BOLD;							\
	hFont = CreateFontIndirect(&fontAttributes);				\
	GetDlgItem((ITEM_ID)).SetFont(hFont);						\
}


/*
 * TODO: title and initial time
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
	SetWindowText(_T("Date"));

#if defined(_WIN32_WCE)

    SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
    RHO_ASSERT(SHInitDialog(&shidi));


    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_GETURL_MENUBAR;
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    RHO_ASSERT(SHCreateMenuBar(&mbi));
	GotoDlgCtrl(GetDlgItem(IDC_DATE_CTRL));

#else 

	CreateButtons();
	GotoDlgCtrl(m_btnOk);

#endif

	DLG_ITEM_SET_FONT_BOLD (IDC_DATE_STATIC);
	DLG_ITEM_SET_FONT_BOLD (IDC_TIME_STATIC);

	if (m_format == CDateTimeMessage::FORMAT_DATE) {
		GetDlgItem(IDC_TIME_CTRL).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TIME_STATIC).ShowWindow(SW_HIDE);
	}

    return FALSE;
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


CTimePickerDialog::CTimePickerDialog (const CDateTimeMessage *msg)
{
	m_returnTime  = 0;
	m_format       = msg->m_format;
	m_title        = msg->m_title;
	m_initialTime  = msg->m_initialTime;
}

CTimePickerDialog::~CTimePickerDialog ()
{
}

LRESULT CTimePickerDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(_T("Date"));

#if defined(_WIN32_WCE)

    SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
    RHO_ASSERT(SHInitDialog(&shidi));

    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_GETURL_MENUBAR;
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    RHO_ASSERT(SHCreateMenuBar(&mbi));
	GotoDlgCtrl(GetDlgItem(IDC_TIME_CTRL));

#else

	CreateButtons();
	GotoDlgCtrl(m_btnOk);

#endif

	DLG_ITEM_SET_FONT_BOLD (IDC_TIME_STATIC);

    return FALSE;
}

LRESULT CTimePickerDialog::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	SYSTEMTIME sysTime;
	DateTime_GetSystemtime (GetDlgItem(IDC_TIME_CTRL), &sysTime);
	m_returnTime = SystemTimeToUnixTime (&sysTime);

    EndDialog(wID);
    return 0;
}

time_t CTimePickerDialog::GetTime()
{
	return m_returnTime;
}

LRESULT CTimePickerDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
