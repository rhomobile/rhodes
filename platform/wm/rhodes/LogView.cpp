// LogView.cpp : Implementation of CLogView

#include "stdafx.h"
#if defined(OS_WINDOWS)
#include <richedit.h>
#include "common/RhoConf.h"
#endif
#include "LogView.h"
#include "LogOptionsDlg.h"

#include "logging/RhoLog.h"
#include "common/RhodesApp.h"

#if defined(OS_WINDOWS)
rho::common::CMutex CLogView::m_ViewFlushLock;
#endif

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

//    SendDlgItemMessage(IDC_LOGEDIT,WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FONT),0);

    loadLogText();

#elif defined(OS_WINDOWS)
	m_grip.InitGrip(m_hWnd);
	m_grip.ShowSizeGrip();
	CWindow edit = GetDlgItem(IDC_LOGEDIT);
	::SendMessage(edit.m_hWnd,EM_EXLIMITTEXT,0,2147483647);
	::SendMessage(edit.m_hWnd, EM_SETEVENTMASK, 0, ENM_MOUSEEVENTS);
	SetTimer(100,1000,NULL);

	RECT rc = { 0,0,500,400 };
	rc.left = getIniInt(_T("log_view_left"),0);
	rc.top = getIniInt(_T("log_view_top"),0);
	int width = getIniInt(_T("log_view_width"),500);
	rc.right = rc.left+width;
	int height = getIniInt(_T("log_view_height"),400);
	rc.bottom = rc.top+height;
	
	MoveWindow(&rc);
#endif //OS_

	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

#if defined(OS_WINDOWS)
void CLogView::writeLogMessage( rho::String& strMsg ) {
	rho::common::CMutexLock oLock(m_ViewFlushLock);
	m_buffer.AddTail(strMsg);
	if (m_buffer.GetCount()>10000) {
		m_buffer.RemoveHead();
	}
}
#endif

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
#if defined(OS_WINDOWS)
	SetDlgItemText(IDC_LOGEDIT,_T(""));
#else
	loadLogText();
    Invalidate();
#endif
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
#if defined(OS_WINDOWS)
	if (m_grip.GetSafeHwnd()) {
		m_grip.UpdateGripPos();
	}
    ::MoveWindow( GetDlgItem(IDC_LOGEDIT), 0,0, LOWORD(lParam), HIWORD(lParam), TRUE );
#else
    ::MoveWindow( GetDlgItem(IDC_LOGEDIT), 0,0, LOWORD(lParam), HIWORD(lParam), TRUE );
#endif
    return 0;
}

#if defined(OS_WINDOWS)
LRESULT CLogView::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
  LPRECT pRect = (LPRECT) lParam;

  // min width is 150 pixels
  int nWidth = pRect->right - pRect->left;
  if ( nWidth < 150 ) pRect->right = pRect->left + 150;

  // min height is 150 pixels
  int nHeight = pRect->bottom - pRect->top;
  if ( nHeight < 150 ) pRect->bottom = pRect->top + 150;

  return 0;
}

LRESULT CLogView::OnPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	LPWINDOWPOS lp = (LPWINDOWPOS)lParam;
	setIniInt(_T("log_view_left"),lp->x);
	setIniInt(_T("log_view_top"),lp->y);
	setIniInt(_T("log_view_width"),lp->cx);
	setIniInt(_T("log_view_height"),lp->cy);
	bHandled = FALSE;
	return 0;
}

LRESULT CLogView::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if(m_pFindDialog) {
		m_pFindDialog->DestroyWindow();
		m_pFindDialog = NULL;
	}
	ShowWindow(SW_HIDE);
    bHandled = TRUE;
    return 0;
}

LRESULT CLogView::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	rho::common::CMutexLock oLock(m_ViewFlushLock);
	CWindow edit = GetDlgItem(IDC_LOGEDIT);
	while(m_buffer.GetCount()) {
		int pos = ::SendMessage(edit.m_hWnd, WM_GETTEXTLENGTH, 0, 0L);
		::SendMessage(edit.m_hWnd,EM_SETSEL,pos,pos);
		::SendMessageA(edit.m_hWnd, EM_REPLACESEL, 0, (LPARAM) m_buffer.RemoveHead().c_str());
	}
	return 0;
}

