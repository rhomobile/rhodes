/******************************************************************************/
#include <windows.h>
#include "HoldGesture.h"

/******************************************************************************/
CHoldGesture::CHoldGesture (POINT centre, int radius, int delay, int interval)
	: CGestureBase ()
{
	gtType = GT_HOLD;
	nHandlesInput = HI_MOUSE;

	ptCentre = centre;
	nRadius = radius;
	nDelay = delay;

	if (interval == 0)
		nInterval = INFINITE;
	else
		nInterval = interval;

	hCancel = CreateEvent (NULL, true, false, NULL);

	// Objects for drawing gesture diagnostics
	hLinePen = CreatePen (PS_SOLID, 2, RGB (0, 0, 192));
	hDetectedBrush = CreateSolidBrush (RGB (255, 228, 225));

	bDetected = false;
}

/******************************************************************************/
CHoldGesture::~CHoldGesture ()
{
	CloseHandle (hCancel);

	DeleteObject (hLinePen);
	DeleteObject (hDetectedBrush);
}

/******************************************************************************/
bool CHoldGesture::IsInside (int x, int y)
{
	float distance = sqrtf ((float) (((x - ptCentre.x) * (x - ptCentre.x)) + ((y - ptCentre.y) * (y - ptCentre.y))));
	return (distance < nRadius);
}

/******************************************************************************/
void CHoldGesture::WaitForDetection (void)
{
	// Wait for initial delay, then fire event
	if (WaitForSingleObject (hCancel, nDelay) != WAIT_TIMEOUT)
		return;

	bDetected = true;
	nDetectCount = 1;
	OnDetected ();
	OnRepaint ();

	// Periodically fire subsequent events
	while (1)
	{
		if (WaitForSingleObject (hCancel, nInterval) != WAIT_TIMEOUT)
			break;

		nDetectCount++;
		OnDetected ();
		OnRepaint ();
	}
}

/******************************************************************************/
DWORD CHoldGesture::TimerProc (LPVOID pparam)
{
	CHoldGesture *pthis = (CHoldGesture*) pparam;
	pthis->WaitForDetection ();

	return 0;
}

/******************************************************************************/
void CHoldGesture::StartTimer (void)
{
	HANDLE hthread;

	ResetEvent (hCancel);
	hthread = CreateThread (NULL, 0, TimerProc, this, 0, NULL);
	CloseHandle (hthread);
}

/******************************************************************************/
void CHoldGesture::CancelTimer (void)
{
	SetEvent (hCancel);

	if (bDetected)
	{
		bDetected = false;

		// Fire final event to indicate end of hold
		nDetectCount = 0;
		OnDetected ();

		OnRepaint ();
	}
}

/******************************************************************************/
void CHoldGesture::HandleMouseDown (int x, int y)
{
	if (IsInside (x, y))
		StartTimer ();
}

/******************************************************************************/
void CHoldGesture::HandleMouseMove (int x, int y)
{
	if (!IsInside (x, y))
		CancelTimer ();
}

/******************************************************************************/
void CHoldGesture::HandleMouseUp (int x, int y)
{
	CancelTimer ();
}

/******************************************************************************/
void CHoldGesture::Render (HDC hdc)
{
	RECT rect;
	WCHAR s [16];

	if (!bShowDiagnostics)
		return;

	SelectObject (hdc, hLinePen);
	SelectObject (hdc, bDetected ? hDetectedBrush : (HBRUSH)GetStockObject(NULL_BRUSH));
	Ellipse (hdc, ptCentre.x - nRadius, ptCentre.y - nRadius, ptCentre.x + nRadius, ptCentre.y + nRadius);

	if (bDetected)
	{
		SetBkMode (hdc, TRANSPARENT);
		rect.left = ptCentre.x - 16;
		rect.top = ptCentre.y - 16;
		rect.right = ptCentre.x + 16;
		rect.bottom = ptCentre.y + 16;
		wsprintf (s, L"%d", nDetectCount);
		DrawText (hdc, s, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
	else
	{
		MoveToEx (hdc, ptCentre.x - 8, ptCentre.y - 8, NULL);
		LineTo (hdc, ptCentre.x + 8, ptCentre.y + 8);
		MoveToEx (hdc, ptCentre.x + 8, ptCentre.y - 8, NULL);
		LineTo (hdc, ptCentre.x - 8, ptCentre.y + 8);
	}
}
