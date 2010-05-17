#ifndef __ALERT__H__
#define __ALERT__H__

#include <common/RhoStd.h>
#include <logging/RhoLog.h>

using namespace rho;
using namespace rho::common;

#define ID_ALERT_DLG_BUTTON_MAX   (8)
#define ID_ALERT_DLG_BUTTON_FIRST (1)
#define ID_ALERT_DLG_BUTTON_LAST  (ID_ALERT_DLG_BUTTON_FIRST + (ID_ALERT_DLG_BUTTON_MAX) - 1)

class CAlertDialog : public CIndirectDialogImpl<CAlertDialog, CMemDlgTemplate>
{
public:
	/**
	 * Params is a container to pass parameters to UI thread.
	 */
	class Params {
	public:
		enum {DLG_DEFAULT, DLG_CUSTOM};

		Params (String &message)
		{
			m_dlgType  = DLG_DEFAULT;
			m_message  = message;
		}

		Params (String &title, String &message, String &icon, String &callback, Hashtable<String, String> &buttons)
		{
			m_dlgType  = DLG_CUSTOM;
			m_title    = title;
			m_message  = message;
			m_icon     = icon;
			m_callback = callback;
			m_buttons  = buttons;
		}

		int    m_dlgType;
		String m_title;
		String m_message;
		String m_icon;
		String m_callback;
		Hashtable<String, String> m_buttons;
	};
	
	class CustomButton : public CButton {
	public:
		CustomButton () {}

		CustomButton (String title, String strId, int numId)
		{
			m_title = title;
			m_strId = strId;
			m_numId = numId;
		}
	public:
		String m_title;
		String m_strId;
		int	   m_numId;
	};

public:
	BEGIN_MSG_MAP(CAlertDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_RANGE_HANDLER(ID_ALERT_DLG_BUTTON_FIRST, ID_ALERT_DLG_BUTTON_LAST, OnAlertDialogButton)
	END_MSG_MAP()

	CAlertDialog(Params *msg);
	~CAlertDialog();

	void DoInitTemplate();
	void DoInitControls();

	bool findButton(int id, CustomButton &button);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&bHandled);
	LRESULT OnAlertDialogButton (WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	HICON loadIcon(void);

private:
	String m_title;
	String m_message;
	String m_callback;
	String m_icon;

	CStatic m_messageCtrl;
	CStatic m_iconCtrl;
	Vector<CustomButton> m_buttons;
};


class CAlert {
    DEFINE_LOGCLASS;
  public:
	static void showPopup(CAlertDialog::Params *params);
    static void vibrate();
    static void playFile(String fileName);
};

#endif // __ALERT__H__
