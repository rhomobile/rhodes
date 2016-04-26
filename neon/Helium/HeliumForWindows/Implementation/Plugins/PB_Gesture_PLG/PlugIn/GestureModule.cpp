/******************************************************************************/
/*
- How to select REGISTER_ANY_WINDOW at run time?

/******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "GestureModule.h"

/******************************************************************************/
bool StringToBool (LPCTSTR pstring)
{
	if (!_wcsicmp (pstring, L"true") || !_wcsicmp (pstring, L"yes"))
		return true;

	return false;
}

/******************************************************************************/
CGestureModule *CGestureModule::pThis;

/******************************************************************************/
CGestureModule::CGestureModule (PPBCORESTRUCT pPBCoreStructure)
{
	pThis = this;
	CurrentGesture = GT_UNKNOWN;
	pSensor = NULL;
	m_pPBCoreStructure = pPBCoreStructure;
}

/******************************************************************************/
BOOL CGestureModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Gesture");
	RegisterForEvent (PB_APPFOCUSEVENT);
	RegisterForEvent (PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent (PB_BROWSER_NAV_COMPLETE_EVENT);
	RegisterForEvent (PB_BROWSER_DOC_COMPLETE_EVENT);

	return true;
}

/******************************************************************************/
void CGestureModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent (PB_APPFOCUSEVENT);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_BROWSER_DOC_COMPLETE_EVENT);
}

/******************************************************************************/
BOOL CGestureModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Creating Instance",L"onAttachInstance",__LINE__);

	RegisterForMessage (WM_LBUTTONDOWN);
	RegisterForMessage (WM_MOUSEMOVE);
	RegisterForMessage (WM_LBUTTONUP);

	RegisterForPaint ();

	// For each instance create a list of gesture objects, get the framework to hold it
	// Include the container window handle for gesture diagnostic painting
	pCurrentSet = new CGestureSet (m_pPBCoreStructure);
	pCurrentSet->hwndContainer = pPBStructure->hWnd;
	pCurrentSet->nInstanceID = pInstStruct->instID;
	pCurrentSet->bJavascriptEvent = FALSE;

	pInstStruct->pWrappedObj = pCurrentSet;
	if(pInstStruct->pWrappedObj)
		return TRUE;

	return FALSE;
}

