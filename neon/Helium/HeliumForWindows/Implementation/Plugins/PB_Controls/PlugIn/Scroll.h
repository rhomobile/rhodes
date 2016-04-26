/******************************************************************************/
#pragma once

/******************************************************************************/
#include "Control.h"

/******************************************************************************/
class CScrollBar
{
public:
	CScrollBar (HINSTANCE hinstance, HWND hparent);
	virtual ~CScrollBar ();

	int nWidth, nHeight;
	BOOL bScrollInProgress;
	int nScrollPos;

	void Show (BOOL show) {ShowWindow (hWindow, show ? SW_SHOWNOACTIVATE : SW_HIDE);}
	void Resize (int width, int height);
	void SetInfo (int range, int page);
	void SetPosition (int pos);
	int MoveScrollThumb (int code, int thumb_pos);
	virtual void OnScroll (int code, int thumb_pos) {return;}

protected:
	HWND hWindow, hScroll;

	static BOOL bRegistered;
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};

/******************************************************************************/
class CVScrollBar: public CScrollBar
{
public:
	CVScrollBar (HINSTANCE hinstance, HWND hparent);
	void OnScroll (int code, int thumb_pos);
};

/******************************************************************************/
class CHScrollBar: public CScrollBar
{
public:
	CHScrollBar (HINSTANCE hinstance, HWND hparent);
	void OnScroll (int code, int thumb_pos);
};

/******************************************************************************/
class CScroll: public CControl
{
public:
	CScroll (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CScroll ();

	void OnScrollChange (WPARAM wparam, LPARAM lparam);

private:
	CVScrollBar *pVScrollBar;
	CHScrollBar *pHScrollBar;

	int nVExtent, nHExtent;
	int nVPos, nHPos;

	void OnExtentChange (int v_extent, int h_extent);
	void OnPositionChange (int v_pos, int h_pos);
};
