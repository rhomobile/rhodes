/******************************************************************************/
#pragma once

/******************************************************************************/
#include "Control.h"

/******************************************************************************/
class CControlListEntry
{
public:
	CControlListEntry (CControl *pcontrol);
	~CControlListEntry ();

	CControl *pControl;
	CControlListEntry *pNext;
};

/******************************************************************************/
class CControlList
{
public:
	CControlList ();
	~CControlList ();

	void AddEntry (CControlListEntry *padd);
	CControlListEntry *Find (LPCWSTR id);
	void Clear ();

private:
	CControlListEntry *pFirstEntry;
};
