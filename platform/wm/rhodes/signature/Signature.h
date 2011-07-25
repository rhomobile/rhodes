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
#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_CE )
#include <msinkaut.h>
#else
struct IInkOverlay{};
#endif

class CRhoTakeSignatureDlg : public 
	CDialogImpl <CRhoTakeSignatureDlg>
{
public:
	CRhoTakeSignatureDlg();
	~CRhoTakeSignatureDlg();
	
	enum { IDD = IDD_TAKE_SIGNATURE };

	BEGIN_MSG_MAP(CRhoTakeSignatureDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	IInkOverlay*        m_pInkOverlay;
};

class Signature
{
public:
	class Params {
	public:
		Params(String &callback_url, String &image_format){
			m_callback_url = callback_url;
			m_image_format = image_format;
		}
		String m_callback_url;
		String m_image_format;
	};
public:
	Signature(void);
	virtual ~Signature(void);

public: 
	HRESULT takeSignature(HWND hwndOwner,LPTSTR pszFilename,LPCTSTR szFormat);

private:
	HBITMAP getScreenHBITMAP();
private:
	CRhoTakeSignatureDlg dlg;
};

extern "C" void rho_signature_take_signature(char* callback_url, char* image_format);