LRESULT CLogView::OnCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SendDlgItemMessage(IDC_LOGEDIT,WM_COPY);
    bHandled = TRUE;
	return 0;
}

LRESULT CLogView::OnSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SendDlgItemMessage(IDC_LOGEDIT,EM_SETSEL,0,-1);
    bHandled = TRUE;
	return 0;
}

LRESULT CLogView::OnFind(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_pFindDialog == NULL) {
		m_pFindDialog = new CFindReplaceDialog();
		m_pFindDialog->Create(TRUE, m_findText.c_str(), NULL, m_findParams, this->m_hWnd);
	}
	m_pFindDialog->ShowWindow(SW_SHOW);
    bHandled = TRUE;
	return 0;
}

LRESULT CLogView::OnFindText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
    if (m_pFindDialog->IsTerminating()) {
        m_pFindDialog = NULL;
	} else {
		m_findText = m_pFindDialog->GetFindString();
		CWindow edit = GetDlgItem(IDC_LOGEDIT);
		CHARRANGE curr_sel = {0,-1};
		::SendMessage(edit.m_hWnd,EM_EXGETSEL,0,(LPARAM)&curr_sel);
		m_findParams = 0;
		m_findParams |= m_pFindDialog->MatchWholeWord() ? FR_WHOLEWORD : 0;
		m_findParams |= m_pFindDialog->MatchCase() ? FR_MATCHCASE : 0;
		FINDTEXTEX findtext = { {curr_sel.cpMin,0},m_pFindDialog->GetFindString() };
		if (m_pFindDialog->SearchDown()) {
			m_findParams |= FR_DOWN;
			findtext.chrg.cpMin = curr_sel.cpMax;
			findtext.chrg.cpMax = -1;
		} 
		int pos = ::SendMessage(edit.m_hWnd, EM_FINDTEXTEXW,m_findParams,(LPARAM)&findtext);
		if (pos!=-1) {
			int nLine = ::SendMessage(edit.m_hWnd,EM_LINEFROMCHAR,pos,0);
			::SendMessage(edit.m_hWnd,EM_LINESCROLL,0,nLine);
			::SendMessage(edit.m_hWnd,EM_SETSEL,pos,findtext.chrgText.cpMax);
		} else {
			::MessageBox(m_pFindDialog->m_hWnd,
				L"Can't find specifyed text",L"Find",MB_OK);
		}
	}
	bHandled = TRUE;
	return 0;
}

void CLogView::OnPopupMenuCommand() {
	CMenu menu;
	CMenu sub;
    POINT point;                                            
    ::GetCursorPos(&point); //where is the mouse?

	RECT  rect;

	GetWindowRect(&rect);

	VERIFY(menu.LoadMenu(IDR_LOGVIEW_MENU));
	sub.Attach(menu.GetSubMenu(0));
    sub.TrackPopupMenu(
            TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_VERNEGANIMATION, 
			point.x, 
			point.y,
			m_hWnd);
	sub.Detach();
}

LRESULT CLogView::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 
	if ( (wParam == IDC_LOGEDIT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)   
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN)) {
		OnPopupMenuCommand();
		bHandled = TRUE;
	} else {
		bHandled = FALSE;
	}
	return 0;
}

#endif  //OS_WINDOWS

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
    rho_conf_send_log();

	return 0;
}

// CResizableGrip
#if defined(OS_WINDOWS)
CResizableGrip::CResizableGrip()
{
	m_hParent = NULL;

	m_sizeGrip.cx = GetSystemMetrics(SM_CXVSCROLL);
	m_sizeGrip.cy = GetSystemMetrics(SM_CYHSCROLL);

	m_binitialrect = FALSE;
}

CResizableGrip::~CResizableGrip()
{
}

void CResizableGrip::UpdateGripPos()
{
	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)

	RECT rect;
	::GetClientRect(m_hParent,&rect);

	rect.left = rect.right - m_sizeGrip.cx;
	rect.top = rect.bottom - m_sizeGrip.cy;

	// must stay below other children
	::SetWindowPos(m_wndGrip,HWND_BOTTOM, rect.left, rect.top, 0, 0,
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREPOSITION);


	if ( ::IsZoomed(m_hParent) )
	{
		::EnableWindow(m_wndGrip, FALSE);
		ShowSizeGrip(FALSE);
	}
	else
	{
		::EnableWindow(m_wndGrip, TRUE);
		ShowSizeGrip(TRUE);
	}
}

