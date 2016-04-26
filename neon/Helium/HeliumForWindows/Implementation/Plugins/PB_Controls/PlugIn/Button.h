/******************************************************************************/
#pragma once

/******************************************************************************/
#include "Control.h"

/******************************************************************************/
class CAddressBar;

/******************************************************************************/

static TCHAR* tcCustomButtonEventNames[] = 
{
	NULL
};


class CButton: public CControl
{
public:
	CButton (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CButton ();

	virtual BOOL SetWidth (int width);
	virtual BOOL SetHeight (int height);
	virtual BOOL SetImageUp (LPCWSTR url);
	virtual BOOL SetImageDown (LPCWSTR url);
	virtual BOOL SetBorder (BOOL show);

private:
	static BOOL bRegistered;
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static int StaticIMOCallbackUp (PVARSTRUCT pvars, int id, LPARAM lparam);
	static int StaticIMOCallbackDown (PVARSTRUCT pvars, int id, LPARAM lparam);

	virtual void OnClick () {return;}

	HDC hdcImageUp, hdcImageDown;
	HBITMAP hbmOriginalImageUp, hbmOriginalImageDown;
	BOOL bButtonDown, bCapturing;
	IMOREF imoImageUp, imoImageDown;
	LPWSTR imoImageUpFilename, imoImageDownFilename;

	BOOL SetImageUpFromFile (LPCWSTR filename);
	BOOL SetImageDownFromFile (LPCWSTR filename);

protected:
	void LoadDefaultImages (LPCWSTR image_up, LPCWSTR image_down);
	virtual void Paint (HDC hdc);
};

/******************************************************************************/
class CQuitButton: public CButton
{
public:
	CQuitButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CReloadButton: public CButton
{
public:
	CReloadButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CBackButton: public CButton
{
public:
	CBackButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CForwardButton: public CButton
{
public:
	CForwardButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CGoButton: public CButton
{
public:
	CGoButton (HINSTANCE hinstance, HWND hparent, int instance_id);
	void SetAddressBar (CAddressBar *paddress_bar) {pAddressBar = paddress_bar;}

	virtual BOOL Show ();
	virtual BOOL Hide ();

	void Display (BOOL visible);

private:
	void OnClick ();
	CAddressBar *pAddressBar;
	BOOL bEnabled;
};

/******************************************************************************/
class CHomeButton: public CButton
{
public:
	CHomeButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CMinimizeButton: public CButton
{
public:
	CMinimizeButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CPageZoomInButton: public CButton
{
public:
	CPageZoomInButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CPageZoomOutButton: public CButton
{
public:
	CPageZoomOutButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CSIPButton: public CButton
{
public:
	CSIPButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CStopButton: public CButton
{
public:
	CStopButton (HINSTANCE hinstance, HWND hparent, int instance_id);

	virtual BOOL Show ();
	virtual BOOL Hide ();

	void Display (BOOL visible);

private:
	BOOL bEnabled;
	void OnClick ();
};

/******************************************************************************/
class CZoomTextButton: public CButton
{
public:
	CZoomTextButton (HINSTANCE hinstance, HWND hparent, int instance_id);

private:
	void OnClick ();
};

/******************************************************************************/
class CCustomButton: public CButton
{
public:
	CCustomButton (HINSTANCE hinstance, HWND hparent, int instance_id);
	~CCustomButton ();

	BOOL SetClick (LPCWSTR navigate);

private:
	LPWSTR psNavigate;
	void OnClick ();
};
