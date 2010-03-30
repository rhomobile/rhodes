#ifndef __DATETIMEPICKER_H__
#define __DATETIMEPICKER_H__

#include "resource.h"

//TODO: delegates
class CDateTimeMessage 
{
public:
	static enum 
	{
		FORMAT_DATE_TIME,
		FORMAT_DATE,
		FORMAT_TIME
	};

	CDateTimeMessage (const char* callback, const char* title, long initial_time, int format, const char* data)
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
	char *m_data;
	long  m_initialTime;
	int   m_format;
};

class CDateTimePickerDialog : public CDialogImpl<CDateTimePickerDialog>
{
public:
	CDateTimePickerDialog  (const CDateTimeMessage *msg);
	~CDateTimePickerDialog ();

	time_t GetTime();

	enum { IDD = IDD_TIMEPICKER };
	
	BEGIN_MSG_MAP(CDateTimePickerDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	int         m_format;
	time_t      m_returnTime;
	time_t		m_initialTime;
	const char *m_title;
};

class CDatePickerDialog : public CDialogImpl<CDatePickerDialog>
{
public:
	CDatePickerDialog  (const CDateTimeMessage *msg);
	~CDatePickerDialog ();

	time_t GetTime();

	enum { IDD = IDD_DATEPICKER };
	
	BEGIN_MSG_MAP(CDateTimePickerDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	int         m_format;
	time_t      m_returnTime;
	time_t		m_initialTime;
	const char *m_title;
};


extern "C" void  choose_datetime(char* callback, char* title, 
								 long initial_time, int format, char* data);

#endif //__DATETIMEPICKER_H__
