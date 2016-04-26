/******************************************************************************/
#pragma once

/******************************************************************************/
#include "Control.h"

/******************************************************************************/
#define KSI_MAX_INDICATORS	4

#define KSI_SHIFT			L"Shift.bmp"
#define KSI_SHIFT_LOCK		L"ShiftLock.bmp"
#define KSI_CONTROL			L"CtrlButton.bmp"
#define KSI_ALT				L"AltButton.bmp"
#define KSI_BLUE			L"BlueDoughnut.bmp"
#define KSI_BLUE_LOCK		L"BlueFill.bmp"
#define KSI_ORANGE			L"OrangeDoughnut.bmp"
#define KSI_ORANGE_LOCK		L"OrangeFill.bmp"
#define KSI_GRAY			L"GrayDoughnut.bmp"

#define	ORANGE				0x0010
#define GRAY				0x0020
#define BLUE				0x0080
#define LOCKED				0x8000

/******************************************************************************/
class CKeyState: public CControl
{
public:
	CKeyState (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CKeyState ();

	void OnKeyStateChange (DWORD state);
	BOOL SetLeft (int left);
	BOOL SetRight (int right);
	BOOL SetHeight (int height);
	BOOL SetWidth (int width);
	BOOL SetBorder (BOOL show);

private:
	static BOOL bRegistered;
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	virtual void Paint (HDC hdc);
	HBITMAP LoadScaledImage (LPCWSTR filename);
	void LoadImages (void);

	int nRight;
	int nIndicators;
	HBITMAP hbmIndicators [KSI_MAX_INDICATORS];
	int nIndicatorWidth, nIndicatorHeight;
	HBITMAP hbmShift, hbmShiftLock, hbmControl, hbmAlt, hbmBlue, hbmBlueLock, hbmOrange, hbmOrangeLock, hbmGray;
	BOOL bBorder;
};
