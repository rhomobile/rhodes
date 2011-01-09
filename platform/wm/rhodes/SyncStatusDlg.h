#pragma once

#include "resource.h"

class CSyncStatusDlg : 
	public CDialogImpl<CSyncStatusDlg>
{
    CStringW m_strStatus, m_strTitle;
public:
    CSyncStatusDlg(){}
	~CSyncStatusDlg(){}

	enum { IDD = IDD_SYNCSTATUS };

BEGIN_MSG_MAP(CSyncStatusDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    void setStatusText(LPCWSTR szStatus);
    void setTitle(LPCWSTR szTitle){ m_strTitle = szTitle; }
};


