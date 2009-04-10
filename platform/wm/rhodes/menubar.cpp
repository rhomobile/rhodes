#include "stdafx.h"

#if !defined(_WIN32_WCE)

#include "resource.h"
#include "menubar.h"

/*
void CODButtonImpl::DrawItem ( LPDRAWITEMSTRUCT lpdis )
{
	CDCHandle dc = lpdis->hDC;
	CDC dcMem;

    dcMem.CreateCompatibleDC ( dc );
    dc.SaveDC();
    dcMem.SaveDC();

    // Draw the button's background, red if it has the focus, blue if not.
	if ( lpdis->itemState & ODS_FOCUS ) {
		dc.FillSolidRect ( &lpdis->rcItem, ::GetSysColor(COLOR_3DLIGHT) );
		dc.Draw3dRect(&lpdis->rcItem, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
	} else {
		dc.FillSolidRect ( &lpdis->rcItem, ::GetSysColor(COLOR_3DLIGHT) );
	}

	// Draw the bitmap in the top-left, or offset by 1 pixel if the button
    // is clicked.
    //dcMem.SelectBitmap ( m_bmp );

    //if ( lpdis->itemState & ODS_SELECTED ) 
    //    dc.BitBlt ( 1, 1, 80, 80, dcMem, 0, 0, SRCCOPY );
    //else
    //    dc.BitBlt ( 0, 0, 80, 80, dcMem, 0, 0, SRCCOPY );

    dcMem.RestoreDC(-1);
    dc.RestoreDC(-1);
}
*/

LRESULT CMenuBar::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
//	m_btnLeft.Create(m_hWnd,CWindow::rcDefault,TEXT("Exit"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,0,10);
//	m_btnRight.Create(m_hWnd,CWindow::rcDefault,TEXT("Menu"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,0,11);
	m_btnLeft.Create(m_hWnd,CWindow::rcDefault,TEXT("Back"),WS_CHILD|WS_VISIBLE,0,IDB_BACK);
	m_btnRight.Create(m_hWnd,CWindow::rcDefault,TEXT("Menu"),WS_CHILD|WS_VISIBLE,0,IDB_MENU);
	return 0;
}

LRESULT CMenuBar::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	if (m_btnLeft.m_hWnd && m_btnRight.m_hWnd) {
		int width = LOWORD(lParam)>>1;
		m_btnLeft.MoveWindow(1, 1, width-1, HIWORD(lParam)-2);
		m_btnRight.MoveWindow(width, 1, LOWORD(lParam)-width-1, HIWORD(lParam)-2);
	}
	return 0;
}

LRESULT CMenuBar::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
	HDC hDC  = (HDC)wParam;
	
	RECT rect;
	GetClientRect(&rect);

	COLORREF clrOld = ::SetBkColor(hDC, ::GetSysColor(COLOR_3DLIGHT));
	if(clrOld != CLR_INVALID) {
		RECT rect;
		::GetClientRect(m_hWnd,&rect);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		::SetBkColor(hDC, clrOld);
	}	

	bHandled = TRUE;
	return 0;
}

LRESULT CMenuBar::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::PostMessage(this->GetParent(),WM_COMMAND,IDM_BACK,0);
    return 0;
}

LRESULT CMenuBar::OnMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::PostMessage(this->GetParent(),WM_COMMAND,IDM_POPUP_MENU,0);
    return 0;
}

#endif //!defined(_WIN32_WCE)