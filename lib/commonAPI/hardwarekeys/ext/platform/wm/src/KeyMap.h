/** 
* \file KeyMap.h
* Maps virtual key codes to actions - replace, ignore, navigate string
*/

#pragma once

#include <windows.h>
#include "api_generator/MethodResult.h"

class CKeyMapEntry
{
public:
	CKeyMapEntry ();
	CKeyMapEntry (int code, rho::apiGenerator::CMethodResult pnavigate, BOOL dispatch);
	CKeyMapEntry (int code, int remap);
	~CKeyMapEntry ();

public:
	int nVirtualCode;
	rho::apiGenerator::CMethodResult psNavigate;
	BOOL bDispatch;
	int nRemap;
	CKeyMapEntry *pNext;

	void SetNavigate (rho::apiGenerator::CMethodResult pCallback);
};

class CKeyMap
{
public:
	CKeyMap ();
	~CKeyMap ();

	void AddEntry (CKeyMapEntry* padd);
	CKeyMapEntry *Find (int code);
	void DeleteEntry (int code);
	void Clear (void);
	bool HasContents(void);

private:
	CKeyMapEntry *pFirstEntry;
};
