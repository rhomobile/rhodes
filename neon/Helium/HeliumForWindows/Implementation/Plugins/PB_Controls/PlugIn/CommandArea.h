/******************************************************************************/
#pragma once

/******************************************************************************/
#include "Control.h"

/******************************************************************************/
class CCommandArea: public CControl
{
public:
	CCommandArea (HINSTANCE hinstance, HWND hparent, int instance_id, BOOL top);
	virtual ~CCommandArea ();

	virtual BOOL SetHeight (int height);
	virtual BOOL SetColour (LPCWSTR colour);
	virtual BOOL SetImage (LPCWSTR url);
	virtual BOOL SetBorder (BOOL show);

	virtual BOOL OnWindowChanged (int event) {return TRUE;}
	static void ResizeHTML ();

private:
	static BOOL bRegistered;
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static int StaticIMOCallback (PVARSTRUCT pvars, int id, LPARAM lparam);

	IMOREF imoImage;
	LPWSTR imoImageFilename;

	BOOL SetImageFromFile (LPCWSTR filename);

protected:
	virtual void Paint (HDC hdc);
	BOOL bBorder;
	HBRUSH hBrush;
	HDC hdcImage;
	HBITMAP hbmOriginalImage;

	static int nTopHeight, nBottomHeight;
	static BOOL bTopVisible, bBottomVisible;
};

/******************************************************************************/
class CTopCommandArea: public CCommandArea
{
public:
	CTopCommandArea (HINSTANCE hinstance, HWND hparent, int instance_id);

	virtual BOOL Show ();
	virtual BOOL Hide ();
	virtual BOOL SetHeight (int height);

	virtual BOOL SetLeft (int left) {return TRUE;}
	virtual BOOL SetRight (int right) {return TRUE;}
	virtual BOOL SetTop (int top) {return TRUE;}
	virtual BOOL SetWidth (int width) {return TRUE;}

	virtual BOOL OnWindowChanged (int event);

private:
	void Paint (HDC hdc);
};

/******************************************************************************/
class CBottomCommandArea: public CCommandArea
{
public:
	CBottomCommandArea (HINSTANCE hinstance, HWND hparent, int instance_id);

	virtual BOOL Show ();
	virtual BOOL Hide ();
	virtual BOOL SetHeight (int height);

	virtual BOOL SetLeft (int left) {return TRUE;}
	virtual BOOL SetRight (int right) {return TRUE;}
	virtual BOOL SetTop (int top) {return TRUE;}
	virtual BOOL SetWidth (int width) {return TRUE;}

	virtual BOOL OnWindowChanged (int event);

private:
	void Paint (HDC hdc);
};
