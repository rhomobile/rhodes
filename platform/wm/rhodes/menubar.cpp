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

#if !defined(_WIN32_WCE)

#include "resource.h"
#include "menubar.h"
#include "common/RhodesApp.h"

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
    RHODESAPP().navigateBack();
    return 0;
}

LRESULT CMenuBar::OnMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::PostMessage(this->GetParent(),WM_COMMAND,IDM_POPUP_MENU,0);
    return 0;
}

#endif //!defined(_WIN32_WCE)