#pragma once

#include "..\..\Common\Public\PB_Defines.h"

#define MAX_MESSAGES		(WM_USER + 256)
/*
There will be an array of pointers to CMessageMap objects, one per Windows system message up to MAX_MESSAGES - 1.
Each map will be the head of a linked list of maps, each representing a registered instance of a plugin.
*/
/******************************************************************************/
class CMessageMap
{
public:
	int nAppID;		// App which has registered for the message
	HWND hWnd;		// Window to watch for messages; zero => any window
	UINT uMsg;		// Message to watch for
	PBModule *pModule;	// Registered module; passed to static callback in plugin so it has a 'this' pointer
	MESSAGECALLBACK *pCallback;		// Address of static callback function in DLL
	CMessageMap *pNext;

	CMessageMap (int appid, HWND hwnd, UINT msg, PBModule *pmodule, MESSAGECALLBACK *pcallback);
	~CMessageMap ();
};

/******************************************************************************/
class CMessageManager
{
public:
	CMessageManager ();
	~CMessageManager ();

	// Message manager takes ownership of map and will delete object itself
	BOOL AddMap (CMessageMap *pmap);

	void Clear (void);
	BOOL Process (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	CMessageMap **ppMaps;
};
