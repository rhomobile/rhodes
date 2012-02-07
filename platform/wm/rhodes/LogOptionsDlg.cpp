/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "LogOptionsDlg.h"

#include "logging/RhoLog.h"

LRESULT CLogOptionsDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined( OS_WINCE ) && !defined (OS_PLATFORM_MOTCE)
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
    SHCreateMenuBar(&mbi);
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
