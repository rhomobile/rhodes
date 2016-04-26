/** 
* \file GestureModule.h
*/

#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))
#define WSAFECOPY(d,s)	StringCchCopy(d,COUNTOF(d),s)

/******************************************************************************/
#pragma once

/******************************************************************************/
#include "..\..\..\Common\Public\PBPlugin.h"
#include "GestureSet.h"

#include "..\Gesture\GestureBase.h"
#include "..\Gesture\LinearGesture.h"
#include "..\Gesture\CircleGesture.h"
#include "..\Gesture\HoldGesture.h"
#include "..\Gesture\TiltGesture.h"
#include "..\Gesture\ShakeGesture.h"
#include "..\Gesture\ISTSupport.h"

/******************************************************************************/
/** Macro giving count of elements in an array */
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

/** Macro to copy a wide string, guaranteed not to overrun the destination and always to be null terminated */
#define WSAFECOPY(d,s)	StringCchCopy(d,COUNTOF(d),s)

/** Macro to write to PBModule log automatically adding function name and line number */
#define LOG(s,m) Log(s, m,_T(__FUNCTION__), __LINE__)

static TCHAR* tcGestureDetectedEventNames[] = 
{
	L"id",
	L"count",
	NULL
};

/******************************************************************************/
/**
* Gesture plugin module class derived from PBModule.
* Uses CGestureSet objects to hold the gestures for each instance.
*/
/******************************************************************************/
class CGestureModule: public PBModule
{
public:
	CGestureModule (PPBCORESTRUCT pPBCoreStructure);

	/** \name Overrides of functions in PBModule base class */
	//@{
	BOOL onInit(PPBSTRUCT pPBStructure);
	void onDeInit(PPBSTRUCT pPBStructure);
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL onBeforeNavigate (int iInstID);
	BOOL onNavigateComplete	(int iInstID);
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
	BOOL onAppFocus(int iOldID,int iNewID);
	BOOL onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject);
	void onPaint (HDC hdc, void *pobject);
	void DisableIST (void);	///< Terminates IST processing thread and frees wrapper object
	//@}

private:
	static CGestureModule *pThis;	///< Reference to single instance of this class which static fucntions can use during callbacks

	/**
	* Called by gesture class when a gesture is detected. Calls SendPBNavigate to notify core.
	* \param pgesture Detected gesture
	*/
	static void OnDetected (CGestureBase *pgesture);

	/**
	* Called by gesture class when it wants to force a repaint because new diagnostic information is available.
	* Invalidates the window hosting the gesture.
	* \param pgesture Gesture wanting to repaint
	*/
	static void OnRepaint (CGestureBase *pgesture);

	CGestureSet *pCurrentSet;	///< The gesture set for the currently active instance; used by functions which are not overrides of PBModule functions and therefore don't have access to pWrappedObj (e.g. the IST thread).
	GestureType CurrentGesture;	///< The type of gesture currently being created, maintained between calls to MetaProc.
	WCHAR sGestureID [64];	///< ID string of gesture currently being created, maintained between calls to MetaProc.
	bool bGestureDiagnostics;	///< Flag to enable diagnostic drawing for gesture currently being created, maintained between calls to MetaProc.

	/** \name IST support */
	//@{
	CSENSORSUPPORT *pSensor;	///< Reference to the Sensor wrapper which gives access to underlying IST subsystem	
	HANDLE hSensorThread;	///< Handle of thread which continually receives Tilt Sensor data and passes to the gesture classes
	
	///< Thread procedure which continually receives Tilt Sensor data and passes to the gesture classes
	static DWORD CGestureModule::SensorProc (LPVOID pparam);

	///< Thread procedure which continually receives Shake Sensor data and passes to the gesture classes
	static DWORD CGestureModule::SensorShakeProc (LPVOID pparam);	
	BOOL EnableIST (_SENSOR_TYPE eType);	///< Creates IST wrapper object and creates processing thread, if not already enabled
	//@}

	/** \name Parsers for name/value pairs from MetaProc */
	//@{
	BOOL StoreLinearParameter (LPCTSTR pname, LPCTSTR pvalue);
	BOOL StoreCircleParameter (LPCTSTR pname, LPCTSTR pvalue);
	BOOL StoreHoldParameter (LPCTSTR pname, LPCTSTR pvalue);
	BOOL StoreTiltParameter (LPCTSTR pname, LPCTSTR pvalue);
	BOOL StoreShakeParameter (LPCTSTR pname, LPCTSTR pvalue);
	//@}

	/**
	* Sets parameters for predefined gestures; calls gesture specific function
	* \param ptype Name of predefined gesture type
	* \param hcontainer Handle of window holding the gesture
	*/
	BOOL PresetValues (LPCTSTR ptype, HWND hcontainer);

	/**
	* \name Functions to set parameters for predefined gestures; called by PresetValues */
	//@{
	BOOL LinearPreset (LPCTSTR ptype, HWND hcontainer);
	BOOL CirclePreset (LPCTSTR ptype, HWND hcontainer);
	BOOL HoldPreset (LPCTSTR ptype, HWND hcontainer);
	BOOL TiltPreset (LPCTSTR ptype, HWND hcontainer);
	BOOL ShakePreset (LPCTSTR ptype, HWND hcontainer);
	//@}

	/** \name Parameters defining CLinearGesture being created, maintained between calls to MetaProc. */
	//@{
	POINT ptLinearStart, ptLinearEnd;
	int nLinearTolerance, nLinearSensitivity, nLinearSkew, nLinearDeviation, nLinearRegionWidth;
	//@}

	/** \name Parameters defining circle gesture being created, maintained between calls to MetaProc. */
	//@{
	POINT ptCircleCentre;
	int nCircleRadius, nCircleStart, nCircleEnd, nCircleTolerance, nCircleSensitivity;
	//@}

	/** \name Parameters defining hold gesture being created, maintained between calls to MetaProc. */
	//@{
	POINT ptHoldCentre;
	int nHoldRadius, nHoldDelay, nHoldInterval;
	//@}

	/** \name Parameters defining tilt gesture being created, maintained between calls to MetaProc. */
	//@{
	int nTiltX, nTiltY, nTiltZ;
	int nTiltTolerance, nTiltHysteresis;
	//@}

	/** \name Parameters defining shake gesture being created, maintained between calls to MetaProc. */
	//@{
	int nShakeThreshold, nShakeQuiet;
	//@}

	PPBCORESTRUCT m_pPBCoreStructure;		///< Pointer to callback functions provided by core
};
