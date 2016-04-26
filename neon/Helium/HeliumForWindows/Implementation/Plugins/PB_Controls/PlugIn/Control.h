/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "..\..\..\Common\Private\SIP.h"

/******************************************************************************/
typedef HBITMAP (*PLOADIMAGEPROC) (LPCWSTR);

/******************************************************************************/
/******************************************************************************/
class CControl
{
public:
	CControl (int instance_id, HWND hparent);
	virtual ~CControl ();

	virtual BOOL Show ();
	virtual BOOL Hide ();

	virtual BOOL SetLeft (int left) {nLeft = left; MoveControl (); return TRUE;}
	virtual BOOL SetRight (int right) {return TRUE;}
	virtual BOOL SetTop (int top) {nTop = top; MoveControl (); return TRUE;}
	virtual BOOL SetWidth (int width) {nWidth = width; MoveControl (); return TRUE;}
	virtual BOOL SetHeight (int height) {nHeight = height; MoveControl (); return TRUE;}
	virtual BOOL SetImage (LPCWSTR url) {return TRUE;}
	virtual BOOL SetImageUp (LPCWSTR url) {return TRUE;}
	virtual BOOL SetImageDown (LPCWSTR url) {return TRUE;}
	virtual BOOL SetColour (LPCWSTR colour) {return TRUE;}
	virtual BOOL SetBorder (BOOL show);
	virtual BOOL SetClick (LPCWSTR navigate) {return TRUE;}
	virtual BOOL SetMiscValue (LPCWSTR pparameter, LPCWSTR pvalue) {return FALSE;}

	BOOL SetID (LPCWSTR id);
	LPCWSTR GetID (void) {return psID;}

	HWND GetWindow () {return hWindow;}
	BOOL IsVisible (void) {return IsWindowVisible (hWindow);}

protected:
	LPWSTR psID;
	int nInstanceID;
	HWND hwndTopLevel, hWindow;
	int nLeft, nTop, nWidth, nHeight;
	static CSIP *pSIPControl;

	static HBITMAP LoadImageFile (LPCWSTR filename);
	void ScaleImage (HBITMAP hbm_original, HDC hdc_scaled);
	static BOOL StringToColour (LPCWSTR string, COLORREF *pcolour);

	virtual void MoveControl ();

private:
	static int nUsageCount;
	static HMODULE hImageLibrary;
	static PLOADIMAGEPROC pLoadImageProc;
	static HMODULE hZlib;
	static HMODULE hImaging;
};

