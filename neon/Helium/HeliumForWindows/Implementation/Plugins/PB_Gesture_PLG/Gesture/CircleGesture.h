/** 
* \file CircleGesture.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "GestureBase.h"

/******************************************************************************/
/**
* Circle gestures are defined by centre point, radius, start and end angles,
* direction, tolerance and sensitivity. See constructor for details.
*/
class CCircleGesture: public CGestureBase
{
private:
	POINT ptCentre;		///< Centre of gesture
	int nRadius;		///< Radius of gesture

	int nRegions;		///< Number of regions covering gesture
	CCircleRegion **ppRegions;	///< Pointer to array of circluar regions covering the gesture

	int nSensitivity;	///< Sensitivity factor, see constructor for details

	HPEN hLinePen;		// Pen for drawing diagnostics
	
public:
	/**
	* Constructor for a circle gesture. Creates an array of circular sub-regions which span 
	* the gesture path.
	* \param centre The centre of the gesture path
	* \param radius The radius of the gesture path
	* \param start_angle The starting angle of the gesture in degrees. Zero degrees is a horizontal line from the centre (i.e. at 3 o'clock).
	* \param end_angle The ending angle of the gesture in degrees. Zero degrees is a horizontal line from the centre (i.e. at 3 o'clock).
	* \param tolerance How close to the gesture path the mouse track must be; translates to the radius of the circluar regions covering the path.
	* \param sensitivity Minimum percentage of regions which must be crossed by the mouse track.
	*/
CCircleGesture::CCircleGesture (POINT centre, int radius, int start_angle, int end_angle,
		int tolerance, int sensitivity);
	~CCircleGesture ();

	void HandleMouseTrack (int npoints, POINT *ppoints);
	void Render (HDC hdc);
};