/******************************************************************************/
#include <windows.h>
#include "ControlsModule.h"
#include "Scroll.h"

/******************************************************************************/
extern CControlsModule *pControlsModule;

/******************************************************************************/
#define SCROLL_CLASS_NAME		L"PB_SCROLL"

/******************************************************************************/
BOOL CScrollBar::bRegistered = FALSE;

/******************************************************************************/
/******************************************************************************/
CScrollBar::CScrollBar (HINSTANCE hinstance, HWND hparent)
{
	// Register class if needed
	if (!bRegistered)
	{
		WNDCLASS wndclass;
		memset (&wndclass, 0, sizeof wndclass);
		wndclass.lpfnWndProc = StaticWindowProc;
		wndclass.hInstance = hinstance;
		wndclass.lpszClassName = SCROLL_CLASS_NAME;
		RegisterClass (&wndclass);

		bRegistered = TRUE;
	}

	nScrollPos = -1;
	bScrollInProgress = FALSE;

	// Create containing window with zero size initially
	hWindow = CreateWindowEx (WS_EX_NOACTIVATE, SCROLL_CLASS_NAME, NULL, WS_POPUP, 
		0, 0, 0, 0, hparent, (HMENU) -1, hinstance, NULL);

	// Add a pointer to ourselves to the window
	SetWindowLong (hWindow, GWL_USERDATA, (LONG) this);
}

/******************************************************************************/
CScrollBar::~CScrollBar ()
{
}

/******************************************************************************/
LRESULT CALLBACK CScrollBar::StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CScrollBar *pthis;

	// Get a pointer to ourselves
	pthis = (CScrollBar*) GetWindowLong (hwnd, GWL_USERDATA);

	switch (message)
	{
		case WM_CREATE:
			return 0;

		case WM_SIZE:
			// Resize scroll control to match
			if (wparam != SIZE_MINIMIZED)
				MoveWindow (pthis->hScroll, 0, 0, LOWORD (lparam), HIWORD (lparam), FALSE);

			return 0;

		case WM_VSCROLL:
		case WM_HSCROLL:
			pthis->OnScroll ((int) LOWORD (wparam), (int) HIWORD (wparam));
			return 0;
	}

	return DefWindowProc (hwnd, message, wparam, lparam);
}

