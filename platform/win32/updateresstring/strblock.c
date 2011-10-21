/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.

   Copyright 1998 Microsoft Corporation.  All Rights Reserved.
**************************************************************************/

/**************************************************************************

   File:          strblock.c

   Description:   Implements the functions that manipulate a string block.

**************************************************************************/
#include "strblock.h"


// The format of string resources is explained below.
//
// The smallest granularity of string resource that can be loaded/updated is a block.
// Each block is identified by an ID, starting with 1. You need to use the block ID
// when calling FindResource(), LoadResource(), UpdateResource().
//
// A string with ID, nStringID, is in the block with ID, nBlockID, given by the following
// formula:
//                    nBlockID = (nStringID / 16) + 1; // Note integer division.
//
// A block of string resource is laid out as follows:
// Each block has NO_OF_STRINGS_PER_BLOCK (= 16) strings. Each string is represented as
// an ordered pair, (LENGTH, TEXT). The LENGTH is a WORD that specifies the size, in terms
// of number of characters, of the string that follows. TEXT follows the LENGTH and is
// a sequence of UNICODE characters, NOT terminated by a NULL character.Any TEXT may be of
// zero-length, in which case, LENGTH is zero.
//
// An executable does not have a string table block with ID, nBlockID, if it does not have any
// strings with IDs - ((nBlockID - 1) * 16) thru' ((nBlockID * 16) - 1).
//
// This format is the same for Windows NT, Windows 95 & Windows 98. Yes, strings in a resource
// are internally stored in UNICODE format even in Windows 95 & Windows 98.


// Internal data structure format for a string block.
// Our block of strings has as an array of UNICODE string pointers.

typedef struct tagSTRINGBLOCK
{
	UINT		nBlockID;	// The ID of the block.
	WORD		wLangID;	// The language ID.
	LPWSTR		strArray[NO_OF_STRINGS_PER_BLOCK];	// We maintain the strings
							// internally in UNICODE.
} STRINGBLOCK, * PSTRINGBLOCK;


// A thread-specific error number for the last block operation.
__declspec(thread) STRBLOCKERR g_strBlockErr = STRBLOCKERR_OK;

// Set the error code.
void SetBlockError( STRBLOCKERR err ) { g_strBlockErr = err; }


// Forward declarations.

// Create a string block & return the pointer to the block. Return NULL on failure.
// Sets the error code.
PSTRINGBLOCK CreateBlock( HINSTANCE hInstLib, UINT nBlockID, WORD wLangID );

// Parse the string block resource pointed at by, pParse, and fill the strings in pStrBlock.
BOOL ParseRes( LPVOID pRes, PSTRINGBLOCK pStrBlock );

// Get the size of the raw string block resource in the given block.
DWORD GetResSize( PSTRINGBLOCK pStrBlock );

// Update a block of string in the specified library.
// hUpdate specifies the update-file handle. This handle is returned by the BeginUpdateResource.
// pStrBlock contains the new strings.
// nBlockID specifies the ID of the block. Use the same block ID as of pStrBlock if this value is -1.
// wlangID specifies the language ID of the block. Use the same language ID as of pStrBlock, if this value is 0.
// Returns TRUE on success and FALSE on failure.
// Sets the error code.
BOOL UpdateBlock( HANDLE hUpdate, PSTRINGBLOCK pStrBlock, int nBlockID, WORD wLangID );

// Use the strings in the block, pStrBloc, and build a buffer whose format matches that of the
// string resource block that can be used to update string resource.
// pRes points to a buffer that gets filled. It must be large enough to hold the entire block.
// To figure out the size needed, call GetResSize().
VOID BuildRes( PSTRINGBLOCK pStrBlock, LPVOID pRes );


// Create a string block.

HSTRBLOCK WINAPI GetStringBlockA( LPCSTR strAppName, UINT nBlockID, WORD wLangID )
{
	PSTRINGBLOCK pStrBlock = NULL;
	HINSTANCE hInstLib = NULL;

	hInstLib = LoadLibraryExA(
					strAppName,
					NULL,
					DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE
							 );

	if( NULL == hInstLib )
	{
		SetBlockError(STRBLOCKERR_APPLOADFAILED);
		return NULL;
	}

	// Create the block of strings.
	pStrBlock = CreateBlock(hInstLib, nBlockID, wLangID);

	// Free the library.
	FreeLibrary(hInstLib);

	if( pStrBlock )
		SetBlockError(STRBLOCKERR_OK);

	return (HSTRBLOCK)pStrBlock;
}


// Create a string block.

