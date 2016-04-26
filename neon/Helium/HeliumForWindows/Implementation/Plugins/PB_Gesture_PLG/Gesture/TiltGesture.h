/** 
* \file TileGesture.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "GestureBase.h"

/**
* Tilt gestures are defined by the IST tilt sensor values representing a given orientation in space.
* The orientation must move out of the defined position and back in again to fire another detection.
*/
class CTiltGesture: public CGestureBase
{
private:
	int nXTarget;		///< Target X sensor value; values range from -90 to +90.
	int nYTarget;		///< Target Y sensor value; values range from -90 to +90.
	int nZTarget;		///< Target Z sensor value; values range from -90 to +90.
	int nTolerance;		///< Allowable deviation from target values - see constructor
	int nHysteresis;	///< To prevent false re-triggering - see constructor
	int bInRange;		///< Flag to indicate that reported IST values are in allowable range of target

public:
	/**
	* Constructor for a tilt gesture.
	* \param x The target X tilt sensor value
	* \param y The target Y tilt sensor value
	* \param z The target Z tilt sensor value
	* \param tolerance The IST values must all be within 'tolerance' of the target values for the geture to fire
	* \param hysteresis After a gesture is fired any one IST value must move away from its target value by at last 'tolerance' plus 'hysteresis' to allow the gesture to fire again; this is to prevent false re-triggering owing to the any device shake.
	*/
CTiltGesture (int x, int y, int z, int tolerance, int hysteresis);
	~CTiltGesture ();

	void HandleSensor (int x, int y, int z);
};