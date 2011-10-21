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

#pragma once

#if !defined(_WIN32_WCE)
#define IDB_BACK 10
#define IDB_MENU 11

class CMenuBar: public CWindowImpl<CMenuBar,CWindow,CControlWinTraits>
{
public:
  DECLARE_WND_CLASS(TEXT("Rhodes.MenuBar"))

  BEGIN_MSG_MAP(CMenuBar)
	  MESSAGE_HANDLER(WM_CREATE, OnCreate)
	  MESSAGE_HANDLER(WM_SIZE, OnSize)
	  MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      COMMAND_ID_HANDLER(IDB_BACK, OnBackCommand)
	  COMMAND_ID_HANDLER(IDB_MENU, OnMenuCommand)
	  REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

//  CODButtonImpl m_btnLeft;
//  CODButtonImpl m_btnRight;
  CButton m_btnLeft;
  CButton m_btnRight;

private:
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
  LRESULT OnEraseBkgnd(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

  // WM_COMMAND handlers
  LRESULT OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

};

#endif //!defined(_WIN32_WCE)