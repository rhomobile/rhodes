#pragma once

#include "stdafx.h"
#include "resource.h"

class CGetURLDialog : 
    public CDialogImpl<CGetURLDialog>
{
public:
    CGetURLDialog();
    ~CGetURLDialog();

    BSTR GetURL() const
    { return m_bstrURL; }

    enum { IDD = IDD_GETURLDIALOG };

    BEGIN_MSG_MAP(CGetURLDialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
        COMMAND_ID_HANDLER(IDOK, OnOKCommand)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancelCommand)
    END_MSG_MAP()

private:
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnOKCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancelCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
    BSTR m_bstrURL;
};


