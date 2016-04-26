/******************************************************************************/
#pragma once

/******************************************************************************/
#include "Control.h"

/******************************************************************************/
#define WM_ADDURL		(WM_APP + 1)

/******************************************************************************/

static TCHAR* tcAddressBarEventNames[] = {NULL};

class CAddressBar: public CControl
{
public:
	CAddressBar (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CAddressBar ();

	virtual BOOL SetWidth (int width);
	virtual BOOL SetHeight (int height);
	virtual BOOL SetBorder (BOOL show) {return TRUE;}

	void NavigateSelected ();
	void NavigateText ();
	void DeleteCharacter();
	void OnNavigate ();
	BOOL onPrimaryMessage (MSG *pmsg);

private:
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	static LRESULT CALLBACK StaticComboBoxProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	WNDPROC lpfnComboBoxProc;

	void RemoveEncoding (LPCWSTR psource, LPWSTR pdest);
	void AddURL (LPCWSTR purl);

	HWND hComboBox;
	int nOpenHeight;
	bool bGotFocus;
};
