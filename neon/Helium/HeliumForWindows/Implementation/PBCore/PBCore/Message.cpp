#include "stdafx.h"

#include "Message.h"

/******************************************************************************/
CMessageMap::CMessageMap (int appid, HWND hwnd, UINT msg, PBModule *pmodule, MESSAGECALLBACK *pcallback)
{
	nAppID = appid;
	hWnd = hwnd;
	uMsg = msg;
	pModule = pmodule;
	pCallback = pcallback;
	pNext = NULL;
}

/******************************************************************************/
CMessageMap::~CMessageMap ()
{
}

/******************************************************************************/
CMessageManager::CMessageManager()
{
	// Allocate and zero array
	ppMaps = new CMessageMap* [MAX_MESSAGES];
	memset (ppMaps, 0, sizeof (CMessageMap*) * MAX_MESSAGES);
}

/******************************************************************************/
CMessageManager::~CMessageManager()
{
	// Delete all map objects
	Clear ();

	// Free array itself
	delete [] ppMaps;
}

/******************************************************************************/
BOOL CMessageManager::AddMap (CMessageMap *pmap)
{
	// Range check
	if (pmap->uMsg >= MAX_MESSAGES)
	{
		delete pmap;
		return FALSE;
	}

	// Get head of map list for this message
	CMessageMap *pentry = ppMaps [pmap->uMsg];

	// Store pointer to new map if message not already registered
	// (pNext already initialised to zero in CMessageMap constructor)
	if (!pentry)
	{
		ppMaps [pmap->uMsg] = pmap;
		return TRUE;
	}

	// Walk to end of list
	while (pentry->pNext) {
        if ((pentry->uMsg == pmap->uMsg) &&
            (pentry->pModule == pmap->pModule) &&
            (pentry->pCallback == pmap->pCallback) &&
            (pentry->hWnd == pmap->hWnd) &&
            (pentry->nAppID == pmap->nAppID)) {
            // ignore duplicates
            return TRUE;
        }
		pentry = pentry->pNext;
	}

    if ((pentry->uMsg != pmap->uMsg) ||
        (pentry->pModule != pmap->pModule) ||
        (pentry->pCallback != pmap->pCallback) ||
        (pentry->hWnd != pmap->hWnd) ||
        (pentry->nAppID != pmap->nAppID)) {
       	// Add new map if it doesn't exist
		pentry->pNext = pmap;
    }
	return TRUE;
}

/******************************************************************************/
void CMessageManager::Clear()
{
	CMessageMap *pentry, *pnext;
	int n;

	// Walk each list and delete maps
	for (n = 0; n < MAX_MESSAGES; n++)
	{
		pentry = ppMaps [n];

		while (pentry)
		{
			pnext = pentry->pNext;
			delete pentry;
			pentry = pnext;
		}
	}
}

/******************************************************************************/
BOOL CMessageManager::Process (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Range check
	if (msg >= MAX_MESSAGES)
		return FALSE;

	// Get pointer to head of list for this message
	CMessageMap *pentry = ppMaps [msg];

	// Walk list looking for matching window handle
	while (pentry)
	{
		if (pentry->hWnd == 0 || pentry->hWnd == hwnd)
			if ((*(pentry->pCallback)) (pentry->pModule, pentry->nAppID, hwnd, msg, wparam, lparam))
				// Return TRUE to halt processing of this message if plugin returned TRUE itself
				return TRUE;

		pentry = pentry->pNext;
	}

	return FALSE;
}
