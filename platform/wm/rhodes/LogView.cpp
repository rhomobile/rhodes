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

#include "stdafx.h"
#include "LogView.h"
#include "LogOptionsDlg.h"

#include "logging/RhoLog.h"
#include "common/RhodesApp.h"

LRESULT CLogView::OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    if ( m_hWndCommandBar )
        ::DestroyWindow(m_hWndCommandBar);

    m_hWndCommandBar = 0;

	return FALSE;
}

LRESULT CLogView::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined( OS_WINCE )

//#if !defined (OS_PLATFORM_MOTCE)
	if(RHO_IS_WMDEVICE)
	{
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
		SHCreateMenuBar(&mbi);
	}
	//    SendDlgItemMessage(IDC_LOGEDIT,WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FONT),0);
	//#else
	else
	{
		SetWindowLong(GWL_STYLE,(long)WS_BORDER);
		ShowWindow(SW_MAXIMIZE);

		m_hWndCommandBar = CommandBar_Create(_AtlBaseModule.GetResourceInstance(), m_hWnd, 1);
		CommandBar_AddAdornments(m_hWndCommandBar, 0, 0 );
		HMENU menu = LoadMenu(0, MAKEINTRESOURCE(IDR_LOGMENUBAR));
		CommandBar_InsertMenubarEx(m_hWndCommandBar, 0, (LPTSTR)menu, 0);
		CommandBar_DrawMenuBar(m_hWndCommandBar, 0);
		CommandBar_Show(m_hWndCommandBar, TRUE);
	}
//#endif

    loadLogText();

#endif //OS_

	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

void CLogView::loadLogText(){
    rho::StringW strText;
    LOGCONF().getLogTextW(strText);
    //strText += L"\r\n";
    SetDlgItemText(IDC_LOGEDIT, strText.c_str());

    int nPos = LOGCONF().getLogTextPos();
    int nLine = SendDlgItemMessage(IDC_LOGEDIT,EM_LINEFROMCHAR,nPos,0);
    SendDlgItemMessage(IDC_LOGEDIT,EM_LINESCROLL,0,nLine);
    SendDlgItemMessage(IDC_LOGEDIT,EM_SETSEL,nPos,nPos);
}

LRESULT CLogView::OnRefresh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){
    loadLogText();
    Invalidate();

    bHandled = TRUE;
    return 0;
}

LRESULT CLogView::OnClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){

    LOGCONF().clearLog();
	loadLogText();
    Invalidate();
    bHandled = TRUE;
    return 0;
}

LRESULT CLogView::OnCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    HDC hdcStatic = (HDC) wParam; 
    HWND hwndStatic = (HWND) lParam;

    if ( !m_hBrush )
        m_hBrush = CreateSolidBrush(RGB(255,255,255));

    bHandled = TRUE;
    return (LRESULT)m_hBrush;
}

void CLogView::OnFinalMessage(HWND /*hWnd*/)
{
    if ( m_hBrush )
        DeleteObject(m_hBrush);

    m_hBrush = NULL;
}

LRESULT CLogView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::MoveWindow( GetDlgItem(IDC_LOGEDIT), 0,CommandBar_Height( m_hWndCommandBar ), LOWORD(lParam), HIWORD(lParam), TRUE );
    return 0;
}


#ifdef OS_WINCE
LRESULT CLogView::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
    bHandled = TRUE;
	return 0;
}

LRESULT CLogView::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
    bHandled = TRUE;
	return 0;
}

LRESULT CLogView::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
    bHandled = TRUE;
	return 0;
}
#endif

LRESULT CLogView::OnBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
    bHandled = TRUE;
	return 0;
}

LRESULT CLogView::OnOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    CLogOptionsDlg oLogOptions;
    oLogOptions.DoModal(m_hWnd);

	return 0;
}

LRESULT CLogView::OnSendLog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    rho_conf_send_log("");

	return 0;
}

// CResizableGrip
