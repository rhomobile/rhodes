/** 
* \file GestureSet.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>
#include "..\Gesture\GestureBase.h"
#include "GestureModule.h"

/******************************************************************************/
#define MAX_TRACK_POINTS	1024
#define MAX_GESTURES		75		///< The maximum number of gestures which can be created per module.

/******************************************************************************/
/**
* Holds an entry in the gesture set with a reference to the gesture object and pointer to the next entry in the set.
*/
/******************************************************************************/
class CGestureSetEntry
{
public:
	CGestureBase *pGesture;	///< The gesture object for this entry
	CGestureSetEntry *pNext;	///< Pointer to the next entry in the set

	/**
	* Creates a new entry with NULL pointer to next entry
	* \param pgesture The gesture to reference
	*/
	CGestureSetEntry (CGestureBase *pgesture);
	~CGestureSetEntry ();
};

/******************************************************************************/
/**
* Set of gestures for an instance of the plugin
*/
/******************************************************************************/
class CGestureSet
{
public:
	CGestureSet (PPBCORESTRUCT pPBCoreStructure);		///< Initialises an empty set
	~CGestureSet ();

	WCHAR sNavigate [128];	///< Navigation string passed to SendPBNavigate when a gesture is detected.
	HWND hwndContainer;		///< Handle of window containing gesture set; used for repainting.
	int nInstanceID;		///< ID of PBPlugin instance containing gesture set.
	BOOL bJavascriptEvent;	///< Whether or not the Gesture Detected event is a Javascript function

	/** Adds a gesture to the set
	* \param pgesture The gesture to add
	*/
	void Add (CGestureBase *pgesture);

	/** Removes a gesture with the given ID from the set if it exists */
	void Remove (WCHAR* pId);

	/** Clears and destructs all gestures in the set */
	void Clear ();

	BOOL NavigateComplete();

	CGestureSetEntry *pFirst;	///< Pointer to the first entry in the set

	/** \name Members to maintain the mouse track for the set
	* Used by the plugin during Windows mouse message handling.
	*/
	//@{
	int nTrackPoints;	///< Number of points in the track
	POINT *pTrack;		///< Array of points

	void InitTrack (void);	///< Allocates memory for track
	void FreeTrack (void);	///< Frees track memory
	void AddTrackPoint (int x, int y);	///< Adds point to track
	void ClearTrack ();		///< Removes all points from track
	//@}

	void Capture (void);	///< Gets exclusive access to the set (thread safe)
	void Release (void);	///< Releases exclusive access to the set (thread safe)

private:
	CRITICAL_SECTION csAccess;	///< Used to ensure thread safe access to the set
	PPBCORESTRUCT m_pPBCoreStructure;		///< Pointer to callback functions provided by core
	bool m_bActive;	///< Flag cleared during navigate to prevent new gestures being added to cleared lists
};
