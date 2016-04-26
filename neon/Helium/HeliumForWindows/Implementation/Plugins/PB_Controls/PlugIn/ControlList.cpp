/******************************************************************************/
#include "ControlList.h"

/******************************************************************************/
/******************************************************************************/
CControlListEntry::CControlListEntry (CControl *pcontrol)
{
	pControl = pcontrol;
	pNext = NULL;
}

/******************************************************************************/
CControlListEntry::~CControlListEntry ()
{
	delete pControl;
}

/******************************************************************************/
/******************************************************************************/
CControlList::CControlList ()
{
	pFirstEntry = NULL;
}

/******************************************************************************/
CControlList::~CControlList ()
{
	Clear ();
}

/******************************************************************************/
void CControlList::AddEntry (CControlListEntry *padd)
{
	// Add new entry to end of list, or start new list
	if (pFirstEntry)
	{
		CControlListEntry *pentry = pFirstEntry;

		while (pentry->pNext)
			pentry = pentry->pNext;

		pentry->pNext = padd;
	}
	else
		pFirstEntry = padd;

	padd->pNext = NULL;
}

/******************************************************************************/
void CControlList::Clear ()
{
	CControlListEntry *pentry = pFirstEntry, *pnext;

	// Free list
	while (pentry)
	{
		pnext = pentry->pNext;
		delete pentry;
		pentry = pnext;
	}

	pFirstEntry = NULL;
}

/******************************************************************************/
CControlListEntry *CControlList::Find (LPCWSTR id)
{
	CControlListEntry *pentry = pFirstEntry;

	// Return matching map entry for specified ID, or NULL if not found
	while (pentry)
	{
		if (!wcsicmp (pentry->pControl->GetID (), id))
			return pentry;

		pentry = pentry->pNext;
	}

	return NULL;
}
