/******************************************************************************/
#include <windows.h>
#include "ShakeGesture.h"

/******************************************************************************/
CShakeGesture::CShakeGesture (int threshold, int quiet_period)
	: CGestureBase ()
{
	gtType = GT_SHAKE;
	nHandlesInput = HI_IST_SHAKE;

	nThreshold = threshold;
	nQuietPeriod = quiet_period;

	bWaitingQuiet = false;
}

/******************************************************************************/
CShakeGesture::~CShakeGesture ()
{
}

/******************************************************************************/
void CShakeGesture::HandleSensor (int accel_x, int accel_y, int accel_z)
{
	bool any_active;
	int elapsed;

	// Now the range is from -2 to 2 but our algorithm assumes that the range is from 0 to 4000
	// so add 2000 to the values

	accel_x += 2000;
	accel_y += 2000;
	accel_z += 2000;

	any_active = (accel_x < nThreshold || accel_x > (4000 - nThreshold)) ||
		(accel_y < nThreshold || accel_y > (4000 - nThreshold)) ||
		(accel_z < nThreshold || accel_z > (4000 - nThreshold));

	if (bWaitingQuiet)
	{
		if (!any_active)
		{
			// Get time since quiet period started - ignore rollover
			elapsed = GetTickCount () - dwQuietStart;

			if (elapsed >= nQuietPeriod)
				bWaitingQuiet = false;
		}
		else
		{
			dwQuietStart = GetTickCount ();
		}
	}
	else
	{
		// Out of quiet period
		if (any_active)
		{
			OnDetected ();
			dwQuietStart = GetTickCount ();
			bWaitingQuiet = true;
		}
	}
}
