#include "StdAfx.h"
#include "LogOptionsDlg.h"

#include "logging/RhoLog.h"

LRESULT CLogOptionsDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef OS_WINCE
	SHINITDLGINFO shidi;
	shidi.dwMask = SHIDIM_FLAGS; 
	shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;//SHIDIF_DONEBUTTON | SHIDIF_SIZEDLGFULLSCREEN |SHIDIF_EMPTYMENU; 
	shidi.hDlg = m_hWnd; 
	SHInitDialog(&shidi);

    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_LOGOPTIONSMENUBAR; // ID of toolbar resource
    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
    mbi.dwFlags    = SHCMBF_HMENU;
    RHO_ASSERT(SHCreateMenuBar(&mbi));
#endif //OS_WINCE

    SendDlgItemMessage(IDC_CBXLEVELS,CB_ADDSTRING,0,  (LPARAM)_T("Trace"));
    SendDlgItemMessage(IDC_CBXLEVELS,CB_ADDSTRING,0,  (LPARAM)_T("Info"));
    SendDlgItemMessage(IDC_CBXLEVELS,CB_ADDSTRING,0,  (LPARAM)_T("Warning"));
    SendDlgItemMessage(IDC_CBXLEVELS,CB_ADDSTRING,0,  (LPARAM)_T("Error"));

    SendDlgItemMessage(IDC_CBXLEVELS,CB_SETCURSEL,LOGCONF().getMinSeverity(),0);

    CStringW strW(LOGCONF().getEnabledCategories().c_str());
    SetDlgItemText(IDC_MSGCLASSES, strW );
    strW = LOGCONF().getDisabledCategories().c_str();
    SetDlgItemText(IDC_MSGEXCLUDE, strW );

	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

void CLogOptionsDlg::saveOptions()
{
    int nLevel = SendDlgItemMessage(IDC_CBXLEVELS,CB_GETCURSEL,0,0);
    int nSize = SendDlgItemMessage( IDC_MSGCLASSES, WM_GETTEXTLENGTH, 0, 0 );
    CStringW strClasses; 
    GetDlgItemText(IDC_MSGCLASSES, strClasses.GetBuffer(nSize), nSize+1 );
    strClasses.ReleaseBuffer();

    nSize = SendDlgItemMessage( IDC_MSGEXCLUDE, WM_GETTEXTLENGTH, 0, 0 );
    CStringW strExcludes; 
    GetDlgItemText(IDC_MSGEXCLUDE, strExcludes.GetBuffer(nSize), nSize+1 );
    strExcludes.ReleaseBuffer();

    LOGCONF().setMinSeverity(nLevel);
    LOGCONF().setEnabledCategories( CStringA(strClasses));
    LOGCONF().setDisabledCategories( CStringA(strExcludes) );
    LOGCONF().saveToFile();
}

LRESULT CLogOptionsDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    saveOptions();
	EndDialog(IDOK);
	return 0;
}

LRESULT CLogOptionsDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return 0;
}
