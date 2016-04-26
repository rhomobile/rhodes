/******************************************************************************/
#include <windows.h>
#include "GestureBase.h"

/******************************************************************************/
/* CGestureBase
/******************************************************************************/
CGestureBase::CGestureBase ()
{
	gtType = GT_UNKNOWN;
	pRepaintProc = NULL;
	bShowDiagnostics = false;
}

/******************************************************************************/
CGestureBase::~CGestureBase ()
{
}

/******************************************************************************/
/* CGestureRegion
/******************************************************************************/
CGestureRegion::CGestureRegion ()
{
	Crossed = false;
}

/******************************************************************************/
CGestureRegion::~CGestureRegion ()
{
}

/******************************************************************************/
/* CPolyRegion
/******************************************************************************/
int CPolyRegion::nInstanceCount = 0;
HPEN CPolyRegion::hOutlinePen;
HBRUSH CPolyRegion::hCrossedBrush;
HBRUSH CPolyRegion::hNonCrossedBrush;

/******************************************************************************/
CPolyRegion::CPolyRegion (int points)
{
	nPoints = points;
	pPoints = new POINT [nPoints];
	nNextPoint = 0;

	if (nInstanceCount++ == 0)
	{
		hOutlinePen = CreatePen (PS_SOLID, 2, RGB (192, 0, 0));
		hCrossedBrush = CreateSolidBrush (RGB (255, 228, 225));
		hNonCrossedBrush = CreateSolidBrush (RGB (255, 255, 255));
	}
}

/******************************************************************************/
CPolyRegion::~CPolyRegion ()
{
	if (pPoints)
		delete [] pPoints;

	if (--nInstanceCount == 0)
	{
		DeleteObject (hOutlinePen);
		DeleteObject (hCrossedBrush);
		DeleteObject (hNonCrossedBrush);
	}
}

/******************************************************************************/
void CPolyRegion::AddPoint (POINT point)
{
	if (nNextPoint < nPoints)
		memcpy (pPoints + nNextPoint++, &point, sizeof POINT);
}

/******************************************************************************/
void CPolyRegion::Render (HDC hdc)
{
	if (!pPoints)
		return;

	SelectObject (hdc, hOutlinePen);
	SelectObject (hdc, Crossed ? hCrossedBrush : (HBRUSH)GetStockObject(NULL_BRUSH));

	Polygon (hdc, pPoints, nPoints);
}

/******************************************************************************/
int GetSign (float f)
{
	if (f < 0.0F)
		return -1;

	if (f > 0.0F)
		return 1;

	return 0;
}

/******************************************************************************/
float Determinant(POINT p0, POINT p1, POINT p2)
{
	return ((float)p1.x * (float)p2.y) - ((float)p1.y * (float)p2.x) -
		((float)p0.x * (float)p2.y) + ((float)p0.y * (float)p2.x) +
		((float)p0.x * (float)p1.y) - ((float)p0.y * (float)p1.x);
}

/******************************************************************************/
bool CPolyRegion::ContainsPoint (POINT point)
{
	int n, det_type = 0;
	float det;

	if (!pPoints)
		return false;

	if (nPoints < 3)
		return false;

	for (n = 0; n < nPoints - 1; n++)
	{
		det = Determinant(pPoints[n], pPoints[n + 1], point);

		if (det_type == 0)
		{
			det_type = GetSign(det);
		}
		else
		{
			if (det_type != GetSign(det))
			{
				return false;
			}
		}
	}

	det = Determinant(pPoints[nPoints - 1], pPoints[0], point);
	return (GetSign(det) == det_type);
}

/******************************************************************************/
/* CCircleRegion
/******************************************************************************/
int CCircleRegion::nInstanceCount = 0;
HPEN CCircleRegion::hOutlinePen;
HBRUSH CCircleRegion::hCrossedBrush;

/******************************************************************************/
CCircleRegion::CCircleRegion (float centre_x, float centre_y, float radius)
{
	fCentreX = centre_x;
	fCentreY = centre_y;
	fRadius = radius;

	if (nInstanceCount++ == 0)
	{
		hOutlinePen = CreatePen (PS_SOLID, 2, RGB (192, 0, 0));
		hCrossedBrush = CreateSolidBrush (RGB (255, 228, 225));
	}
}

/******************************************************************************/
CCircleRegion::~CCircleRegion ()
{
	if (--nInstanceCount == 0)
	{
		DeleteObject (hOutlinePen);
		DeleteObject (hCrossedBrush);
	}
}

/******************************************************************************/
void CCircleRegion::Render (HDC hdc)
{
	SelectObject (hdc, hOutlinePen);
	SelectObject (hdc, Crossed ? hCrossedBrush : (HBRUSH)GetStockObject(NULL_BRUSH));
	Ellipse (hdc, (int) (fCentreX - fRadius), (int) (fCentreY - fRadius), (int) (fCentreX + fRadius), (int) (fCentreY + fRadius));
}

/******************************************************************************/
bool CCircleRegion::ContainsPoint (POINT point)
{
	float distance;

	distance = sqrtf (((point.x - fCentreX) * (point.x - fCentreX)) + ((point.y - fCentreY) * (point.y - fCentreY)));

	return distance <= fRadius;
}
