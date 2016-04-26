#include "stdafx.h"

#include "Paint.h"

/******************************************************************************/
CPaintMap::CPaintMap (int appid, HWND hwnd, PBModule *pmodule, PAINTCALLBACK *pcallback)
{
	nAppID = appid;
	hWnd = hwnd;
	pModule = pmodule;
	pCallback = pcallback;
	pNext = NULL;
}

/******************************************************************************/
CPaintManager::CPaintManager()
{
	pHead = NULL;
}

/******************************************************************************/
CPaintManager::~CPaintManager()
{
	Clear ();
}

/******************************************************************************/
BOOL CPaintManager::AddMap (CPaintMap *pmap)
{
	CPaintMap *pentry = pHead;

	// Add map to list, pNext already initialised to zero in CPaintMap constructor
	if (!pHead)
	{
		pHead = pmap;
		return TRUE;
	}

	// Walk to end of list
	for (pentry = pHead; pentry->pNext; pentry = pentry->pNext)
		;

	// Add new map
	pentry->pNext = pmap;

	return TRUE;
}

/******************************************************************************/
void CPaintManager::Clear()
{
	CPaintMap *pmap, *pnext;

	pmap = pHead;
	while (pmap)
	{
		pnext = pmap->pNext;
		delete pmap;
		pmap = pnext;
	}
}

/******************************************************************************/
void CPaintManager::Process (HWND hwnd, HDC hdc)
{
	CPaintMap *pentry;

	for (pentry = pHead; pentry; pentry = pentry->pNext)
	{
		if (pentry->hWnd == hwnd)
			(pentry->pCallback) (pentry->pModule, pentry->nAppID, hdc);
	}
}
