/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"

#pragma pack(2)

// For each 1..idCount:
typedef struct
{
	BYTE bWidth; // Width, in pixels, of the image
	BYTE bHeight; // Height, in pixels, of the image
	BYTE bColorCount; // Number of colors in image (0 if >=8bpp)
	BYTE bReserved; // Reserved
	WORD wPlanes; // Color Planes
	WORD wBitCount; // Bits per pixel
	DWORD dwBytesInRes; // how many bytes in this resource?
	DWORD dwImageOffeset; // the ID
} ICONDIRENTRY;

typedef struct
{
	WORD idReserved; // Reserved (must be 0)
	WORD idType; // Resource type (1 for icons)
	WORD idCount; // How many images?
	ICONDIRENTRY idEntries[1]; // The entries for each image
} ICONDIR;


typedef struct
{
	BYTE bWidth; // Width, in pixels, of the image
	BYTE bHeight; // Height, in pixels, of the image
	BYTE bColorCount; // Number of colors in image (0 if >=8bpp)
	BYTE bReserved; // Reserved
	WORD wPlanes; // Color Planes
	WORD wBitCount; // Bits per pixel
	DWORD dwBytesInRes; // how many bytes in this resource?
	WORD nID; // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct
{
	WORD idReserved; // Reserved (must be 0)
	WORD idType; // Resource type (1 for icons)
	WORD idCount; // How many images?
	GRPICONDIRENTRY idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;

BOOL ReplaceResource(HANDLE hUpdate, LPCTSTR lpType, LPCTSTR lpName, LPVOID lpData, DWORD cbData);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hUpdateRes = NULL;  // update resource handle
	LPVOID lpResLock = NULL;   // pointer to resource data
	int result = 0;
	LPTSTR type = NULL;
	LPTSTR name = NULL;
	BYTE* temp = NULL;
	GRPICONDIR* newDir = NULL;

	if ( argc < 4 )
	{
		printf("Invalid number of arguments!\n");
		printf("Usage:\n\treplaceicon <path to executable> <path to the new ico file> <IDI_ICON to replace>\n\n"
			   "For example:\n\treplaceicon C:\rhodes.exe C:\newicon.ico 114");
		result = -1;
		goto cleanup;
	}

	//read  IDI_ICON
	int IDI_ICON = atoi(argv[3]);

	HANDLE inFile = CreateFile(argv[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (inFile == INVALID_HANDLE_VALUE)
	{
		printf("Error cannot open ico file: %s\n", argv[2]);
		result = -1;
		goto cleanup;
	}

	// Open the file to which we want to change icon
	hUpdateRes = BeginUpdateResource(argv[1], FALSE);
	if (hUpdateRes == NULL)
	{
		printf("Could not open file for writing: %s\n", argv[1]);
		result = -1;
		goto cleanup;
	}

	DWORD read;
	DWORD size = GetFileSize(inFile, NULL);
	temp = (BYTE*)GlobalAlloc(GMEM_FIXED, size);
	ReadFile(inFile, temp, size, &read, NULL);

	// Change the icon group
	ICONDIR* ig = (ICONDIR*)(temp);

	DWORD newSize = sizeof(GRPICONDIR) + (sizeof(GRPICONDIRENTRY)*(ig->idCount - 1));
	newDir = (GRPICONDIR*)GlobalAlloc(GMEM_FIXED, newSize);
	newDir->idReserved = ig->idReserved;
	newDir->idType = ig->idType;
	newDir->idCount = ig->idCount;

	DWORD rv;
	for (int i = 0; i < ig->idCount; i++)
	{
		BYTE* temp1 = temp + ig->idEntries[i].dwImageOffeset;
		DWORD size1 = ig->idEntries[i].dwBytesInRes;

		newDir->idEntries[i].bWidth = ig->idEntries[i].bWidth;
		newDir->idEntries[i].bHeight = ig->idEntries[i].bHeight;
		newDir->idEntries[i].bColorCount = ig->idEntries[i].bColorCount;
		newDir->idEntries[i].bReserved = ig->idEntries[i].bReserved;
		newDir->idEntries[i].wPlanes = ig->idEntries[i].wPlanes;
		newDir->idEntries[i].wBitCount = ig->idEntries[i].wBitCount;
		newDir->idEntries[i].dwBytesInRes = ig->idEntries[i].dwBytesInRes;
		newDir->idEntries[i].nID = i + 1;

		type = RT_ICON;
		name = MAKEINTRESOURCE(i + 1);
		if (TRUE == (rv = ReplaceResource(hUpdateRes, type, name, (LPVOID)temp1, size1 )))
		{
			printf("Icon %d replaced OK!\n", i + 1);
		}
		else
		{
			printf("Replacing the icon %d group FAILED!\n", i + 1);
		}
	}

	type = RT_GROUP_ICON;
	name = MAKEINTRESOURCE(IDI_ICON);

	if (TRUE == (rv = ReplaceResource(hUpdateRes, type, name, (LPVOID)newDir, newSize)))
	{
		printf("Icon group replaced OK!\n");
	} else
	{
		printf("Replacing the icon group FAILED!\n");
	}

	// Write changes to EXE and then close it.
	if (!EndUpdateResource(hUpdateRes, FALSE))
		printf("Could not write changes to file.");

cleanup:
	if ( inFile != NULL && inFile != INVALID_HANDLE_VALUE)
		CloseHandle(inFile);

	if ( newDir != NULL )
		GlobalFree(newDir);

	if ( temp != NULL )
		GlobalFree(temp);

	return result;
}

BOOL ReplaceResource(HANDLE hUpdate, LPCTSTR lpType, LPCTSTR lpName, LPVOID lpData, DWORD cbData)
{

	return UpdateResource(hUpdate,							// update resource handle
			   lpType,										// change dialog box resource
			   lpName,										// dialog box id
			   MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),	// neutral language
			   lpData,										// ptr to resource data
			   cbData);										// size of resource data

};