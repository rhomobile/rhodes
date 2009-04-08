// LogView.cpp : Implementation of CLogView

#include "stdafx.h"
#include "LogView.h"

#include "logging/RhoLog.h"

LRESULT CLogView::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef OS_WINCE
	SHINITDLGINFO shidi;
	shidi.dwMask = SHIDIM_FLAGS; 
	shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;//SHIDIF_DONEBUTTON | SHIDIF_SIZEDLGFULLSCREEN |SHIDIF_EMPTYMENU; 
	shidi.hDlg = m_hWnd; 
	SHInitDialog(&shidi);

    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_LOGMENUBAR; // ID of toolbar resource
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    mbi.dwFlags    = SHCMBF_HMENU;
    RHO_ASSERT(SHCreateMenuBar(&mbi));

    rho::StringW strText;
    LOGCONF().getLogTextW(strText);
    SetDlgItemText(IDC_LOGEDIT, strText.c_str() );
#endif //OS_WINCE

	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

LRESULT CLogView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::MoveWindow( GetDlgItem(IDC_LOGEDIT), 0,0, LOWORD(lParam), HIWORD(lParam), TRUE );

    return 0;
}

LRESULT CLogView::OnBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CLogView::OnOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return 0;
}

// CLogView