void CResizableGrip::ShowSizeGrip(BOOL bShow)
{
	::ShowWindow(m_wndGrip, bShow ? SW_SHOW : SW_HIDE);
}


HWND CResizableGrip::GetSafeHwnd()
{
	return ::IsWindow(m_wndGrip) ? m_wndGrip : NULL;
}

#define RSZ_GRIP_OBJ	_T("ResizableGrip")

BOOL CResizableGrip::InitGrip(HWND hParent)
{
	m_hParent = hParent;

	RECT rect = { 0 , 0, m_sizeGrip.cx, m_sizeGrip.cy };

	m_wndGrip = ::CreateWindowEx(0, _T("SCROLLBAR"), 
								(LPTSTR)NULL, 
								WS_CHILD | WS_CLIPSIBLINGS | SBS_SIZEGRIP,
								rect.left, rect.top, 
								rect.right-rect.left,
								rect.bottom-rect.top,
								hParent,
								(HMENU)0,
								NULL,
								NULL);

	if (m_wndGrip)
	{
		// set a triangular window region
		HRGN rgnGrip, rgn;
		rgn = ::CreateRectRgn(0,0,1,1);
		rgnGrip = ::CreateRectRgnIndirect(&rect);

		for (int y=0; y<m_sizeGrip.cy; y++)
		{
			::SetRectRgn(rgn, 0, y, m_sizeGrip.cx-y, y+1);
			::CombineRgn(rgnGrip, rgnGrip, rgn, RGN_DIFF); 
		}
		::SetWindowRgn(m_wndGrip, rgnGrip, FALSE);

		// subclass control
		::SetProp(m_wndGrip, RSZ_GRIP_OBJ,
			(HANDLE)::GetWindowLong(m_wndGrip, GWL_WNDPROC));
		::SetWindowLong(m_wndGrip, GWL_WNDPROC, (LONG)GripWindowProc);

		// force dialog styles (RESIZABLE BORDER, NO FLICKERING)
		::SetWindowLong(hParent, GWL_STYLE, 
			::GetWindowLong(hParent, GWL_STYLE) | WS_THICKFRAME | WS_CLIPCHILDREN);
		

		// update pos
		UpdateGripPos();
		ShowSizeGrip();
	}

	return m_wndGrip!=NULL;
}

LRESULT CResizableGrip::GripWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldWndProc = (WNDPROC)::GetProp(hwnd, RSZ_GRIP_OBJ);

	switch (msg)
	{
	case WM_NCHITTEST:

		// choose proper cursor shape
		if (IsRTL(hwnd))
			return HTBOTTOMLEFT;
		else
			return HTBOTTOMRIGHT;

	case WM_DESTROY:
		
		// unsubclass
		::RemoveProp(hwnd, RSZ_GRIP_OBJ);
		::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)oldWndProc);

		break;
	}

	return ::CallWindowProc(oldWndProc, hwnd, msg, wParam, lParam);
}

LPCTSTR getIniPath() {
	static TCHAR _inipath[MAX_PATH];
	static bool path_loaded = false;
	if (!path_loaded) {
		int len = GetModuleFileName(NULL,_inipath,MAX_PATH);
		if( len == 0 ) {
			wcscpy(_inipath,_T("."));
		} else {
			while( !(_inipath[len] == '.') )
				len--;
			_inipath[len]=0;
			swprintf(_inipath,MAX_PATH,_T("%s.ini"),_inipath);
		}
		path_loaded = 1;
	}
	return _inipath;
}

int getIniInt(LPCTSTR lpKeyName, int nDefault) {
	return GetPrivateProfileInt( _T("properties"), 
		lpKeyName, nDefault, getIniPath());
}

void setIniInt(LPCTSTR lpKeyName, int nValue) {
	TCHAR value[128];
	_itow_s(nValue,value,128,10);
	WritePrivateProfileString( _T("properties"), 
		lpKeyName, value, getIniPath());
}

#endif //OS_WINDOWS