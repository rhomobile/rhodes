/******************************************************************************/
#include <windows.h>
#include <math.h>
#include "LinearGesture.h"

/******************************************************************************/
CLinearGesture::CLinearGesture (POINT start, POINT end, int tolerance, int sensitivity,
	int max_skew, int max_deviation, int region_width)
	: CGestureBase ()
{
	float x_offset, y_offset, line_gradient, perp_gradient, line_length;
	float x_region_separation, y_region_separation, x_region_width, y_region_width;
	float midpoint_x, midpoint_y;
	int segments, n;
	POINT vertex;
	CPolyRegion *pregion;

	gtType = GT_LINEAR;
	nHandlesInput = HI_MOUSE_TRACK;

	hLinePen = 0;
	hBestFitPen = 0;
	hLineBrush = 0;

	// Indicate that best fit hasn't yet been calculated
	ptBestFitStart.x = -1;

	nRegions = 0;
	ppRegions = 0;

	ptStart = start;
	ptEnd = end;

	// Handle special cases
	if (ptStart.y == ptEnd.y)
	{
		x_offset = 0;
		y_offset = (float)tolerance;
	}
	else if (ptStart.x == ptEnd.x)
	{
		x_offset = (float)tolerance;
		y_offset = 0;
	}
	else
	{
		// Get gradient of line
		line_gradient = (float)(ptEnd.y - ptStart.y) / (float)(ptEnd.x - ptStart.x);

		// Get gradient of perp. line
		perp_gradient = -(1 / line_gradient);

		// Get offsets to perp. points
		y_offset = sqrtf((tolerance * tolerance * perp_gradient * perp_gradient) / ((perp_gradient * perp_gradient) + 1));
		x_offset = (y_offset / perp_gradient);
	}

	// Get length of gesture line, calculate number of segments
	line_length = (float) sqrt((float) ((ptEnd.x - ptStart.x) * (ptEnd.x - ptStart.x) + (ptEnd.y - ptStart.y) * (ptEnd.y - ptStart.y)));
	segments = (int)ceil(line_length / (float)region_width);

	// Get x, y values to divide line into segments
	x_region_separation = (float)(ptEnd.x - ptStart.x) / (float)(segments - 1);
	y_region_separation = (float)(ptEnd.y - ptStart.y) / (float)(segments - 1);

	// Add overlap
	x_region_width = x_region_separation * 1.2F;
	y_region_width = y_region_separation * 1.2F;

	// Create segments
	nRegions = segments;
	ppRegions = new CPolyRegion* [segments];

	for (n = 0; n < segments; n++)
	{
		// Create region with 4 vertices
		pregion = new CPolyRegion (4);

		midpoint_x = (float) ptStart.x + (x_region_separation * n);
		midpoint_y = (float) ptStart.y + (y_region_separation * n);

		vertex.x = (int)(midpoint_x - (x_region_width / 2) + x_offset);
		vertex.y = (int)(midpoint_y - (y_region_width / 2) + y_offset);
		pregion->AddPoint (vertex);

		vertex.x = (int)(midpoint_x + (x_region_width / 2) + x_offset);
		vertex.y = (int)(midpoint_y + (y_region_width / 2) + y_offset);
		pregion->AddPoint (vertex);

		vertex.x = (int)(midpoint_x + (x_region_width / 2) - x_offset);
		vertex.y = (int)(midpoint_y + (y_region_width / 2) - y_offset);
		pregion->AddPoint (vertex);

		vertex.x = (int)(midpoint_x - (x_region_width / 2) - x_offset);
		vertex.y = (int)(midpoint_y - (y_region_width / 2) - y_offset);
		pregion->AddPoint (vertex);

		ppRegions [n] = pregion;
	}

	// Record parameters for later fit testing
	nSensitivity = sensitivity;
	nMaxSkew = max_skew;
	nMaxDeviation = max_deviation;

	// Objects for drawing gesture diagnostics
	hLinePen = CreatePen (PS_SOLID, 2, RGB (0, 0, 192));
	hBestFitPen = CreatePen (PS_SOLID, 2, RGB (0, 192, 0));
	hLineBrush = CreateSolidBrush (RGB (0, 0, 192));
}

/******************************************************************************/
CLinearGesture::~CLinearGesture ()
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

	if (hBestFitPen)
		DeleteObject (hBestFitPen);

	if (hLineBrush)
		DeleteObject (hLineBrush);
}

