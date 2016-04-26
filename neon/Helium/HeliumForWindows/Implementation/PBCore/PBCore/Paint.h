#pragma once

#include <windows.h>
#include "..\..\Common\Public\PB_Defines.h"

/*
Holds a linked list of CPaintMap pointers, each representing a plugin which has
registered for paint notification.
*/

/******************************************************************************/
class CPaintMap
{
public:
	int nAppID;		// App which has registered
	HWND hWnd;		// Window to watch for paint
	PBModule *pModule;	// Registered module; passed to static callback in plugin so it has a 'this' pointer
	PAINTCALLBACK *pCallback;		// Address of static callback function in DLL
	CPaintMap *pNext;

	CPaintMap (int appid, HWND hwnd, PBModule *pmodule, PAINTCALLBACK *pcallback);
};

/******************************************************************************/
class CPaintManager
{
public:
	CPaintManager ();
	~CPaintManager ();

	// Paint manager takes ownership of map and will delete object itself
	BOOL AddMap (CPaintMap *pmap);

	void Clear (void);
	void Process (HWND hwnd, HDC hdc);

private:
	CPaintMap *pHead;
};
