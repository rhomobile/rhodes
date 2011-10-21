/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.

   Copyright 1998 Microsoft Corporation.  All Rights Reserved.
**************************************************************************/

/**************************************************************************

   File:          strblock.h

   Description:   Programmer's interface to manipulate a block of string
				  resources. It defines an API with functions to Create,
				  Access, Modify & Destroy a string block.

**************************************************************************/

#ifndef	_STRBLOCK_H
#define _STRBLOCK_H

#include <windows.h>

#ifdef __cplusplus
	extern "C" {
#endif

// The number of strings in a block. The string resource is internally
// stored as blocks of NO_OF_STRINGS_PER_BLOCK ( = 16) strings. A block
// is the smallest granularity for manipulating a string resource.
#define		NO_OF_STRINGS_PER_BLOCK		16

// Error codes for operations on a string block.
typedef enum {
	STRBLOCKERR_OK,				// The last operation was successful.
	STRBLOCKERR_NOMEMORY,		// The last operation on the block failed due to inadequate memory.
	STRBLOCKERR_INVALIDINDEX,	// The index passed was incorrect.
	STRBLOCKERR_UPDATEFAILED,	// The specified app could not be updated.
	STRBLOCKERR_APPLOADFAILED,	// The specified app could not be loaded.
	STRBLOCKERR_RESNOTFOUND,	// The specified resource could not be located.
	STRBLOCKERR_LOADRESFAILED,	// The specified resource could not be loaded.
	STRBLOCKERR_INVALIDBLOCK,	// The specified block handle is invalid.
	STRBLOCKERR_STRINVALID,		// The string pointer passed was invalid.
	STRBLOCKERR_UPDATENOTIMPLEMENTED,	// UpdateResource not implemented.
	STRBLOCKERR_UNKNOWN			// An unspecified error.
} STRBLOCKERR;


// Handle to a block of string. We hide the internal format we use for manipulating
// a string resource by providing access to it through a handle.

DECLARE_HANDLE(HSTRBLOCK);


// Methods to access a string block.

// Function:			HSTRBLOCK WINAPI GetStringBlockA( LPCSTR strAppName, UINT nBlockID,
//														  WORD wLangID );
// Purpose:				Get the block of string with the specified ID & language from the
//						specified application (ANSI version).
// LPCSTR	strAppName:	The name of the application.
// UINT		nBlockID:	The ID of the block.
// WORD		wLangID:	The language identifier. You can create a language
//						identifier using the macro, MAKELANGID.
// Returns:				Handle to a string block if successful, NULL otherwise.
// Comments:			This function creates a string block. Call DeleteStringBlock()
//						when you no longer need the block.
HSTRBLOCK WINAPI GetStringBlockA( LPCSTR strAppName, UINT nBlockID, WORD wLangID );


// Function:			HSTRBLOCK WINAPI GetStringBlockW( LPCWSTR strAppName, UINT nBlockID,
//														  WORD wLangID );
// Purpose:				Get the block of string with the specified ID & language from the
//						specified application (UNICODE version).
// LPCWSTR	strAppName:	The name of the application.
// UINT		nBlockID:	The ID of the block.
// WORD		wLangID:	The language identifier. You can create a language
//						identifier using the macro, MAKELANGID.
// Returns:				Handle to a string block if successful, NULL otherwise.
// Comments:			This function creates a string block. Call DeleteStringBlock()
//						when you no longer need the block.
HSTRBLOCK WINAPI GetStringBlockW( LPCWSTR strAppName, UINT nBlockID, WORD wLangID );


// Function:			BOOL WINAPI DeleteStringBlock( HSTRBLOCK hStrBlock );
// Purpose:				Delete a block of string.
// HSTRBLOCK hStrBlock: The handle to the block.
// Returns:				TRUE if successful, FALSE on failure.
// Comments:			Call this function when you no longer need a block. Calling this function
//						frees the memory occupied by the block. Failure to delete a block
//						results in memory leaks.
BOOL WINAPI DeleteStringBlock( HSTRBLOCK hStrBlock );


// Function:			int WINAPI GetStringLength( HSTRBLOCK hStrBlock, UINT nIndex );
// Purpose:				Get the length of a string in the block.
// HSTRBLOCK hStrBlock:	The handle to the string block.
// UINT nIndex:			The zero-based index of the string in the block. The index can be
//						any value from 0 thru' (NO_OF_STRINGS_PER_BLOCK - 1).
// Returns:				If successful, the string length in number of characters, excluding the terminating
//						NULL character; -1 if the function fails.
// Comments:			Call this function to get the string length before calling GetString.
int WINAPI GetStringLength( HSTRBLOCK hStrBlock, UINT nIndex );


// Function:			BOOL WINAPI GetStringA( HSTRBLOCK hStrBlock, UINT nIndex, LPSTR pszStr );
// Purpose:				Get the string at the specified index in the block (ANSI version).
// HSTRBLOCK hStrBlock: The handle to the string block.
// UINT nIndex:			The zero-based index of the string in the block. The index can be a value
//						from 0 through (NO_OF_STRINGS_PER_BLOCK - 1).
// LPSTR pszStr:		The pointer to the memory to which the string requested will be copied.
//						This memory should be large enough to contain the string.
// Returns:				TRUE on success, FALSE otherwise.
// Comments:			Before calling this function, call GetStringLength() to get the string length.
BOOL WINAPI GetStringA( HSTRBLOCK hStrBlock, UINT nIndex, LPSTR pszStr );


// Function:			BOOL WINAPI GetStringW( HSTRBLOCK hStrBlock, UINT nIndex, LPWSTR pszStr );
// Purpose:				Get the string at the specified index in the block (UNICODE version).
// HSTRBLOCK hStrBlock: The handle to the string block.
// UINT nIndex:			The zero-based index of the string in the block. The index can be a value
//						from 0 through (NO_OF_STRINGS_PER_BLOCK - 1).
// LPSTR pszStr:		The pointer to the memory to which the string requested will be copied.
//						This memory should be large enough to contain the string.
// Returns:				TRUE on success, FALSE otherwise.
// Comments:			Before calling this function, call GetStringLength() to get the string length.
BOOL WINAPI GetStringW( HSTRBLOCK hStrBlock, UINT nIndex, LPWSTR pszStr );


// Function:			BOOL WINAPI SetStringA( HSTRBLOCK hStrBlock, UINT nIndex, LPCSTR pszText );
// Purpose:				Change the string at the specified index in the block (ANSI version).
// HSTRBLOCK hStrBlock: The handle to the string block.
// UINT nIndex:			The zero-based index of the string in the block. The index can be a value
//						from 0 through (NO_OF_STRINGS_PER_BLOCK - 1).
// LPCSTR pszText:		The pointer to the new string.
// Returns:				TRUE on success, FALSE otherwise.
BOOL WINAPI SetStringA( HSTRBLOCK hStrBlock, UINT nIndex, LPCSTR pszText );


// Function:			BOOL WINAPI SetStringW( HSTRBLOCK hStrBlock, UINT nIndex, LPCWSTR pszStr );
// Purpose:				Change the string at the specified index in the block (UNICODE version).
// HSTRBLOCK hStrBlock: The handle to the string block.
// UINT nIndex:			The zero-based index of the string in the block. The index can be a value
//						from 0 through (NO_OF_STRINGS_PER_BLOCK - 1).
// LPCWSTR pszText:		The pointer to the new string.
// Returns:				TRUE on success, FALSE otherwise.
BOOL WINAPI SetStringW( HSTRBLOCK hStrBlock, UINT nIndex, LPCWSTR pszText );


// Function:			int WINAPI GetFirstStringID( HSTRBLOCK hStrBlock );
// Purpose:				Get the ID of the first string in the block.
// HSTRBLOCK hStrBlock: The handle to the string block.
// Returns:				The ID of the first string in the block. -1 if the function fails.
int WINAPI GetFirstStringID( HSTRBLOCK hStrBlock );


// Function:			int WINAPI GetBlockID( HSTRBLOCK hStrBlock );
// Purpose:				Get the ID of the string block. This is the ID of the string block that was
//						specified when the block was created.
// HSTRBLOCK hStrBlock:	The handle to the string block.
// Returns:				The block ID, if sucessful, or -1 on failure.
int WINAPI GetBlockID( HSTRBLOCK hStrBlock );


// Function:			WORD WINAPI GetBlockLanguage( HSTRBLOCK hStrBlock );
// Purpose:				Get the identifier of the language for the string block. This is the language ID
//						specified when the block was created.
// HSTRBLOCK hStrBlock:	The handle to the string block.
// Returns:				The language ID of the block, if successful; 0 on failure.
// Comments:			For info on language IDs, refer help of MAKELANGID.
WORD WINAPI GetBlockLanguage( HSTRBLOCK hStrBlock );


// Function:			BOOL WINAPI UpdateStringBlockA( LPCSTR strAppName, HSTRBLOCK hStrBlock,
//														int nBlockID, WORD wLangID );
// Purpose:				Update the block of string resource in the specified application
//						(ANSI version).
// LPCTSTR strAppName:	The name of the application whose string resource has to be updated.
// HSTRBLOCK hStrBlock: The handle to the block that contains the new strings.
// int nBlockID:		The ID of the block to update in the app. If it is -1, use the same block
//						ID as that of hStrBlock.
// WORD wLangID:		The identifier of the language whaose string block has to be updated. If this
//						value is 0, use the same language as that of hStrBlock.
// Returns:				TRUE on success, FALSE on failure.
BOOL WINAPI UpdateStringBlockA( LPCSTR strAppName, HSTRBLOCK hStrBlock, int nBlockID, WORD wLangID );


// Function:			BOOL WINAPI UpdateStringBlockW( LPCWTR strAppName, HSTRBLOCK hStrBlock,
//														int nBlockID, WORD wLangID );
// Purpose:				Update the block of string resource in the specified application
//						(UNICODE version).
// LPCTSTR strAppName:	The name of the application whose string resource has to be updated.
// HSTRBLOCK hStrBlock: The handle to the block that contains the new strings.
// int nBlockID:		The ID of the block to update in the app. If it is -1, use the same block
//						ID as that of hStrBlock.
// WORD wLangID:		The identifier of the language whaose string block has to be updated. If this
//						value is 0, use the same language as that of hStrBlock.
// Returns:				TRUE on success, FALSE on failure.
BOOL WINAPI UpdateStringBlockW( LPCWSTR strAppName, HSTRBLOCK hStrBlock, int nBlockID, WORD wLangID );


// Function:			STRBLOCKERR WINAPI GetStringBlockError();
// Purpose:				Get the error status of the last string block action.
// Returns:				The error status of the last operation on the specified block.
// Comments:			The error code is maintained on a per-thread basis. Multiple threads do not
//						overwrite each other's last block error code.
STRBLOCKERR WINAPI GetStringBlockError();


#ifdef UNICODE
#define		GetStringBlock		GetStringBlockW
#define		GetString			GetStringW
#define		SetString			SetStringW
#define		UpdateStringBlock	UpdateStringBlockW
#else
#define		GetStringBlock		GetStringBlockA
#define		GetString			GetStringA
#define		SetString			SetStringA
#define		UpdateStringBlock	UpdateStringBlockA
#endif // UNICODE

#ifdef __cplusplus
	}
#endif

#endif // _STRBLOCK_H