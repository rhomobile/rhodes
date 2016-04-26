#include "resource3.h"
#include "HourglassAnimation.h"

#define ID_ANIMATIONTIMER	0x8001
#define MODULE_NAME L"WTG_DeviceApplication_PLG.dll"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
CHourglassAnimation::CHourglassAnimation(HINSTANCE hInstance, 
										 HWND hParent,
										 HANDLE hWindowInitialisedEvent,
										 DWORD dwAnimationInterval,
										 DWORD dwStartX,
										 DWORD dwStartY)
	:m_dwTimerInterval(dwAnimationInterval)
	,m_hInstance(hInstance)
	,m_hParent(hParent)
	,m_hWindowInitialisedEvent(hWindowInitialisedEvent)
	,m_dwStartX(dwStartX)
	,m_dwStartY(dwStartY)
	,m_iVisibleFrame(0)
	,m_hwndAnimation(NULL)
	,m_bAnimationRunning(FALSE)
	,m_lpfnAnimationProc(NULL)
	,m_bModuleIsForeground(TRUE)
	,m_bRequestedToShow(FALSE)
	,m_hAnimationThread(NULL)
	
{
	m_hBitmap = LoadBitmap(GetModuleHandle(MODULE_NAME), 
							MAKEINTRESOURCE(IDB_HOURGLASS));
	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	StartThread();
	
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
CHourglassAnimation::~CHourglassAnimation()
{
	SetEvent(m_hExit);
	PostMessage(m_hwndAnimation, WM_DESTROY, 0, 0);
//	if (WaitForSingleObject (m_hAnimationThread, 250) == WAIT_TIMEOUT)
//	{
//		//  Terminate Thread if it didn't shut down within a quarter of a second
//		TerminateThread(m_hAnimationThread, 0);
//	}
	DeleteObject(m_hBitmap);
	m_hBitmap=NULL;
	m_hwndAnimation=NULL;
	m_lpfnAnimationProc=NULL;
	CloseHandle(m_hExit);
	m_hExit = NULL;
	CloseHandle(m_hAnimationThread);
	m_hAnimationThread = NULL;
	
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
VOID CHourglassAnimation::StartThread()
{
	m_hAnimationThread = CreateThread(NULL, 0, AnimationThreadProc, this, 0, NULL);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
* \date		March 2010	  
*/
BOOL CHourglassAnimation::MoveHourglassLeft(int iLeft)
{
	m_dwStartX = iLeft;
	bool bShowAfterMove = false;
	if (IsWindowVisible(m_hwndAnimation))
	{
		bShowAfterMove = true;
		RequestToShow(FALSE);
	}
	RECT rc;
	GetWindowRect(m_hwndAnimation, &rc);
	int iTemp = rc.right - rc.left;
	rc.left = iLeft;
	rc.right = rc.left + iLeft;

	MoveWindow(m_hwndAnimation, rc.left, rc.top, 
				rc.right - rc.left, rc.bottom-rc.top, TRUE);
	if (bShowAfterMove)
		RequestToShow(TRUE);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
* \date		March 2010	 
*/
BOOL CHourglassAnimation::MoveHourglassTop(int iTop)
{
	m_dwStartY = iTop;
	bool bShowAfterMove = false;
	if (IsWindowVisible(m_hwndAnimation))
	{
		bShowAfterMove = true;
		RequestToShow(FALSE);
	}
	RECT rc;
	GetWindowRect(m_hwndAnimation, &rc);
	int iTemp = rc.bottom - rc.top;
	rc.top = iTop;
	rc.bottom = rc.top + iTop;

	MoveWindow(m_hwndAnimation, rc.left, rc.top, 
				rc.right - rc.left, rc.bottom-rc.top, TRUE);

	if (bShowAfterMove)
		RequestToShow(TRUE);
	return true;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
BOOL CHourglassAnimation::SetFocus(BOOL bCurrentlyHasFocus)
{
	m_bModuleIsForeground = bCurrentlyHasFocus;
	//  If we have moved to the background stop the animation if it is 
	//  currently running
	if (!bCurrentlyHasFocus)
	{
		//  Background
		if (m_bAnimationRunning)
			StopAnimation();
	}
	else
	{
		//  We have gained focus
		//  Test whether or not we should be showning the hour glass
		if (m_bRequestedToShow)
		{
			//  We have previously been requested to shown the animation
			if (!m_bAnimationRunning)
				StartAnimation();
		}
		else
		{
			//  We have previously been requested to not show the animation
			//  It should not be running but test and stop it here anyway.
			if (m_bAnimationRunning)
				StopAnimation();
		}
	}
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
BOOL CHourglassAnimation::RequestToShow(BOOL bHourglassShouldBeShown)
{
	m_bRequestedToShow = bHourglassShouldBeShown;
	if (!bHourglassShouldBeShown)
	{
		//  Requested to hide the hourglass, stop the animation if it is 
		//  currently running
		if (m_bAnimationRunning)
			return StopAnimation();
	}
	else
	{
		//  Requested to Show the Hourglass
		if (m_bModuleIsForeground)
		{
			//  Hourglass is requested to be shown and we are the foreground
			//  window, start the animation if it is not already running
			if (!m_bAnimationRunning)
				return StartAnimation();
			else
				return FALSE;
		}
		else
		{
			//  Hourglass is requested to be shown and we are the background
			//  window.  No action.
		}
	}
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
BOOL CHourglassAnimation::StartAnimation()
{
	if (0 == SetTimer(m_hwndAnimation, ID_ANIMATIONTIMER, m_dwTimerInterval, NULL))
	{
		return FALSE;
	}

	RECT rc;
	GetWindowRect(m_hwndAnimation, &rc);
	//  Enlarge the Animation Window
	MoveWindow(m_hwndAnimation, m_dwStartX, m_dwStartY/*rc.left, rc.top*/, ANIMATIONWIDTH, ANIMATIONHEIGHT, FALSE);
	SetWindowPos(m_hwndAnimation, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	//  Without the line below the hourglass failed to 
	//   show on some of the devices 
	ShowWindow(m_hwndAnimation, SW_SHOW);

	//  Notify the Paint method to paint the window
	LONG dwVal = TRUE;
	InterlockedExchange(&m_bAnimationRunning, dwVal);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
BOOL CHourglassAnimation::StopAnimation()
{
	//  Tell the Paint method to stop painting the window
	LONG lVal = 0;
	InterlockedExchange(&m_bAnimationRunning, lVal);
	InterlockedExchange(&m_iVisibleFrame, lVal);
	RECT rc;
	GetWindowRect(m_hwndAnimation, &rc);

	//  Shrink the Animation Window to 0 pixels
	MoveWindow(m_hwndAnimation,  rc.left, rc.top, 0, 0, FALSE);
	SetWindowPos(m_hwndAnimation, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_HIDEWINDOW);
	ShowWindow(m_hwndAnimation, SW_HIDE);

	KillTimer(m_hwndAnimation, ID_ANIMATIONTIMER);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
DWORD CHourglassAnimation::Run(DWORD dwDefaultX, DWORD dwDefaultY)
{
	m_hwndAnimation = CreateWindowEx(WS_EX_TOPMOST|WS_EX_NOANIMATION, _T("STATIC"), 
					NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED,
					dwDefaultX, dwDefaultY, 0,0, m_hParent, NULL,
					m_hInstance, NULL);
	
	SetEvent(m_hWindowInitialisedEvent);
	if (!m_hwndAnimation)
		return FALSE;
	
	SetWindowLong(m_hwndAnimation, GWL_USERDATA, (DWORD)this);
	m_lpfnAnimationProc = (WNDPROC )SetWindowLong(m_hwndAnimation, 
													GWL_WNDPROC, 
													(LONG )AnimationWndProc);

	MSG msg;
	while(WaitForSingleObject(m_hExit, 0) == WAIT_TIMEOUT)
	{
		GetMessage(&msg, m_hwndAnimation, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(m_hwndAnimation, ID_ANIMATIONTIMER);

	return 0;
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
LRESULT CALLBACK CHourglassAnimation::AnimationWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CHourglassAnimation *pthrdAnimation = (CHourglassAnimation*)GetWindowLong(hwnd, GWL_USERDATA);

	switch(message)
	{
	case WM_TIMER:
		pthrdAnimation->Paint();
		return 0L;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	}	
	return CallWindowProc(pthrdAnimation->m_lpfnAnimationProc, hwnd, message, wParam, lParam);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
VOID CHourglassAnimation::Paint()
{
	HDC hDC = GetDC(m_hwndAnimation);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBmpOld = (HBITMAP )SelectObject(hMemDC, m_hBitmap);
	BitBlt(hDC, 0, 0, ANIMATIONWIDTH, ANIMATIONHEIGHT,
					hMemDC, 0, m_iVisibleFrame * ANIMATIONHEIGHT, SRCCOPY);
	SelectObject(hMemDC, hBmpOld);
	DeleteDC(hMemDC);
	ReleaseDC(m_hwndAnimation, hDC);
	if (m_bAnimationRunning)
	{
		InterlockedIncrement(&m_iVisibleFrame);
		if(m_iVisibleFrame == MAXFRAMES)
		{
			LONG lVal = 0;
			InterlockedExchange(&m_iVisibleFrame, lVal);
		}
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Hourglass functionality)
*/
DWORD WINAPI CHourglassAnimation::AnimationThreadProc(LPVOID lpParam)
{
	CHourglassAnimation *pHourglass = (CHourglassAnimation *)lpParam;
	return pHourglass->Run(pHourglass->m_dwStartX, pHourglass->m_dwStartY);
}
