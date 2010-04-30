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