/******************************************************************************/
BOOL CGestureModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Moved disabling IST to this method as the onDeInit function was not being called (I believe
	//  this is a long standing bug in RhoElements 1.x even that was never prioritized).  
	DisableIST ();
	CGestureSet *pset = (CGestureSet*) (pInstStruct->pWrappedObj);
	if (pset)
		delete pset;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::onAppFocus(int iOldID,int iNewID)
{
	WCHAR s [32];
	wsprintf (s, L"App ID %d", iNewID);
	Log(PB_LOG_INFO,s,L"onAppFocus",__LINE__);

	// GteObjFromID() will return NULL if we're activating an instance of a different plugin
	void *pobject = GetObjFromID (iNewID);

	if (pobject)
		pCurrentSet = (CGestureSet*) pobject;

	return TRUE;
}

/******************************************************************************/
/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 Initial Creation, DCC
*/
BOOL CGestureModule::onBeforeNavigate(int iInstID)
{
	// Clear all current gestures for this instance and force repaint
	void *pobject = GetObjFromID (iInstID);

	if (pobject)
	{
		pCurrentSet = (CGestureSet*) pobject;
		pCurrentSet->Clear ();
		wcscpy (pCurrentSet->sNavigate, L"");
	}
	return TRUE;
}

/******************************************************************************/
/**
* \author	John Heywood (JH, VBC863)
* \date		December 2012 Initial Creation, JH
*/
BOOL CGestureModule::onNavigateComplete(int iInstID)
{
	void *pobject = GetObjFromID (iInstID);

	if (pobject)
	{
		pCurrentSet = (CGestureSet*) pobject;
		return pCurrentSet->NavigateComplete();
	}
	return FALSE;
}


/******************************************************************************/
BOOL CGestureModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	WCHAR empty_string [2];

	// Make sure 'value' isn't a null pointer
	if (!pbMetaStructure->lpValue)
	{
		wcscpy (empty_string, L"");
		pbMetaStructure->lpValue = empty_string;
	}

	// Get pointer to gesture list for other functions to call during this callback
	CGestureSet *pset = (CGestureSet*) pParam;

	if (!_wcsicmp (pbMetaStructure->lpParameter, L"type"))
	{
		// Don't show diagnostics by default
		bGestureDiagnostics = FALSE;

		// Record gesture type being created and set default parameter values
		if (!_wcsicmp (pbMetaStructure->lpValue, L"linear"))
		{
			CurrentGesture = GT_LINEAR;
			return PresetValues (L"left-right", pPBStructure->hWnd);
		}
		else if (!_wcsicmp (pbMetaStructure->lpValue, L"circle"))
		{
			CurrentGesture = GT_CIRCLE;
			return PresetValues (L"happy", pPBStructure->hWnd);
		}
		else if (!_wcsicmp (pbMetaStructure->lpValue, L"hold"))
		{
			CurrentGesture = GT_HOLD;
			return PresetValues (L"centre", pPBStructure->hWnd);
		}
		else if (!_wcsicmp (pbMetaStructure->lpValue, L"tilt"))
		{
			CurrentGesture = GT_TILT;
			return PresetValues (L"face-up", pPBStructure->hWnd);
		}
		else if (!_wcsicmp (pbMetaStructure->lpValue, L"shake"))
		{
			CurrentGesture = GT_SHAKE;
			return PresetValues (L"normal", pPBStructure->hWnd);
		}

		// Unknown type
		return FALSE;
	}
	else if (!_wcsicmp (pbMetaStructure->lpParameter, L"preset"))
	{
		return PresetValues (pbMetaStructure->lpValue, pPBStructure->hWnd);
	}
	else if (!_wcsicmp (pbMetaStructure->lpParameter, L"diagnostics"))
	{
		bGestureDiagnostics = StringToBool (pbMetaStructure->lpValue);
		return TRUE;
	}
	else if (!_wcsicmp (pbMetaStructure->lpParameter, L"id"))
	{
		WSAFECOPY (sGestureID, pbMetaStructure->lpValue);
		return TRUE;
	}
	else if (!_wcsicmp (pbMetaStructure->lpParameter, L"detected"))
	{
		// Navigate destination applies to all gestures in current set
		WSAFECOPY (pset->sNavigate, pbMetaStructure->lpValue);
		//  Modify the code for adding possibility of ruby callback
		pset->bJavascriptEvent = 
			(wcslen(pset->sNavigate) > 1 && wcsncmp(pset->sNavigate, L"/", 1) == 0)
			|| ((wcslen(pset->sNavigate) > wcslen(L"Javascript:")) &&
			(wcsnicmp(pset->sNavigate, L"Javascript:", wcslen(L"Javascript:")) == 0));
		return TRUE;
	}
	else if (!_wcsicmp (pbMetaStructure->lpParameter, L"create"))
	{
		CGestureBase *pgesture;

		switch (CurrentGesture)
		{
		case GT_LINEAR:
			pgesture = new CLinearGesture (ptLinearStart, ptLinearEnd, nLinearTolerance, nLinearSensitivity, nLinearSkew, nLinearDeviation, nLinearRegionWidth);
			break;

		case GT_CIRCLE:
			pgesture = new CCircleGesture (ptCircleCentre, nCircleRadius, nCircleStart, nCircleEnd, nCircleTolerance, nCircleSensitivity);
			break;

		case GT_HOLD:
			pgesture = new CHoldGesture (ptHoldCentre, nHoldRadius, nHoldDelay, nHoldInterval);
			break;

		case GT_TILT:
			pgesture = new CTiltGesture (nTiltX, nTiltY, nTiltZ, nTiltTolerance, nTiltHysteresis);

			if (!EnableIST (SENSOR_TYPE_TILT_ANGLE))
			{
				Log(PB_LOG_WARNING,L"Unable to load the Tilt Sensor API",L"Tilt Gesture",__LINE__);
				return FALSE;
			}

			break;

		case GT_SHAKE:
			pgesture = new CShakeGesture (nShakeThreshold, nShakeQuiet);

			if (!EnableIST (SENSOR_TYPE_ACCELEROMETER))
			{
				Log(PB_LOG_WARNING,L"Unable to load the Shake Sensor API",L"Shake Sensor",__LINE__);
				return FALSE;
			}

			break;

		default:
			return FALSE;
		}

		wcscpy (pgesture->ID, sGestureID);
		pgesture->pDetectedProc = OnDetected;
		pgesture->pRepaintProc = OnRepaint;
		pgesture->bShowDiagnostics = bGestureDiagnostics;
		pgesture->pSet = pset;
		pset->Add (pgesture);

		CurrentGesture = GT_UNKNOWN;

		// Force immediate paint
		InvalidateRect (pPBStructure->hWnd, NULL, true);

		return TRUE;
	}
	else if (!_wcsicmp (pbMetaStructure->lpParameter, L"delete"))
	{
		pset->Remove (sGestureID);
	}

	// Not one of the standard parameters, treat it as gesture specific
	switch (CurrentGesture)
	{
	case GT_LINEAR:
		return StoreLinearParameter (pbMetaStructure->lpParameter, pbMetaStructure->lpValue);

	case GT_CIRCLE:
		return StoreCircleParameter (pbMetaStructure->lpParameter, pbMetaStructure->lpValue);

	case GT_HOLD:
		return StoreHoldParameter (pbMetaStructure->lpParameter, pbMetaStructure->lpValue);

	case GT_TILT:
		return StoreTiltParameter (pbMetaStructure->lpParameter, pbMetaStructure->lpValue);

	case GT_SHAKE:
		return StoreShakeParameter (pbMetaStructure->lpParameter, pbMetaStructure->lpValue);

	default:
		return FALSE;
	}

	// Shouldn't ever get here
	return FALSE;
}