HSTRBLOCK WINAPI GetStringBlockW( LPCWSTR strAppName, UINT nBlockID, WORD wLangID )
{
	PSTRINGBLOCK pStrBlock = NULL;
	HINSTANCE hInstLib = NULL;

	hInstLib = LoadLibraryExW(
					strAppName,
					NULL,
					DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE
							 );

	if( NULL == hInstLib )
	{
		SetBlockError(STRBLOCKERR_APPLOADFAILED);
		return NULL;
	}

	// Create the block of strings.
	pStrBlock = CreateBlock(hInstLib, nBlockID, wLangID);

	// Free the library.
	FreeLibrary(hInstLib);

	if( pStrBlock )
		SetBlockError(STRBLOCKERR_OK);

	return (HSTRBLOCK)pStrBlock;
}


BOOL WINAPI DeleteStringBlock( HSTRBLOCK hStrBlock )
{
	PSTRINGBLOCK pStrBlock = (PSTRINGBLOCK)hStrBlock;
	int			 i;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return FALSE;
	}

	for( i = 0; i < NO_OF_STRINGS_PER_BLOCK; i++ )
	{
		if( pStrBlock->strArray[i] )
			free(pStrBlock->strArray[i]);
	}
	free( pStrBlock);

	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


int WINAPI GetStringLength( HSTRBLOCK hStrBlock, UINT nIndex )
{
	int				nLen;
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return -1;
	}
	if( (nIndex < 0) || (nIndex >= NO_OF_STRINGS_PER_BLOCK) )
	{
		SetBlockError(STRBLOCKERR_INVALIDINDEX);
		return -1;
	}

	nLen = wcslen(pStrBlock->strArray[nIndex]);
	SetBlockError(STRBLOCKERR_OK);

	return nLen;
}


BOOL WINAPI GetStringA( HSTRBLOCK hStrBlock, UINT nIndex, LPSTR pszText )
{
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return FALSE;
	}
	if( (nIndex < 0) || (nIndex >= NO_OF_STRINGS_PER_BLOCK) )
	{
		SetBlockError(STRBLOCKERR_INVALIDINDEX);
		return FALSE;
	}
	if( NULL == pszText )
	{
		SetBlockError(STRBLOCKERR_STRINVALID);
		return FALSE;
	}

	if( !WideCharToMultiByte(CP_ACP, 0, pStrBlock->strArray[nIndex], -1, pszText,
			wcslen(pStrBlock->strArray[nIndex]) + 1, NULL, NULL) )
	{
		SetBlockError(STRBLOCKERR_UNKNOWN);
		return FALSE;
	}

	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


BOOL WINAPI GetStringW( HSTRBLOCK hStrBlock, UINT nIndex, LPWSTR pszText )
{
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return FALSE;
	}
	if( (nIndex < 0) || (nIndex >= NO_OF_STRINGS_PER_BLOCK) )
	{
		SetBlockError(STRBLOCKERR_INVALIDINDEX);
		return FALSE;
	}
	if( NULL == pszText )
	{
		SetBlockError(STRBLOCKERR_STRINVALID);
		return FALSE;
	}

	wcscpy(pszText, pStrBlock->strArray[nIndex]);
	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


BOOL WINAPI SetStringA( HSTRBLOCK hStrBlock, UINT nIndex, LPCSTR pszText )
{
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;
	int				nLen;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return FALSE;
	}
	if( (nIndex < 0) || (nIndex >= NO_OF_STRINGS_PER_BLOCK) )
	{
		SetBlockError(STRBLOCKERR_INVALIDINDEX);
		return FALSE;
	}

	// Delete the current string & reallocate a new one..
	free(pStrBlock->strArray[nIndex]);

	nLen = strlen(pszText) + 1;
	pStrBlock->strArray[nIndex] = (LPWSTR)malloc( sizeof(WCHAR) * nLen);

	if( NULL == pStrBlock->strArray[nIndex] )
	{
		SetBlockError(STRBLOCKERR_NOMEMORY);
		return FALSE;
	}

	if( !MultiByteToWideChar(CP_ACP, 0, pszText, -1, pStrBlock->strArray[nIndex], nLen) )
	{
		SetBlockError(STRBLOCKERR_UNKNOWN);
		return FALSE;
	}

	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


