#include <windows.h>
#include "ViewFinder.h"

extern "C" HINSTANCE rho_wmimpl_get_appinstance();

#define CMD_CANCEL	0x6001		///< HMENU id for the cancel button
#define CMD_CAPTURE	0x6002		///< HMENU id for the capture button


CViewFinder::CViewFinder()
{
	m_pCallBack = NULL;	
	m_hWndViewer = NULL;
	m_hWndViewerParent =NULL;
	eMode = eConfigurable;

}
void CViewFinder::RegisterCallBack(IViewFinderCallBack* pCallBack)
{
	m_pCallBack = pCallBack;
}
HWND CViewFinder::CreateViewerWindow(HWND hwndParent,RECT& pos, ViewrWndMode eMode)
{	
	if(m_hWndViewer != NULL)
	{
		DestroyViewerWindow();
	}
	if(NULL != hwndParent)
	{
		m_hWndViewerParent = hwndParent;
		HINSTANCE hInstance = rho_wmimpl_get_appinstance();
		if(eFullScreen == eMode)
		{
			m_iH = GetSystemMetrics(SM_CYSCREEN);
			int iXCoord = GetSystemMetrics(SM_CXSCREEN);
			float iRatio;
			if (iXCoord > m_iH)
				iRatio = (float)iXCoord / (float)m_iH;
			else
				iRatio = (float)m_iH / (float)iXCoord;

			//  Only currently considered ratios are 1:1.333333 and 1:1.66666
			int iSmallSide = 240;
			int iLongSide = 320;
			if (iRatio < 1.7f && iRatio >1.6f)
			{
				//  Screen is 1.666666 ratio
				iSmallSide = 240;
				iLongSide = 400;
			}

			if (iXCoord > m_iH)
			{
				//  The device is in landscape mode
				m_iX = iLongSide;
				m_iY = iSmallSide;
				m_bIsLandscape = true;
			}
			else
			{
				m_iX = iSmallSide;
				m_iY = iLongSide;
			}

			int iLeftPos = (GetSystemMetrics(SM_CXSCREEN)/2) - scaledpx(m_iX/2);
			int iTopPos = (GetSystemMetrics(SM_CYSCREEN)/2) - scaledpx(m_iY/2);

			HWND m_hwnd = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"Dialog", L"", WS_VISIBLE ,
				iLeftPos, iTopPos, scaledpx(m_iX), scaledpx(m_iY), hwndParent, NULL, hInstance, NULL);

			if (m_hwnd)
			{	
				m_hWndViewerParent = m_hwnd;	
				eMode = eFullScreen;
				//set viewer window position in case of full screen
				pos.left = 0;
				pos.top = scaledpx(scaleForY(20));
				pos.right = GetSystemMetrics(SM_CXSCREEN);
				pos.bottom =  scaledpx(scaleForY(228));

				//create child controls
				HWND m_hwndCancel = CreateWindowEx(WS_EX_NOANIMATION,_T("Button"), L"Cancel", 
					WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE , 
					scaledpx(scaleForMargain(180)), scaledpx(scaleForY(240)), scaledpx(55), scaledpx(scaleForY(22)), m_hwnd, (HMENU)CMD_CANCEL, hInstance, NULL);
				HWND m_hwndCapture = CreateWindowEx(WS_EX_NOANIMATION,_T("Button"), L"Capture", 
					WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE , 
					scaledpx(scaleForMargain(120)), scaledpx(scaleForY(240)), scaledpx(55), scaledpx(scaleForY(22)), m_hwnd, (HMENU)CMD_CAPTURE, hInstance, NULL);

			}

		}
		else
		{
			eMode = eConfigurable;
		}
		if(m_hWndViewerParent)
		{
			// Create the viewer window
			m_hWndViewer = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"static", NULL, WS_VISIBLE | WS_POPUP, 
				pos.left,
				pos.top,
				pos.right,
				pos.bottom, 
				m_hWndViewerParent, NULL, hInstance, NULL);
		}

	}
	return m_hWndViewer;
}
void CViewFinder::RepositionWindow(const RECT& pos)
{
	if (IsWindow(m_hWndViewer))
	{
		MoveWindow(m_hWndViewer,
			pos.left,
			pos.top,
			pos.right,
			pos.bottom, 
			FALSE);
	}
}
void CViewFinder::DestroyViewerWindow()
{
	//destroy viwerwnd
	if (IsWindow(m_hWndViewer))
	{
		DestroyWindow(m_hWndViewer);		
	}
	m_hWndViewer = NULL;
	//if parent wnd was created during full screen mode, then kill fulscreen window as well
	if(eMode == eConfigurable)
	{
		if (IsWindow(m_hWndViewerParent))
		{
			DestroyWindow(m_hWndViewerParent);
		}
		m_hWndViewerParent = NULL;


	}	

}
LRESULT CALLBACK CViewFinder::FullScreenWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
int CViewFinder::scaledpx( int size)
{
	int retSize = (int)(m_iH/((double)m_iY/(double)size));
	return retSize;
}

int CViewFinder::scaleForY( int size)
{
	if (m_bIsLandscape)
		return (int)((float)size / 1.4);
	else
		return size;
}
int CViewFinder::scaleForMargain(int size)
{
	if (m_bIsLandscape)
		return size + 30;
	else
		return size;
}

