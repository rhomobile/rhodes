/******************************************************************************/
#include <windows.h>
#include "TiltGesture.h"

/******************************************************************************/
CTiltGesture::CTiltGesture (int x, int y, int z, int tolerance, int hysteresis)
	: CGestureBase ()
{
	gtType = GT_TILT;
	nHandlesInput = HI_IST_TILT;

	nXTarget = x;
	nYTarget = y;
	nZTarget = z;

	nTolerance = tolerance;
	nHysteresis = hysteresis;

	bInRange = false;
}

/******************************************************************************/
CTiltGesture::~CTiltGesture ()
{
}

/******************************************************************************/
void CTiltGesture::HandleSensor (int tilt_x, int tilt_y, int tilt_z)
{
	bool x_detected = false, y_detected = false, z_detected = false;
	
	if (bInRange)
	{
		// Wait for one value to be out of range allowing for hysteresis
		if (tilt_x > (nXTarget + nTolerance + nHysteresis) || tilt_x < (nXTarget - nTolerance - nHysteresis))
			bInRange = false;
	
		if (tilt_y > (nYTarget + nTolerance + nHysteresis) || tilt_y < (nYTarget - nTolerance - nHysteresis))
			bInRange = false;

		if (tilt_z > (nZTarget + nTolerance + nHysteresis) || tilt_z < (nZTarget - nTolerance - nHysteresis))
			bInRange = false;
	}
	else
	{
		// Wait for all values to be in range
		if (tilt_x < (nXTarget + nTolerance) && tilt_x > (nXTarget - nTolerance))
			x_detected = true;

		if (tilt_y < (nYTarget + nTolerance) && tilt_y > (nYTarget - nTolerance))
			y_detected = true;

		if (tilt_z < (nZTarget + nTolerance) && tilt_z > (nZTarget - nTolerance))
			z_detected = true;

		bInRange = (x_detected && y_detected && z_detected);

		if (bInRange)
			OnDetected ();
	}
}