/******************************************************************************/
void CScrollBar::Resize (int width, int height)
{
	DEBUGMSG(TRUE, (L"%s, width=%d, height=%d\n", TEXT(__FUNCTION__), width, height));

	nWidth = width;
	nHeight = height;

	SetWindowPos (hWindow, NULL, 0, 0, nWidth, nHeight, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}

/******************************************************************************/
void CScrollBar::SetInfo (int range, int page)
{
	DEBUGMSG(TRUE, (L"%s, range=%d, page=%d\n", TEXT(__FUNCTION__), range, page));

	SCROLLINFO info;

	memset (&info, 0, sizeof info);
	info.cbSize = sizeof info;
	info.fMask = SIF_RANGE | SIF_PAGE;
	info.nMin = 0;
	info.nMax = range;
	info.nPage = page;
	SetScrollInfo (hScroll, SB_CTL, &info, TRUE);
}

/******************************************************************************/
void CScrollBar::SetPosition (int pos)
{
	DEBUGMSG(TRUE, (L"%s, position=%d\n", TEXT(__FUNCTION__), pos));

	SCROLLINFO info;

	memset (&info, 0, sizeof info);
	info.cbSize = sizeof info;
	info.fMask = SIF_POS;
	info.nPos = pos;
	SetScrollInfo (hScroll, SB_CTL, &info, TRUE);
}

/******************************************************************************/
int CScrollBar::MoveScrollThumb (int code, int thumb_pos)
{
	SCROLLINFO info;
	int new_pos;

	// Get current position and other scroll bar data
	memset (&info, 0, sizeof info);
	info.cbSize = sizeof info;
	info.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
	GetScrollInfo (hScroll, SB_CTL, &info);

	// Adjust according to scroll code
	switch (code)
	{
		case SB_LINEDOWN:
			new_pos = info.nPos + 1;
			break;

		case SB_LINEUP:
			new_pos = info.nPos - 1;
			break;

		case SB_PAGEDOWN:
			new_pos = info.nPos + 40;
			break;

		case SB_PAGEUP:
			new_pos = info.nPos - 40;
			break;

		case SB_THUMBTRACK:
			bScrollInProgress = TRUE;
			new_pos = thumb_pos;
			break;

		case SB_ENDSCROLL:
			bScrollInProgress = FALSE;
			return -1;

		default:
			return -1;
	}

	if (new_pos < info.nMin)
		new_pos = info.nMin;

	if (new_pos > info.nMax)
		new_pos = info.nMax;

	// Set new position if necessary
	if (new_pos != info.nPos)
	{
		// Remember new position
		nScrollPos = new_pos;

		// Move scroll bar
		info.fMask = SIF_POS;
		info.nPos = new_pos;
		SetScrollInfo (hScroll, SB_CTL, &info, TRUE);

		return new_pos;
	}
	else
	{
		// No change in position
		return -1;
	}
}

/******************************************************************************/
/******************************************************************************/
CVScrollBar::CVScrollBar (HINSTANCE hinstance, HWND hparent) :
	CScrollBar (hinstance, hparent)
{
	DEBUGMSG(TRUE, (L"%s\n", TEXT(__FUNCTION__)));

	// Create contained scroll control, size will be set to match container during container resize
	// Container created during base class constructor
	hScroll = CreateWindowEx (0, L"SCROLLBAR", NULL, WS_VISIBLE | WS_CHILD | SBS_VERT, 0, 0, 0, 0,
		hWindow, NULL, hinstance, NULL);

	// Set default size and position
	nWidth = GetSystemMetrics (SM_CXVSCROLL);
	nHeight = CControlsModule::nScreenHeight;
	MoveWindow (hWindow, CControlsModule::nScreenWidth - nWidth, 0, nWidth, nHeight, FALSE);
}

/******************************************************************************/
void CVScrollBar::OnScroll (int code, int thumb_pos)
{
	DEBUGMSG(TRUE, (L"%s, code=%d, thumb_pos=%d\n", TEXT(__FUNCTION__), code, thumb_pos));

	int new_pos;

	new_pos = MoveScrollThumb (code, thumb_pos);

	// Move HTML page if necessary
	if (new_pos != -1)
		(*CControlsModule::PBCoreStruct.pBrowserSetVScroll) (0, new_pos);
}

/******************************************************************************/
/******************************************************************************/
CHScrollBar::CHScrollBar (HINSTANCE hinstance, HWND hparent) :
	CScrollBar (hinstance, hparent)
{
	DEBUGMSG(TRUE, (L"%s\n", TEXT(__FUNCTION__)));

	// Create contained scroll control, size will be set to match container during container resize
	// Container created during base class constructor
	hScroll = CreateWindowEx (0, L"SCROLLBAR", NULL, WS_VISIBLE | WS_CHILD | SBS_HORZ, 0, 0, 0, 0,
		hWindow, NULL, hinstance, NULL);

	// Set default size and position
	nWidth = CControlsModule::nScreenWidth;
	nHeight = GetSystemMetrics (SM_CYHSCROLL);
	MoveWindow (hWindow, 0, CControlsModule::nScreenHeight - nHeight, nWidth, nHeight, FALSE);
}

/******************************************************************************/
void CHScrollBar::OnScroll (int code, int thumb_pos)
{
	DEBUGMSG(TRUE, (L"%s, code=%d, thumb_pos=%d\n", TEXT(__FUNCTION__), code, thumb_pos));

	int new_pos;

	new_pos = MoveScrollThumb (code, thumb_pos);

	// Move HTML page if necessary
	if (new_pos != -1)
		(*CControlsModule::PBCoreStruct.pBrowserSetHScroll) (0, new_pos);
}

/******************************************************************************/
/******************************************************************************/
CScroll::CScroll (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CControl (instance_id, hparent)
{
	nVExtent = -1;
	nHExtent = -1;

	nVPos = -1;
	nHPos = -1;

	// Create scroll bars
	pVScrollBar = new CVScrollBar (hinstance, hparent);
	pHScrollBar = new CHScrollBar (hinstance, hparent);
}

/******************************************************************************/
CScroll::~CScroll()
{
	delete pVScrollBar;
	delete pHScrollBar;
}

/******************************************************************************/
void CScroll::OnExtentChange (int v_extent, int h_extent)
{
	DEBUGMSG(TRUE, (L"%s, v_extent=%d, h_extent=%d\n", TEXT(__FUNCTION__), v_extent, h_extent));

	int width, height;
	BOOL v_visible, h_visible;

	v_visible = false;
	h_visible = false;

	// Start with full size of screen
	width = CControlsModule::nScreenWidth;
	height = CControlsModule::nScreenHeight;

	// Vertical scroll bar needed?
	if (v_extent > height)
	{
		// Yes, adjust width and show scroll bar
		width -= pVScrollBar->nWidth;
		v_visible = true;

		// Horizontal scroll bar now needed?
		if (h_extent > width)
		{
			// Yes, adjust height and show
			height -= pHScrollBar->nHeight;
			h_visible = true;
		}
	}
	else
		if (h_extent > width)
		{
			// Yes, adjust height and show scroll bar
			height -= pHScrollBar->nHeight;
			h_visible = true;

			// Vertical scroll bar now needed?
			if (v_extent > height)
			{
				// Yes, adjust width and show
				width -= pVScrollBar->nWidth;
				v_visible = true;
			}
		}

	// Size scroll bars to match
	pVScrollBar->Resize (pVScrollBar->nWidth, height);
	pHScrollBar->Resize (width, pHScrollBar->nHeight);

	// Set scroll bar ranges
	pVScrollBar->SetInfo (v_extent, height);
	pHScrollBar->SetInfo (h_extent, width);

	// Display scroll bars as necessary
	pVScrollBar->Show (v_visible);
	pHScrollBar->Show (h_visible);

	// Set browser size (assume app ID 0)
	CControlsModule::PBCoreStruct.pBrowserResize (0, MODULE_NAME, 0, 0, width, height);
}

/******************************************************************************/
void CScroll::OnPositionChange (int v_pos, int h_pos)
{
	DEBUGMSG(TRUE, (L"%s, v_pos=%d, h_pos=%d\n", TEXT(__FUNCTION__), v_pos, h_pos));

	pVScrollBar->SetPosition (v_pos);
	pHScrollBar->SetPosition (h_pos);
}

/******************************************************************************/
void CScroll::OnScrollChange (WPARAM wparam, LPARAM lparam)
{
	int v_pos, h_pos;
	int v_extent, h_extent;

	v_extent = HIWORD (wparam);
	h_extent = HIWORD (lparam);

	// Any change in extent?
	if (v_extent != nVExtent || h_extent != nHExtent)
		OnExtentChange (v_extent, h_extent);

	nVExtent = v_extent;
	nHExtent = h_extent;

	v_pos = LOWORD (wparam);
	h_pos = LOWORD (lparam);

	// Any change in position?
	if (v_pos != nVPos || h_pos != nHPos)
		OnPositionChange (v_pos, h_pos);

	nVPos = v_pos;
	nHPos = h_pos;
}