/******************************************************************************/
BOOL CGestureModule::LinearPreset (LPCTSTR ptype, HWND hcontainer)
{
	RECT rect;

	wsprintf (sGestureID, L"linear-%s", ptype);
	if (!_wcsicmp(ptype, L""))
	{
		wsprintf (sGestureID, L"linear-left-right");
	}

	// Get window size
	GetClientRect (hcontainer, &rect);

	if (!_wcsicmp (ptype, L"left-right"))
	{
		ptLinearStart.x = rect.right / 10;
		ptLinearStart.y = rect.bottom / 2;

		ptLinearEnd.x = (rect.right * 9) / 10;
		ptLinearEnd.y = ptLinearStart.y;
	}
	else if (!_wcsicmp (ptype, L"right-left"))
	{
		ptLinearStart.x = (rect.right * 9) / 10;
		ptLinearStart.y = rect.bottom / 2;

		ptLinearEnd.x = rect.right / 10;
		ptLinearEnd.y = ptLinearStart.y;
	}
	else if (!_wcsicmp (ptype, L"top-bottom"))
	{
		ptLinearStart.x = rect.right / 2;
		ptLinearStart.y = rect.bottom / 10;

		ptLinearEnd.x = ptLinearStart.x;
		ptLinearEnd.y = (rect.bottom * 9) / 10;
	}
	else if (!_wcsicmp (ptype, L"bottom-top"))
	{
		ptLinearStart.x = rect.right / 2;
		ptLinearStart.y = (rect.bottom * 9) / 10;

		ptLinearEnd.x = ptLinearStart.x;
		ptLinearEnd.y = rect.bottom / 10;
	}
	else
		return FALSE;

	nLinearTolerance = rect.bottom / 4;
	nLinearSensitivity = 50;
	nLinearSkew = 20;
	nLinearDeviation = 20;
	nLinearRegionWidth = rect.right / 10;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::CirclePreset (LPCTSTR ptype, HWND hcontainer)
{
	RECT rect;

	wsprintf (sGestureID, L"circle-%s", ptype);

	// Get window size
	GetClientRect (hcontainer, &rect);

	if (!_wcsicmp (ptype, L"happy"))
	{
		nCircleStart = 0;
		nCircleEnd = 180;
	}
	else if (!_wcsicmp (ptype, L"sad"))
	{
		nCircleStart = 180;
		nCircleEnd = 360;
	}
	else
		return FALSE;

	ptCircleCentre.x = rect.right / 2;
	ptCircleCentre.y = rect.bottom / 2;
	nCircleRadius = min ((rect.right / 3), (rect.bottom / 3));
	nCircleTolerance = rect.right / 6;
	nCircleSensitivity = 50;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::HoldPreset (LPCTSTR ptype, HWND hcontainer)
{
	RECT rect;

	wsprintf (sGestureID, L"hold-%s", ptype);

	// Get window size
	GetClientRect (hcontainer, &rect);

	if (!_wcsicmp (ptype, L"centre") || !_wcsicmp (ptype, L"center"))
	{
		ptHoldCentre.x = rect.right / 2;
		ptHoldCentre.y = rect.bottom / 2;
		nHoldRadius = min ((rect.right / 4), (rect.bottom / 4));
	}
	else
		return FALSE;

	nHoldDelay = 1000;
	nHoldInterval = 0;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::TiltPreset (LPCTSTR ptype, HWND hcontainer)
{
	wsprintf (sGestureID, L"tilt-%s", ptype);

	if (!_wcsicmp (ptype, L"face-up"))
	{
		nTiltX = 0;
		nTiltY = 0;
		nTiltZ = 90;
	}
	else if (!_wcsicmp (ptype, L"face-down"))
	{
		nTiltX = 0;
		nTiltY = 0;
		nTiltZ = -90;
	}
	else if (!_wcsicmp (ptype, L"upright"))
	{
		nTiltX = 0;
		nTiltY = 90;
		nTiltZ = 0;
	}
	else if (!_wcsicmp (ptype, L"turn-down"))
	{
		nTiltX = 0;
		nTiltY = -90;
		nTiltZ = 0;
	}
	else if (!_wcsicmp (ptype, L"turn-left"))
	{
		nTiltX = 90;
		nTiltY = 0;
		nTiltZ = 0;
	}
	else if (!_wcsicmp (ptype, L"turn-right"))
	{
		nTiltX = -90;
		nTiltY = 0;
		nTiltZ = 0;
	}
	else
		return FALSE;

	nTiltTolerance = 10;
	nTiltHysteresis = 10;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::ShakePreset (LPCTSTR ptype, HWND hcontainer)
{
	wsprintf (sGestureID, L"shake-%s", ptype);

	if (!_wcsicmp (ptype, L"normal"))
	{
		nShakeThreshold = 500;
		nShakeQuiet = 1000;
	}
	else
		return FALSE;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::PresetValues (LPCTSTR ptype, HWND hcontainer)
{
	switch (CurrentGesture)
	{
	case GT_LINEAR:
		return LinearPreset (ptype, hcontainer);

	case GT_CIRCLE:
		return CirclePreset (ptype, hcontainer);

	case GT_HOLD:
		return HoldPreset (ptype, hcontainer);

	case GT_TILT:
		return TiltPreset (ptype, hcontainer);

	case GT_SHAKE:
		return ShakePreset (ptype, hcontainer);

	default:
		return FALSE;
	}
}

/******************************************************************************/
BOOL CGestureModule::StoreLinearParameter (LPCTSTR pname, LPCTSTR pvalue)
{
	// Store parameter value
	if (!_wcsicmp (pname, L"start-x") || !_wcsicmp (pname, L"startx"))
	{
		ptLinearStart.x = _ttoi (pvalue);
		if (ptLinearStart.x < 0)
			ptLinearStart.x = 0;
		if (ptLinearStart.x > 10000)
			ptLinearStart.x = 10000;
	}
	else if (!_wcsicmp (pname, L"start-y") || !_wcsicmp (pname, L"starty"))
	{
		ptLinearStart.y = _ttoi (pvalue);
		if (ptLinearStart.y < 0)
			ptLinearStart.y = 0;
		if (ptLinearStart.y > 10000)
			ptLinearStart.y = 10000;
	}
	else if (!_wcsicmp (pname, L"end-x") || !_wcsicmp (pname, L"endx"))
	{
		ptLinearEnd.x = _ttoi (pvalue);
		if (ptLinearEnd.x < 0)
			ptLinearEnd.x = 0;
		if (ptLinearEnd.x > 10000)
			ptLinearEnd.x = 10000;
	}
	else if (!_wcsicmp (pname, L"end-y") || !_wcsicmp (pname, L"endy"))
	{
		ptLinearEnd.y = _ttoi (pvalue);
		if (ptLinearEnd.y < 0)
			ptLinearEnd.y = 0;
		if (ptLinearEnd.y > 10000)
			ptLinearEnd.y = 10000;
	}
	else if (!_wcsicmp (pname, L"tolerance"))
	{
		nLinearTolerance = _ttoi (pvalue);
		if (nLinearTolerance < 1)
			nLinearTolerance = 1;
		if (nLinearTolerance > 10000)
			nLinearTolerance = 10000;
	}
	else if (!_wcsicmp (pname, L"sensitivity"))
	{
		nLinearSensitivity = _ttoi (pvalue);
		if (nLinearSensitivity < 0)
			nLinearSensitivity = 0;
		if (nLinearSensitivity > 100)
			nLinearSensitivity = 100;
	}
	else if (!_wcsicmp (pname, L"skew"))
	{
		nLinearSkew = _ttoi (pvalue);
		if (nLinearSkew < 0)
			nLinearSkew = 0;
		if (nLinearSkew > 90)
			nLinearSkew = 90;
	}
	else if (!_wcsicmp (pname, L"deviation"))
	{
		nLinearDeviation = _ttoi (pvalue);
		if (nLinearDeviation < 0)
			nLinearDeviation = 0;
		if (nLinearDeviation > 100)
			nLinearDeviation = 100;
	}
	else if (!_wcsicmp (pname, L"region-width") || !_wcsicmp (pname, L"regionwidth"))
	{
		nLinearRegionWidth = _ttoi (pvalue);
		if (nLinearRegionWidth < 1)
			nLinearRegionWidth = 1;
		if (nLinearRegionWidth > 10000)
			nLinearRegionWidth = 10000;
	}
	else
		// Unknown parameter
		return FALSE;

	return TRUE;
}
/******************************************************************************/
BOOL CGestureModule::StoreCircleParameter (LPCTSTR pname, LPCTSTR pvalue)
{
	if (!_wcsicmp (pname, L"centre-x") || !_wcsicmp (pname, L"center-x") ||
		!_wcsicmp (pname, L"centrex") || !_wcsicmp (pname, L"centerx"))
	{
		ptCircleCentre.x = _ttoi (pvalue);
		if (ptCircleCentre.x < -10000)
			ptCircleCentre.x = -10000;
		if (ptCircleCentre.x > 10000)
			ptCircleCentre.x = 10000;
	}
	else if (!_wcsicmp (pname, L"centre-y") || !_wcsicmp (pname, L"center-y") ||
		!_wcsicmp (pname, L"centrey") || !_wcsicmp (pname, L"centery"))
	{
		ptCircleCentre.y = _ttoi (pvalue);
		if (ptCircleCentre.y < -10000)
			ptCircleCentre.y = -10000;
		if (ptCircleCentre.y > 10000)
			ptCircleCentre.y = 10000;
	}
	else if (!_wcsicmp (pname, L"radius"))
	{
		 nCircleRadius = _ttoi (pvalue);
		 if (nCircleRadius < 1)
			 nCircleRadius = 1;
		 if (nCircleRadius > 10000)
			 nCircleRadius = 10000;
	}
	else if (!_wcsicmp (pname, L"start"))
	{
		 nCircleStart = _ttoi (pvalue);
		 if (nCircleStart < 0)
			 nCircleStart = 0;
		 if (nCircleStart > 10000)
			 nCircleStart = 10000;
	}
	else if (!_wcsicmp (pname, L"end"))
	{
		 nCircleEnd = _ttoi (pvalue);
		 if (nCircleEnd < 0)
			 nCircleEnd = 0;
		 if (nCircleEnd > 10000)
			 nCircleEnd = 10000;
	}
	else if (!_wcsicmp (pname, L"tolerance"))
	{
		 nCircleTolerance = _ttoi (pvalue);
		if (nCircleTolerance < 1)
			nCircleTolerance = 1;
		if (nCircleTolerance > 10000)
			nCircleTolerance = 10000;
	}
	else if (!_wcsicmp (pname, L"sensitivity"))
	{
		nCircleSensitivity = _ttoi (pvalue);
		if (nCircleSensitivity < 0)
			nCircleSensitivity = 0;
		if (nCircleSensitivity > 100)
			nCircleSensitivity = 100;
	}
	else
		// Unknown parameter
		return FALSE;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::StoreHoldParameter (LPCTSTR pname, LPCTSTR pvalue)
{
	// Store parameter value
	if (!_wcsicmp (pname, L"centre-x") || !_wcsicmp (pname, L"center-x") ||
		!_wcsicmp (pname, L"centrex") || !_wcsicmp (pname, L"centerx"))
	{
		ptHoldCentre.x = _ttoi (pvalue);
		if (ptHoldCentre.x < -10000)
			ptHoldCentre.x = -10000;
		if (ptHoldCentre.x > 10000)
			ptHoldCentre.x = 10000;
	}
	else if (!_wcsicmp (pname, L"centre-y") || !_wcsicmp (pname, L"center-y") ||
		!_wcsicmp (pname, L"centrey") || !_wcsicmp (pname, L"centery"))
	{
		ptHoldCentre.y = _ttoi (pvalue);
		if (ptHoldCentre.y < -10000)
			ptHoldCentre.y = -10000;
		if (ptHoldCentre.y > 10000)
			ptHoldCentre.y = 10000;
	}
	else if (!_wcsicmp (pname, L"radius"))
	{
		 nHoldRadius = _ttoi (pvalue);
		 if (nHoldRadius < 1)
			 nHoldRadius = 1;
		 if (nHoldRadius > 10000)
			 nHoldRadius = 10000;
	}
	else if (!_wcsicmp (pname, L"delay"))
	{
		nHoldDelay = _ttoi (pvalue);
		if (nHoldDelay < 0)
			nHoldDelay = 0;
	}
	else if (!_wcsicmp (pname, L"interval"))
	{
		 nHoldInterval = _ttoi (pvalue);
		if (nHoldInterval < 0)
			nHoldInterval = 0;
	}
	else
		// Unknown parameter
		return FALSE;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::StoreTiltParameter (LPCTSTR pname, LPCTSTR pvalue)
{
	// Store parameter value
	if (!_wcsicmp (pname, L"targetX"))
	{
		nTiltX = _ttoi (pvalue);
		if (nTiltX < -90)
			nTiltX = 90;
		if (nTiltX > 90)
			nTiltX = 90;
	}
	else if (!_wcsicmp (pname, L"targetY"))
	{
		nTiltY = _ttoi (pvalue);
		if (nTiltY < -90)
			nTiltY = 90;
		if (nTiltY > 90)
			nTiltY = 90;
	}
	else if (!_wcsicmp (pname, L"targetZ"))
	{
		nTiltZ = _ttoi (pvalue);
		if (nTiltZ < -90)
			nTiltZ = 90;
		if (nTiltZ > 90)
			nTiltZ = 90;
	}
	else if (!_wcsicmp (pname, L"tiltTolerance"))
	{
		nTiltTolerance = _ttoi (pvalue);
		if (nTiltTolerance < 0)
			nTiltTolerance = 0;
		if (nTiltTolerance > 90)
			nTiltTolerance = 90;
	}
	else if (!_wcsicmp (pname, L"hysteresis"))
	{
		nTiltHysteresis = _ttoi (pvalue);
		if (nTiltHysteresis < 0)
			nTiltHysteresis = 0;
		if (nTiltHysteresis > 90)
			nTiltHysteresis = 90;
	}
	else
		// Unknown parameter
		return FALSE;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::StoreShakeParameter (LPCTSTR pname, LPCTSTR pvalue)
{
	// Store parameter value
	if (!_wcsicmp (pname, L"threshold"))
	{
		nShakeThreshold = _ttoi (pvalue);
		if (nShakeThreshold < 0)
			nShakeThreshold = 0;
		if (nShakeThreshold > 1000)
			nShakeThreshold = 1000;
	}
	else if (!_wcsicmp (pname, L"quiet"))
	{
		nShakeQuiet = _ttoi (pvalue);
		if (nShakeQuiet < 0)
			nShakeQuiet = 0;
	}
	else
		// Unknown parameter
		return FALSE;

	return TRUE;
}

/******************************************************************************/
BOOL CGestureModule::onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject)
{
	CGestureBase *pgesture;
	CGestureSet *pset = (CGestureSet*) pobject;
	CGestureSetEntry *pentry;
	int x, y;

	// Pass Windows messages to those gestures that want them
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lparam);
		y = HIWORD (lparam);

		// Start a new mouse track
		pset->ClearTrack ();
		pset->AddTrackPoint (x, y);

		pset->Capture ();
		for (pentry = pset->pFirst; pentry; pentry = pentry->pNext)
		{
			pgesture = pentry->pGesture;

			if (pgesture->nHandlesInput & HI_MOUSE)
				pgesture->HandleMouseDown (x, y);
		}
		pset->Release ();

		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lparam);
		y = HIWORD (lparam);

		// Record new point in mouse track
		pset->AddTrackPoint (x, y);
		
		pset->Capture ();
		for (pentry = pset->pFirst; pentry; pentry = pentry->pNext)
		{
			pgesture = pentry->pGesture;

			if (pgesture->nHandlesInput & HI_MOUSE)
				pgesture->HandleMouseMove (x, y);
		}
		pset->Release ();

		break;

	case WM_LBUTTONUP:
		x = LOWORD (lparam);
		y = HIWORD (lparam);

		// Add final point to mouse track
		pset->AddTrackPoint (x, y);

		pset->Capture ();
		for (pentry = pset->pFirst; pentry; pentry = pentry->pNext)
		{
			pgesture = pentry->pGesture;

			if (pgesture->nHandlesInput & HI_MOUSE)
				pgesture->HandleMouseUp (x, y);

			if (pgesture->nHandlesInput & HI_MOUSE_TRACK)
				pgesture->HandleMouseTrack (pset->nTrackPoints, pset->pTrack);
		}
		pset->Release ();
	}

	return FALSE;
}

/******************************************************************************/
void CGestureModule::OnDetected (CGestureBase *pgesture)
{
	CHoldGesture *phold;
	WCHAR s [16];

	switch (pgesture->gtType)
	{
	case GT_HOLD:
		if (wcslen (pgesture->pSet->sNavigate) > 0)
		{
			phold = (CHoldGesture*) pgesture;
			if (!pgesture->pSet->bJavascriptEvent && phold->nDetectCount != 0)
				return;
			wsprintf (s, L"%d", phold->nDetectCount);
			pThis->SendPBNavigate (tcGestureDetectedEventNames, pgesture->pSet->nInstanceID, pgesture->pSet->sNavigate, pgesture->ID, s, NULL);
		}
		break;

	default:
		if (wcslen (pgesture->pSet->sNavigate) > 0)
			pThis->SendPBNavigate (tcGestureDetectedEventNames, pgesture->pSet->nInstanceID, pgesture->pSet->sNavigate, pgesture->ID, L"1", NULL);
		break;
	}
}

/******************************************************************************/
void CGestureModule::OnRepaint (CGestureBase *pgesture)
{
	InvalidateRect (pgesture->pSet->hwndContainer, NULL, true);
}

/******************************************************************************/
void CGestureModule::onPaint (HDC hdc, void *pobject)
{
	CGestureSet *pset = (CGestureSet*) pobject;
	CGestureSetEntry *pentry;
	CGestureBase *pgesture;

	// Draw each gesture in set
	pset->Capture ();
	for (pentry = pset->pFirst; pentry; pentry = pentry->pNext)
	{
		pgesture = pentry->pGesture;
		pgesture->Render (hdc);
	}
	pset->Release ();
}

/******************************************************************************/
BOOL CGestureModule::EnableIST (_SENSOR_TYPE eType)
{
	if (!pSensor)
	{
		// Create the object the first time
		pSensor = new CSENSORSUPPORT();

		if (!pSensor->Supported ())
		{
			delete pSensor;
			pSensor = NULL;
			return FALSE;
		}			

		hSensorThread = CreateThread (NULL, 0, SensorProc, NULL, 0, NULL);
	}

	// Register the type of the sensor
	if (pSensor->RegisterSensor(eType) != E_SENSOR_SUCCESS)
			return FALSE;	

	return TRUE;
}

/******************************************************************************/
void CGestureModule::DisableIST ()
{
	if (pSensor)
	{
		delete pSensor;
		pSensor = NULL;
		WaitForSingleObject (hSensorThread, 5000);
		CloseHandle (hSensorThread);
	}
}

/******************************************************************************/
DWORD CGestureModule::SensorProc (LPVOID pparam)
{
	CGestureSetEntry *pentry;
	CGestureBase *pgesture;
	SENSORDATA data;
	DWORD dwResult = 0;

	while (1)
	{
		dwResult = pThis->pSensor->WaitForSensorData ();
		switch (dwResult)
		{
			case SENSOR_TYPE_UNDEFINED:
				//Cancel event
				break;
			case SENSOR_TYPE_ACCELEROMETER:
			{
				pThis->pSensor->GetLatestShakeData (&data);

				// Pass data to all interested gestures
				pThis->pCurrentSet->Capture ();
				for (pentry = pThis->pCurrentSet->pFirst; pentry; pentry = pentry->pNext)
				{
					pgesture = pentry->pGesture;

					if (pgesture->nHandlesInput & HI_IST_SHAKE)
						pgesture->HandleSensor (data.lXValue, data.lYValue, data.lZValue);
				}
				pThis->pCurrentSet->Release ();
				break;
			}
			case SENSOR_TYPE_TILT_ANGLE:
			{
				pThis->pSensor->GetLatestTiltData (&data);

				// Pass data to all interested gestures
				pThis->pCurrentSet->Capture ();
				for (pentry = pThis->pCurrentSet->pFirst; pentry; pentry = pentry->pNext)
				{
					pgesture = pentry->pGesture;

					if (pgesture->nHandlesInput & HI_IST_TILT)
						pgesture->HandleSensor (data.lXValue, data.lYValue, data.lZValue);
				}
				pThis->pCurrentSet->Release ();
				break;
			}
		}
	}

	return 0;
}

