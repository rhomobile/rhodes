/** 
* \file HoldGesture.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "GestureBase.h"

/**
* Hold gestures are defined by centre point, radius, initial delay and interval between firings.
* It will generate multiple detections as long as the mouse is held within the gesture area.
*/
class CHoldGesture: public CGestureBase
{
private:
	POINT ptCentre;		///< The centre point of the gesture
	int nRadius;		///< The radius of the gesture
	int nDelay;			///< Initial delay before first event
	int nInterval;		///< Delay between events after the first event

	bool IsInside (int x, int y);				///< True if the specified point is inside the gesture region
	static DWORD TimerProc (LPVOID pparam);		///< Thread procedure started when gesture region is first touched
	void WaitForDetection (void);				///< Waits for configured delays before firing event
	void StartTimer (void);						///< Launches timer thread - called when mouse down occurs within gesture region
	void CancelTimer (void);					///< Cancels timer thread - called if mouse is lifted or moved out of gesture region
	HANDLE hCancel;								///< Synchronization event used to cancel timer thread
	bool bDetected;								///< True if gesture is being detected

	HPEN hLinePen;								///< Pen for drawing gesture region in diagnostic mode
	HBRUSH hDetectedBrush;						///< Brush for filling gesture region in diagnostic mode
	
public:
	/**
	* Constructor for a touch and hold gesture.
	* \param centre The centre point of the gesture.
	* \param radius The radius of the gesture.
	* \param delay The delay in milliseconds before the first detection event.
	* \param interval The delay in milliseconds between detection events after the first event. Zero means only the first event will fire.
	*/
	CHoldGesture (POINT centre, int radius, int delay, int interval);
	~CHoldGesture ();

	void HandleMouseDown (int x, int y);
	void HandleMouseMove (int x, int y);
	void HandleMouseUp (int x, int y);

	void Render (HDC hdc);

	int nDetectCount;							///< Number of times gesture has been detected since first event
};