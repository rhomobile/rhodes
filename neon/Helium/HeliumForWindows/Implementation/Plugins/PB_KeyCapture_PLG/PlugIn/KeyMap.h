/** 
* \file KeyMap.h
* Maps virtual key codes to actions - replace, ignore, navigate string
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>

/******************************************************************************/
/******************************************************************************/
class CKeyMapEntry
{
public:
	CKeyMapEntry ();
	CKeyMapEntry (int code, LPCWSTR pnavigate, BOOL dispatch);
	CKeyMapEntry (int code, int remap);
	~CKeyMapEntry ();

public:
	int nVirtualCode;
	WCHAR *psNavigate;
	BOOL bDispatch;
	int nRemap;
	CKeyMapEntry *pNext;

	void SetNavigate (LPCWSTR navigate);
};

/******************************************************************************/
/******************************************************************************/
class CKeyMap
{
public:
	CKeyMap ();
	~CKeyMap ();

	void AddEntry (CKeyMapEntry* padd);
	CKeyMapEntry *Find (int code);
	void DeleteEntry (int code);
	void Clear (void);

private:
	CKeyMapEntry *pFirstEntry;
};
