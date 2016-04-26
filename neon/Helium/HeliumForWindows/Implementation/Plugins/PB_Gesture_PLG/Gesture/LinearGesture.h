/** 
* \file LinearGesture.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "GestureBase.h"

/******************************************************************************/
/**
* Linear gestures are defined by start and points, tolerance (how wide the gesture region is),
* sensitivity (what percentage of the region must be covered by the mouse track),
* maximum allowable angle between the mouse track and the gesture line, and 
* maximum allowable deviation of tme mouse track from a straight line.
*/
class CLinearGesture: public CGestureBase
{
private:
	POINT ptStart;	///< The start point of the gesture.
	POINT ptEnd;	///< The end point of the gesture - the gesture must be performed in the start to end direction.

	int nRegions;				///< Count of sub-regions created by the constructor, see \ref CLinearGesture
	CPolyRegion **ppRegions;	///< Pointer to array of rectangular sub-regions which cover the gesture line.

	int nSensitivity;		///< Minimum percentage of sub-regions which must be crossed by the mouse track.
	int nMaxSkew;			///< Maximum angle between the mouse track and the gesture line.
	int nMaxDeviation;		///< Maximum deviation of the mouse track from a straight line.
	POINT ptBestFitStart;	///< The start point of the line which best fits the mouse track. This is calculated during \ref HandleMouseTrack and used for diagnostics.
	POINT ptBestFitEnd;		///< The end point of the line which best fits the mouse track. This is calculated during \ref HandleMouseTrack and used for diagnostics.
	float fSkewAngle;		///< The angle between the mouse track and the gesture line.
	float fDeviation;		///< The deviation of the mouse track from a straight line.

	HPEN hLinePen;			///< Pen used for diagnostic drawing
	HPEN hBestFitPen;		///< Pen used for diagnostic drawing
	HBRUSH hLineBrush;		///< Brush used for diagnostic drawing

	/**
	* Finds best fit line to mouse track using least squares method. Sets \ref fDeviation, \ref fSkewAngle,
	* \ref ptBestFitStart and \ref prBestFitEnd.
	* \param npoints Number of points in mouse track
	* \param ppoints Array of points in mouse track
	*/
	void SetBestFitLine(int npoints, POINT *ppoints);
	
public:
	/**
	* Constructor for a linear gesture. Creates an array of rectangular sub-regions which span 
	* the gesture line.
	* \param start The start point of the gesture.
	* \param end The end point of the gesture - the gesture must be performed in the start to end direction.
	* \param tolerance The maximum distance from the mouse track to the gesture line.
	* \param sensitivity Minimum percentage of sub-regions which must be crossed by the mouse track.
	* \param max_skew Maximum angle between the mouse track and the gesture line.
	* \param max_deviation Maximum deviation of the mouse track from a straight line.
	*/
	CLinearGesture::CLinearGesture (POINT start, POINT end, int tolerance, int sensitivity,
		int max_skew, int max_deviation, int region_width);
	~CLinearGesture ();

	void HandleMouseTrack (int npoints, POINT *ppoints);
	void Render (HDC hdc);
};