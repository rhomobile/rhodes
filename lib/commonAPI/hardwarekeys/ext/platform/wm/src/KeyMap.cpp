#include "KeyMap.h"

CKeyMapEntry::CKeyMapEntry ()
{
	nVirtualCode = -1;
	psNavigate = NULL;
	bDispatch = FALSE;
	nRemap = 0;
	pNext = NULL;
}

CKeyMapEntry::CKeyMapEntry (int code, rho::apiGenerator::CMethodResult pnavigate, BOOL dispatch)
{
	nVirtualCode = code;
	psNavigate = pnavigate;
	bDispatch = dispatch;
	nRemap = 0;
	pNext = NULL;
}

/******************************************************************************/
CKeyMapEntry::CKeyMapEntry (int code, int remap)
{
	nVirtualCode = code;
	psNavigate = NULL;
	if(remap==0)
		bDispatch=true;
	else
		bDispatch = false;
	nRemap = remap;
	pNext = NULL;
}

CKeyMapEntry::~CKeyMapEntry () {}
	
void CKeyMapEntry::SetNavigate (rho::apiGenerator::CMethodResult pCallback)
{
	psNavigate = pCallback;
}

CKeyMap::CKeyMap ()
{
	pFirstEntry = NULL;
}

CKeyMap::~CKeyMap ()
{
	Clear ();
}

void CKeyMap::AddEntry (CKeyMapEntry *padd)
{
	// Add new entry to end of list, or start new list
	if (pFirstEntry)
	{
		CKeyMapEntry *pentry = pFirstEntry;

		while (pentry->pNext)
			pentry = pentry->pNext;

		pentry->pNext = padd;
	}
	else
		pFirstEntry = padd;

	padd->pNext = NULL;
}

CKeyMapEntry *CKeyMap::Find (int code)
{
	CKeyMapEntry *pentry = pFirstEntry;

	// Return matching map entry for specified virtual code, or NULL if not found
	while (pentry)
	{
		if (pentry->nVirtualCode == code)
			return pentry;

		pentry = pentry->pNext;
	}

	return NULL;
}

//  DCC - Implemented this function.  Well done old chap.
void CKeyMap::DeleteEntry (int code)
{
	CKeyMapEntry *pentry = pFirstEntry;
	CKeyMapEntry *pPrevEntry = NULL;

	while (pentry)
	{
		if (pentry->nVirtualCode == code)
		{
			//  We have found the entry to delete
			if (pPrevEntry != NULL)
			{
				//  Not deleting the First entry, set the previous' 
				//  next pointer
				pPrevEntry->pNext = pentry->pNext;
			}
			else
			{
				pFirstEntry = NULL;
			}
			delete pentry;
			pentry = NULL;
			return;
		}
		pPrevEntry = pentry;
		pentry = pentry->pNext;
	}
}

void CKeyMap::Clear (void)
{
	CKeyMapEntry *pentry = pFirstEntry, *pnext;

	// Free list
	while (pentry)
	{
		pnext = pentry->pNext;
		delete pentry;
		pentry = pnext;
	}

	pFirstEntry = NULL;
}

bool CKeyMap::HasContents(void)
{
	return pFirstEntry != NULL;
}
