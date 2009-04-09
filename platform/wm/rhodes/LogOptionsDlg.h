#pragma once

#include "resource.h"
// CLogView

class CLogOptionsDlg : 
	public CDialogImpl<CLogOptionsDlg>
{
public:
    CLogOptionsDlg(){}
	~CLogOptionsDlg(){}

	enum { IDD = IDD_LOGOPTIONS };

BEGIN_MSG_MAP(CLogOptionsDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
    void saveOptions();
};


