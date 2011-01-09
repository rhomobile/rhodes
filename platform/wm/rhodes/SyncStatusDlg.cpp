#include "StdAfx.h"

#include "SyncStatusDlg.h"
#include "logging/RhoLog.h"

LRESULT CSyncStatusDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    SetDlgItemText(IDC_SYNCSTATUS, m_strStatus );

    SendMessage(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strTitle );
	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

void CSyncStatusDlg::setStatusText(LPCWSTR szStatus)
{
    m_strStatus = szStatus;

    if ( m_hWnd )
        SetDlgItemText(IDC_SYNCSTATUS, m_strStatus );
}

LRESULT CSyncStatusDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return 0;
}

LRESULT CSyncStatusDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ShowWindow(SW_HIDE);
    bHandled = TRUE;
	return 0;
}