BOOL WINAPI SetStringW( HSTRBLOCK hStrBlock, UINT nIndex, LPCWSTR pszText )
{
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;
	int				nLen;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return FALSE;
	}
	if( (nIndex < 0) || (nIndex >= NO_OF_STRINGS_PER_BLOCK) )
	{
		SetBlockError(STRBLOCKERR_INVALIDINDEX);
		return FALSE;
	}

	// Delete the current string & reallocate a new one..
	free(pStrBlock->strArray[nIndex]);
	nLen = wcslen(pszText) + 1;

	pStrBlock->strArray[nIndex] = (LPWSTR)malloc( sizeof(WCHAR) * nLen);

	if( NULL == pStrBlock->strArray[nIndex] )
	{
		SetBlockError(STRBLOCKERR_NOMEMORY);
		return FALSE;
	}

	wcscpy(pStrBlock->strArray[nIndex], pszText);
	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


int WINAPI GetFirstStringID( HSTRBLOCK hStrBlock )
{
	PSTRINGBLOCK pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return -1;
	}
	SetBlockError(STRBLOCKERR_OK);

	return (pStrBlock->nBlockID - 1) * NO_OF_STRINGS_PER_BLOCK;
}


int WINAPI GetBlockID( HSTRBLOCK hStrBlock )
{
	PSTRINGBLOCK pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return -1;
	}
	SetBlockError(STRBLOCKERR_OK);

	return pStrBlock->nBlockID;
}


WORD WINAPI GetBlockLanguage( HSTRBLOCK hStrBlock )
{
	PSTRINGBLOCK pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return -1;
	}

	SetBlockError(STRBLOCKERR_OK);
	return pStrBlock->wLangID;
}


BOOL WINAPI UpdateStringBlockA( LPCSTR strAppName, HSTRBLOCK hStrBlock, int nBlockID, WORD wLangID )
{
	HANDLE			hUpdate;
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return -1;
	}

	hUpdate = BeginUpdateResourceA(strAppName, FALSE);

	if( NULL == hUpdate )
	{
		DWORD dwError = GetLastError();

		switch( dwError )
		{
		case ERROR_CALL_NOT_IMPLEMENTED:

			SetBlockError(STRBLOCKERR_UPDATENOTIMPLEMENTED);
			break;

		default:

			SetBlockError(STRBLOCKERR_UPDATEFAILED);
			break;
		}

		return FALSE;
	}

	// Update the resource.
	if( !UpdateBlock(hUpdate, pStrBlock, nBlockID, wLangID) )
	{
		EndUpdateResource(hUpdate, FALSE);
		return FALSE;
	}

	if( !EndUpdateResource(hUpdate, FALSE) )
	{
		SetBlockError(STRBLOCKERR_UPDATEFAILED);
		return FALSE;
	}

	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


BOOL WINAPI UpdateStringBlockW( LPCWSTR strAppName, HSTRBLOCK hStrBlock, int nBlockID, WORD wLangID )
{
	HANDLE			hUpdate;
	PSTRINGBLOCK	pStrBlock = (PSTRINGBLOCK)hStrBlock;

	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_INVALIDBLOCK);
		return -1;
	}

	hUpdate = BeginUpdateResourceW(strAppName, FALSE);

	if( NULL == hUpdate )
	{
		DWORD dwError = GetLastError();

		switch( dwError )
		{
		case ERROR_CALL_NOT_IMPLEMENTED:

			SetBlockError(STRBLOCKERR_UPDATENOTIMPLEMENTED);
			break;

		default:

			SetBlockError(STRBLOCKERR_UPDATEFAILED);
			break;
		}

		return FALSE;
	}

	// Update the resource.
	if( !UpdateBlock(hUpdate, pStrBlock, nBlockID, wLangID) )
	{
		EndUpdateResource(hUpdate, FALSE);
		return FALSE;
	}

	if( !EndUpdateResource(hUpdate, FALSE) )
	{
		SetBlockError(STRBLOCKERR_UPDATEFAILED);
		return FALSE;
	}

	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}


STRBLOCKERR WINAPI GetStringBlockError()
{
	return g_strBlockErr;
}


// Create a string block & return the pointer to the block. Return NULL on failure.

