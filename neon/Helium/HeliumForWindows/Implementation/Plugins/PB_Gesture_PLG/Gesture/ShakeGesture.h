/** 
* \file HoldGesture.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "GestureBase.h"

/******************************************************************************/
/**
* Shake generates a gesture when the IST reports accelerometer values within a given
* threshold of the maximum values in at least one axis. Once a gesture has been detected 
* there must be a quiet period before another gesture is detected.
*/
class CShakeGesture: public CGestureBase
{
private:
	int nThreshold;		///< Threshold for accelerometer values to detect gesture
	int nQuietPeriod;	///< Quiet period before gesture is detected again
	bool bWaitingQuiet;	///< Flag to indicate that quiet period is in progress
	DWORD dwQuietStart;	///< System tick count when quiet period was started

public:
	/**
	* Constructor for a shake gesture.
	* \param threshold Accelerometer values from IST run from -2046 to 2046; reported values must be within 'threshold' of these min/max values to be recognised. Recommended value is 200.
	* \param quiet_period Period in milliseconds of non-shake time after a gesture is detected before another gesture will fire.
	*/
CShakeGesture (int threshold, int quiet_period);
	~CShakeGesture ();

	void HandleSensor (int x, int y, int z);
};