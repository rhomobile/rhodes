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
		enum {DLG_DEFAULT, DLG_CUSTOM, DLG_STATUS};
        struct CAlertButton
        {
            String m_strCaption, m_strID;
            CAlertButton ( const String& strCaption, const String& strID ): m_strCaption(strCaption), m_strID(strID){}

        };

		Params (String &message)
		{
			m_dlgType  = DLG_DEFAULT;
			m_message  = message;
		}

		Params (String &title, String &message, String &icon, String &callback, Vector<CAlertButton>& buttons, int dlgType)
		{
			m_dlgType  = dlgType;
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

		//Hashtable<String, String> m_buttons;
        Vector<CAlertButton> m_buttons;

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
    static void vibrate(int duration_ms);
    static void playFile(String fileName);
};

#endif // __ALERT__H__
