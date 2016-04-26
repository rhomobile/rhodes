#pragma once

#include "Control.h"

//#define KSI_MAX_INDICATORS	4
#define KSI_MAX_INDICATORS	7 //  This value was previously 4 but KW gave supurious error.

#define	ORANGE				0x0010
#define GRAY				0x0020
#define BLUE				0x0080
#define LOCKED				0x8000
#define KEY_EVENT				(WM_USER + 101)
#define KEYBOARD_EVENT			(WM_USER + 102)

class CKeyState: public CControl
{
public:
	CKeyState (HINSTANCE hinstance, HWND hparent);
	virtual ~CKeyState ();
	void init();

	void OnKeyStateChange (DWORD state);
	BOOL SetLeft (int left);
	BOOL SetRight (int right);
	BOOL SetHeight (int height);
	BOOL SetWidth (int width);
	BOOL SetBorder (BOOL show);
	bool GetIconShown() {return m_bIconShown;}
	void SetIconShown(bool bIconShown) {m_bIconShown = bIconShown;}
private:
	static BOOL bRegistered;
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	virtual void Paint (HDC hdc);
	HBITMAP LoadScaledImage (LPCWSTR filename);
	void LoadImages (void);
	static LRESULT CALLBACK StaticKeyWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	int nRight;
	int nIndicators;
	HBITMAP hbmIndicators [KSI_MAX_INDICATORS];
	int nIndicatorWidth, nIndicatorHeight;
	HBITMAP hbmShift, hbmShiftLock, hbmControl, hbmAlt, hbmBlue, hbmBlueLock, hbmOrange, hbmOrangeLock, hbmGray;
	BOOL bBorder;
	HINSTANCE m_hInstance;
	HWND hKeyStateWindow;
	HMODULE hKeyboardDriver;
	bool m_bIconShown;
};