/******************************************************************************/
void CLinearGesture::SetBestFitLine(int npoints, POINT *ppoints)
{
	float sum_x, sum_x_squared, sum_y, sum_x_y;
	float least_squares_gradient, least_squares_intercept;
	float total_error, error_distance, deviation;
	float path_gradient, skew_angle;
	int p;

	if (npoints < 2)
		return;

	// Calculate least squares regression line
	sum_x = 0, sum_x_squared = 0, sum_y = 0, sum_x_y = 0;

	for (p = 0; p < npoints; p++)
	{
		sum_x += ppoints[p].x;
		sum_x_squared += (ppoints[p].x * ppoints[p].x);
		sum_y += ppoints[p].y;
		sum_x_y += ppoints[p].x * ppoints[p].y;
	}

	least_squares_gradient = ((sum_y * sum_x) - (npoints * sum_x_y)) / ((sum_x * sum_x) - (npoints * sum_x_squared));
	least_squares_intercept = ((sum_x * sum_x_y) - (sum_y * sum_x_squared)) / ((sum_x * sum_x) - (npoints * sum_x_squared));

	// Calculate mean of error squares for best fit line, using distance from line
	total_error = 0;

	for (p = 0; p < npoints; p++)
	{
		error_distance = fabsf((least_squares_gradient * ppoints[p].x) - ppoints[p].y + least_squares_intercept) /
			sqrtf((least_squares_gradient * least_squares_gradient) + 1);

		total_error += error_distance  * error_distance;
	}

	deviation = sqrtf(total_error / npoints);

	// Calculate angle between best fit line and gesture path
	if (ptEnd.x == ptStart.x)
	{
		// Path is vertical
		skew_angle = (float) atan2 (1.0F, least_squares_gradient);
	}
	else
	{
		path_gradient = ((float)(ptEnd.y - ptStart.y)) / ((float)(ptEnd.x - ptStart.x));
		skew_angle = (float) atan2 (least_squares_gradient - path_gradient, 1 + least_squares_gradient * path_gradient);
	}

	// Record values for later fit testing and drawing
	fDeviation = deviation;
	fSkewAngle = (skew_angle * 180.0F) / PI;	// Convert radians to degrees

	if (fSkewAngle < -90)
		fSkewAngle += 180;

	if (fSkewAngle > 90)
		fSkewAngle -= 180;

	fSkewAngle = (float) fabs (fSkewAngle);

	// Find start and end points of best fit line for drawing
	ptBestFitStart.x = ppoints[0].x;
	ptBestFitStart.y = (int) ((ppoints[0].x * least_squares_gradient) + least_squares_intercept);

	ptBestFitEnd.x = ppoints[npoints - 1].x;
	ptBestFitEnd.y = (int) ((ppoints[npoints - 1].x * least_squares_gradient) + least_squares_intercept);
}

/******************************************************************************/
void CLinearGesture::HandleMouseTrack (int npoints, POINT *ppoints)
{
	int p, r, current, found, count;

	// Indicate that best fit hasn't yet been calculated
	ptBestFitStart.x = -1;

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

	// Calculate least squares line and fit parameters
	SetBestFitLine(npoints, ppoints);

	// Test for skew
	if (fSkewAngle > (float) nMaxSkew)
		goto Exit;

	// Test for deviation
	if (fDeviation > (float) nMaxDeviation)
		goto Exit;

	OnDetected ();

Exit:
	OnRepaint ();
}

/******************************************************************************/
void CLinearGesture::Render (HDC hdc)
{
	int n;
	WCHAR s [32];
	RECT rect;

	if (!bShowDiagnostics)
		return;

	if (ppRegions)
		for (n = 0; n < nRegions; n++)
			ppRegions [n]->Render (hdc);

	if (hLinePen)
	{
		SelectObject (hdc, hLinePen);
		SelectObject (hdc, hLineBrush);

		MoveToEx (hdc, ptStart.x, ptStart.y, NULL);
		LineTo (hdc, ptEnd.x, ptEnd.y);

		Ellipse (hdc, ptStart.x - 4, ptStart.y - 4, ptStart.x + 4, ptStart.y + 4);
		Ellipse (hdc, ptEnd.x - 4, ptEnd.y - 4, ptEnd.x + 4, ptEnd.y + 4);
	}

	if (hBestFitPen && ptBestFitStart.x != -1)
	{
		// Draw best fit line
		SelectObject (hdc, hBestFitPen);
		MoveToEx (hdc, ptBestFitStart.x, ptBestFitStart.y, NULL);
		LineTo (hdc, ptBestFitEnd.x, ptBestFitEnd.y);

		// Show skew angle
		if (!_isnan (fSkewAngle))
		{
			wsprintf (s, L"%.1f", fSkewAngle);
			rect.left = ptBestFitStart.x + 16;
			rect.top = ptBestFitStart.y + 16;
			rect.right = rect.left + 80;
			rect.bottom = rect.top + 80;
			DrawText (hdc, s, wcslen (s), &rect, DT_LEFT | DT_NOCLIP);
		}

		// Show deviation
		if (!_isnan (fDeviation))
		{
			wsprintf (s, L"%.1f", fDeviation);
			rect.left = ptBestFitEnd.x + 16;
			rect.top = ptBestFitEnd.y + 16;
			rect.right = rect.left + 80;
			rect.bottom = rect.top + 80;
			DrawText (hdc, s, wcslen (s), &rect, DT_LEFT | DT_NOCLIP);
		}
	}
}
