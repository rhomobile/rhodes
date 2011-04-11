#pragma once
#include <msinkaut.h>

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
