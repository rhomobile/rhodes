#include "GestureSet.h"

/******************************************************************************/
CGestureSetEntry::CGestureSetEntry (CGestureBase *pgesture)
{
	pGesture = pgesture;
	pNext = NULL;
}

/******************************************************************************/
CGestureSetEntry::~CGestureSetEntry ()
{
	delete pGesture;
}

/******************************************************************************/
CGestureSet::CGestureSet (PPBCORESTRUCT pPBCoreStructure)
{
	InitializeCriticalSection (&csAccess);
	pFirst = NULL;
	wcscpy (sNavigate, L"");
	InitTrack ();
	m_pPBCoreStructure = pPBCoreStructure;
	m_bActive = true;
}

/******************************************************************************/
CGestureSet::~CGestureSet ()
{
	Clear ();
	FreeTrack ();
	DeleteCriticalSection (&csAccess);
}

/******************************************************************************/
void CGestureSet::Clear ()
{
	CGestureSetEntry *pentry = pFirst, *pnext;

	Capture ();
	m_bActive = false;
	while (pentry)
	{
		pnext = pentry->pNext;
		delete pentry;
		pentry = pnext;
	}
	pFirst = NULL;
	m_bActive = true;
	Release ();

	//  If there are no gestures present on the page we can
	//  re-enable scrolling (core can handle logic of whether the user
	//  wants scrolling or not)
	m_pPBCoreStructure->pDisableScrolling(FALSE);
}

BOOL CGestureSet::NavigateComplete()
{
	return TRUE;
}

/******************************************************************************/
void CGestureSet::Add (CGestureBase *pgesture)
{
	Capture ();

	// if the gesture set has been deactivated don't add a new gesture
	if (!m_bActive)
		goto Exit;

	// Is list empty?
	if (!pFirst)
	{
		pFirst = new CGestureSetEntry (pgesture);
		goto Exit;
	}

	// Get last entry in list
	CGestureSetEntry *pentry;
	//  And count how many there are
	int iGestureCount = 1;

	for (pentry = pFirst; pentry->pNext; pentry = pentry->pNext)
		iGestureCount++;

	if (iGestureCount > MAX_GESTURES)
	{
		DEBUGMSG(TRUE,(L"Unable to create gesture, too many already exist\n"));
		goto Exit;
	}

	// Point to new entry
	pentry->pNext = new CGestureSetEntry (pgesture);

	//  If there are gestures present on the page we need to disable scrolling
	m_pPBCoreStructure->pDisableScrolling(TRUE);

Exit:
	Release ();

	int n = 0;
	for (pentry = pFirst; pentry; pentry = pentry->pNext)
		n++;
	DEBUGMSG(TRUE,(L"%d gestures in set:\n", n));

	return;
}

/******************************************************************************/
void CGestureSet::Remove (WCHAR* pId)
{
	CGestureSetEntry *pentry = pFirst;
	BOOL bInvalidateRequired = false;

	Capture ();
	while (pentry)
	{
		if (pFirst && (wcscmp(pId, pFirst->pGesture->ID) == 0))
		{
			if (pentry->pGesture->bShowDiagnostics)
				bInvalidateRequired = true;

			pFirst = pFirst->pNext;
			delete(pentry);
			pentry = pFirst;
		}
		else
		{
			if (pentry->pNext && (wcscmp(pId, pentry->pNext->pGesture->ID) == 0))
			{
				CGestureSetEntry *pDeleteMe = pentry->pNext;
				if (pDeleteMe->pGesture->bShowDiagnostics)
					bInvalidateRequired = true;

				pentry->pNext = pentry->pNext->pNext;
				delete(pDeleteMe);
			}
			pentry = pentry->pNext;
		}
	}
	Release ();

	if (bInvalidateRequired)
		InvalidateRect (hwndContainer, NULL, true);

	//  If there are no gestures present on the page we can
	//  re-enable scrolling (core can handle logic of whether the user
	//  wants scrolling or not)
	if (!pFirst)
		m_pPBCoreStructure->pDisableScrolling(FALSE);

	return;
}


/******************************************************************************/
// Mouse track handling

/******************************************************************************/
void CGestureSet::InitTrack (void)
{
	pTrack = new POINT [MAX_TRACK_POINTS];
	nTrackPoints = 0;
}

/******************************************************************************/
void CGestureSet::FreeTrack (void)
{
	if (pTrack)
	{
		delete [] pTrack;
		pTrack = 0;
		nTrackPoints = 0;
	}
}

/******************************************************************************/
void CGestureSet::AddTrackPoint (int x, int y)
{
	if (nTrackPoints < MAX_TRACK_POINTS)
	{
		pTrack [nTrackPoints].x = x;
		pTrack [nTrackPoints].y = y;

		nTrackPoints++;
	}
}

/******************************************************************************/
void CGestureSet::ClearTrack ()
{
	nTrackPoints = 0;
}

/******************************************************************************/
void CGestureSet::Capture (void)
{
	EnterCriticalSection (&csAccess);
}

/******************************************************************************/
void CGestureSet::Release (void)
{
	LeaveCriticalSection (&csAccess);
}