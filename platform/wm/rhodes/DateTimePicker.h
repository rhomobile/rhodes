#ifndef __DATETIMEPICKER_H__
#define __DATETIMEPICKER_H__

#include "resource.h"

//TODO: delegates
class CDateTimeMessage 
{
public:
	CDateTimeMessage (char* callback, char* title, long initial_time, int format, char* data)
	{
		m_callback    = strdup (callback);
		m_title       = strdup (title);
		m_initialTime = initial_time;
		m_format      = format;
		m_data        = strdup (data);
	}

	~CDateTimeMessage ()
	{
		free (m_callback);
		free (m_title);
		free (m_data);
	}

	char *m_callback;
	char *m_title;
	long m_initialTime;
	int  m_format;
	char *m_data;
};

class CDateTimePickerDialog : public CDialogImpl<CDateTimePickerDialog>
{

public:
	static enum 
	{
		FORMAT_DATE_TIME,
		FORMAT_DATE,
		FORMAT_TIME
	};

	CDateTimePickerDialog  (int format);
	~CDateTimePickerDialog ();

	time_t GetTime();

	enum { IDD = IDD_TIMEPICKER };
	
	BEGIN_MSG_MAP(CDateTimePickerDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		NOTIFY_HANDLER(IDC_TIME_CTRL, DTN_DATETIMECHANGE, OnDtnDatetimechangeTimeCtrl)
		NOTIFY_HANDLER(IDC_DATE_CTRL, MCN_SELCHANGE, OnMcnSelchangeDateCtrl)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnDtnDatetimechangeTimeCtrl(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnMcnSelchangeDateCtrl(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

private:
	int    m_format;
	time_t m_time;
};


extern "C" void  choose_datetime(char* callback, char* title, 
								 long initial_time, int format, char* data);

#endif //__DATETIMEPICKER_H__