PSTRINGBLOCK CreateBlock( HINSTANCE hInstLib, UINT nBlockID, WORD wLangID )
{
	PSTRINGBLOCK	pStrBlock;
	HRSRC			hFindRes;
	HGLOBAL			hLoadRes;
	LPVOID			pRes;

	hFindRes = FindResourceEx(hInstLib, RT_STRING, MAKEINTRESOURCE(nBlockID), wLangID);
	if( NULL == hFindRes )
	{
		SetBlockError(STRBLOCKERR_RESNOTFOUND);
		return NULL;
	}

	hLoadRes = LoadResource(hInstLib, hFindRes);
	if( NULL == hLoadRes )
	{
		SetBlockError(STRBLOCKERR_LOADRESFAILED);
		return NULL;
	}

	pRes = LockResource(hLoadRes);
	if( NULL == pRes )
	{
		SetBlockError(STRBLOCKERR_LOADRESFAILED);
		return NULL;
	}

	// Create a new string block, fill the strings based on the resource contents.
	pStrBlock = (PSTRINGBLOCK)malloc(sizeof(STRINGBLOCK));
	if( NULL == pStrBlock )
	{
		SetBlockError(STRBLOCKERR_NOMEMORY);
		return NULL;
	}

	pStrBlock->nBlockID = nBlockID;
	pStrBlock->wLangID = wLangID;

	if( !ParseRes(pRes, pStrBlock) )
	{
		free(pStrBlock);
		return NULL;
	}

	return pStrBlock;
}


// Parse the raw string resource, pRes, and build up the string block, pStrBlock.
// The parsing illustrates the format of a string block in an executable.

BOOL ParseRes( LPVOID pRes, PSTRINGBLOCK pStrBlock )
{
	int		i, j;
	int		nLen;
	WCHAR*	pParse = (WCHAR *)pRes;

	// There are NO_OF_STRINGS_PER_BLOCK(=16) strings per block.
	for( i = 0; i < NO_OF_STRINGS_PER_BLOCK; i++ )
	{
		nLen = (int)*pParse++;			// The length of the string.
		pStrBlock->strArray[i] = (LPWSTR)malloc((nLen + 1) * sizeof(WCHAR));

		if( NULL == pStrBlock->strArray[i] )
		{
			int	k;

			for( k = 0; k < i; k++ )		// Free up the memory allocated so far.
				free(pStrBlock->strArray[k]);
			SetBlockError(STRBLOCKERR_NOMEMORY);

			return FALSE;
		}

		for( j = 0; j < nLen; j++ )		// Copy the string.
			pStrBlock->strArray[i][j] = *pParse++;
		pStrBlock->strArray[i][j] = 0;
	}

	SetBlockError(STRBLOCKERR_OK);
	return TRUE;
}


DWORD GetResSize( PSTRINGBLOCK pStrBlock )
{
	DWORD dwResSize = 0;
	int i = 0;

	for( i = 0; i < NO_OF_STRINGS_PER_BLOCK; i++ )
		dwResSize += (wcslen(pStrBlock->strArray[i]) + 1);

	return dwResSize * sizeof(WCHAR);
}


// Build a raw resource block, pRes, based on our string block, pStrBlock.
// The raw resource block may be used to update a string resource.

VOID BuildRes( PSTRINGBLOCK pStrBlock, LPVOID pRes )
{
	int		i, j;
	int		nLen;
	WCHAR*	pParse = (WCHAR *)pRes;

	// There are NO_OF_STRINGS_PER_BLOCK (= 16) strings per block.
	for( i = 0; i < NO_OF_STRINGS_PER_BLOCK; i++ )
	{
		*pParse++ = nLen = wcslen(pStrBlock->strArray[i]);
		for( j = 0; j < nLen; j++ )
			*pParse++ = pStrBlock->strArray[i][j];
	}
}


BOOL UpdateBlock( HANDLE hUpdate, PSTRINGBLOCK pStrBlock, int nBlockID, WORD wLangID )
{
	DWORD	dwResSize;
	LPVOID	pRes;
	DWORD	dwRet = 0;
	WORD	wLanguageID = (0 == wLangID) ? pStrBlock->wLangID : wLangID;

	// Get the resource length as required by a raw string resource block.
	dwResSize = GetResSize(pStrBlock);
	pRes = malloc(dwResSize);
	if( NULL == pRes )
	{
		SetBlockError(STRBLOCKERR_NOMEMORY);
		return FALSE;
	}

	BuildRes(pStrBlock, pRes);

	if( !UpdateResource(
					hUpdate,
					RT_STRING,
					MAKEINTRESOURCE(((-1 == nBlockID) ? pStrBlock->nBlockID : nBlockID)),
					wLanguageID,
					pRes,
					dwResSize
						  ) )
	{
		DWORD dwError = GetLastError();

		switch( dwError )
		{
		case ERROR_CALL_NOT_IMPLEMENTED:

			SetBlockError(STRBLOCKERR_UPDATENOTIMPLEMENTED);
			break;

		default:

			SetBlockError(STRBLOCKERR_UPDATEFAILED);
			break;
		}

		free(pRes);
		return FALSE;
	}

	free(pRes);

	SetBlockError(STRBLOCKERR_OK);

	return TRUE;
}



