// LogView.h : Declaration of the CLogView

#pragma once

#include "resource.h"
// CLogView

class CLogView : 
	public CDialogImpl<CLogView>
{
public:
	CLogView(){}
	~CLogView(){}

	enum { IDD = IDD_LOGVIEW };

BEGIN_MSG_MAP(CLogView)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    COMMAND_ID_HANDLER(IDM_BACK, OnBack)
    COMMAND_ID_HANDLER(IDM_OPTIONS, OnOptions)
END_MSG_MAP()
//	CHAIN_MSG_MAP(CAxDialogImpl<CLogView>)

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

#if 0 //defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DRA::RelayoutDialog(
			AtlGetThisModuleHandle(),
			m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_LOGVIEW_WIDE) : MAKEINTRESOURCE(IDD_LOGVIEW));
		return 0;
	}
#endif
};


