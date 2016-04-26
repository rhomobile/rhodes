/******************************************************************************/
#include <windows.h>
#include <math.h>
#include "CircleGesture.h"

/******************************************************************************/
int GetQuadrant(float angle)
{
	while (angle >= 2 * PI)
		angle -= (float)(2 * PI);

	while (angle < 0)
		angle += (float)(2 * PI);

	if (angle >= (PI / 2) && angle < PI)
		return 1;

	if (angle >= PI && angle < (3 * PI / 2))
		return 2;

	if (angle >= (3 * PI / 2) && angle < (2 * PI))
		return 3;

	return 0;
}

/******************************************************************************/
CCircleGesture::CCircleGesture (POINT centre, int radius, int start_angle, int end_angle,
								int tolerance, int sensitivity)
	: CGestureBase ()
{
	float start_radians, end_radians, angle, increment;
	float dx, dy, dx_squared, dy_squared, tan_squared;
	CCircleRegion *pregion;
	bool anti_clockwise;
	int n;

	gtType = GT_CIRCLE;
	nHandlesInput = HI_MOUSE_TRACK;

	hLinePen = 0;

	ptCentre = centre;
	nRadius = radius;

	if (end_angle < start_angle)
	{
		n = start_angle;
		start_angle = end_angle;
		end_angle = n;
		anti_clockwise = true;
	}
	else
		anti_clockwise = false;

	// Convert to radians
	start_radians = start_angle * 2.0F * PI / 360.0F;
	end_radians = end_angle * 2.0F * PI / 360.0F;

	// Calculate angular distance between regions
	increment = (float) tolerance / (float) radius;

	// Count number of regions to be created
	nRegions = 1;

	if (anti_clockwise)
		angle = (float) end_radians;
	else
		angle = (float) start_radians;

	while (true)
	{
		if (anti_clockwise)
		{
			if (angle <= start_radians)
				break;

			angle -= increment;
		}
		else
		{
			if (angle >= end_radians)
				break;

			angle += increment;
		}

		nRegions++;
	}

	// Create array of regions
	ppRegions = new CCircleRegion* [nRegions];

	// Create regions
	if (anti_clockwise)
		angle = (float) end_radians;
	else
		angle = (float) start_radians;

	for (n = 0; n < nRegions; n++)
	{
		// Precision in Pi value not enough to make angle exactly = pi/2, so no overflow will occur
		tan_squared = (float) tan (angle);
		tan_squared *= tan_squared;

		dx_squared = ((radius * radius) / (1 + tan_squared));
		dx = sqrtf (dx_squared);

		dy_squared = dx_squared * tan_squared;
		dy = sqrtf (dy_squared);

		switch (GetQuadrant(angle))
		{
			case 1:
				dx = -dx;
				break;

			case 2:
				dx = -dx;
				dy = -dy;
				break;

			case 3:
				dy = -dy;
				break;
		}

		pregion = new CCircleRegion(centre.x + dx, centre.y + dy, (float) tolerance);
		ppRegions [n] = pregion;

		if (anti_clockwise)
			angle -= increment;
		else
			angle += increment;
	}

	// Record parameters for later fit testing
	nSensitivity = sensitivity;

	// Objects for drawing gesture diagnostics
	hLinePen = CreatePen (PS_SOLID, 2, RGB (0, 0, 192));
}

/******************************************************************************/
CCircleGesture::~CCircleGesture ()
{
	int n;

	if (ppRegions)
	{
		for (n = 0; n < nRegions; n++)
			delete ppRegions [n];

		delete [] ppRegions;
	}

	if (hLinePen)
		DeleteObject (hLinePen);
}

/******************************************************************************/
void CCircleGesture::HandleMouseTrack (int npoints, POINT *ppoints)
{
	int p, r, current, found, count;

	if (!ppRegions)
		goto Exit;

	// Mark all regions as uncrossed
	for (r = 0; r < nRegions; r++)
		ppRegions[r]->Crossed = false;

	if (npoints <= 2)
		goto Exit;

	current = 0;

	// For each point in track...
	for (p = 0; p < npoints; p++)
	{
		// Find enclosing region, starting from current region
		found = -1;
		for (r = current; r < nRegions; r++)
			if (ppRegions[r]->ContainsPoint(ppoints[p]))
			{
				found = r;
				break;
			}

		// Not a gesture if point is outside all regions
		if (found == -1)
			goto Exit;

		// Mark found region as crossed, continue search from this region
		ppRegions[found]->Crossed = true;
		current = found;
	}

	// Count number of crossed regions
	count = 0;
	for (r = 0; r < nRegions; r++)
		if (ppRegions[r]->Crossed)
			count++;

	// Compare against sensitivity threshold
	if (count < (nRegions * nSensitivity) / 100)
		goto Exit;

	OnDetected ();

Exit:
	OnRepaint ();
}

/******************************************************************************/
void CCircleGesture::Render (HDC hdc)
{
	int n;

	if (!bShowDiagnostics)
		return;

	if (ppRegions)
		for (n = 0; n < nRegions; n++)
			ppRegions [n]->Render (hdc);

	if (hLinePen)
	{
		SelectObject (hdc, hLinePen);
		SelectObject (hdc, (HBRUSH) GetStockObject (NULL_BRUSH));
		Ellipse (hdc, ptCentre.x - nRadius, ptCentre.y - nRadius, ptCentre.x + nRadius, ptCentre.y + nRadius);
	}
}
