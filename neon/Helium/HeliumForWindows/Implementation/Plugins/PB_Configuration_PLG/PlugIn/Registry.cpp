/******************************************************************************/
#include <windows.h>
#include "Registry.h"
#include "..\..\..\Common\Public\PBUtil.h"

/******************************************************************************/
BOOL WriteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size)
{
	BOOL ok;
	HKEY hkey;
	DWORD disposition;
	LONG error;

	ok = FALSE;
	hkey = 0;

	if ((error = RegCreateKeyEx (hroot, psubkey, 0, NULL, 0, 0, NULL, &hkey, &disposition)) != ERROR_SUCCESS)
	{
		WRITELOG (L"RegCreateKeyEx returned %ld", error);
		goto Exit;
	}

	if ((error = RegSetValueEx (hkey, psetting, 0, type, (const BYTE*) pvalue, size)) != ERROR_SUCCESS)
	{
		WRITELOG (L"RegSetValueEx returned %ld", error);
		goto Exit;
	}

	ok = TRUE;

Exit:
	if (hkey)
		RegCloseKey(hkey);

	return ok;
}

/******************************************************************************/
BOOL WriteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size)
{
	DWORD n;
	BOOL ok;
	WCHAR *pfilename, *pc;
	WCHAR *proot;
	FILE *pfile;

	pfile = NULL;
	ok = FALSE;

	if (hroot == HKEY_LOCAL_MACHINE)
		proot = L"HKEY_LOCAL_MACHINE";
	else if (hroot == HKEY_CURRENT_USER)
		proot = L"HKEY_CURRENT_USER";
	else if (hroot == HKEY_CLASSES_ROOT)
		proot = L"HKEY_CLASSES_ROOT";
	else if (hroot == HKEY_USERS)
		proot = L"HKEY_USERS";
	else
	{
		WRITELOG (L"Unknown hive: 0x%08X", hroot);
		goto Exit;
	}

	// Form the filename, replacing '\' with '_'
	pfilename = new WCHAR [128];
	StringCchPrintf (pfilename, 128, L"\\application\\re_%s_%s_%s.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Open file
	pfile = _wfopen (pfilename, L"wt");
	delete [] pfilename;

	// Opened OK?
	if (!pfile)
	{
		WRITELOG (L"Cannot open file: '%s'", pfilename);
		goto Exit;
	}

	// Write header
	fputs ("REGEDIT4\n\n", pfile);

	// Write root, key and setting
	fprintf (pfile, "[%S\\%S]\n\"%S\"=", proot, psubkey, psetting);

	// Write value according to type
	switch (type)
	{
	case REG_DWORD:
		fprintf (pfile, "dword:%08x\n", *((DWORD*) pvalue));
		break;

	case REG_SZ:
		fprintf (pfile, "\"%S\"\n", (LPWSTR) pvalue);
		break;

	case REG_MULTI_SZ:
		fprintf (pfile, "hex(7):");

		// Size is in bytes, we only need half that number of wide chars
		for (n = 0; n < size / 2; n++)
		{
			if (n > 0)
			{
				fprintf (pfile, ",");

				// Break line every 15 bytes
				if (n % 15 == 0)
					fprintf (pfile,"\\\n");
			}

			// Convert each wide char to a byte
			fprintf (pfile, "%02x", (BYTE) (((LPCWSTR) pvalue) [n]));
		}

		fprintf (pfile, "\n");
		break;

	case REG_BINARY:
		fprintf (pfile, "hex:");

		for (n = 0; n < size; n++)
		{
			if (n > 0)
			{
				fprintf (pfile, ",");

				// Break line every 15 bytes
				if (n % 15 == 0)
					fprintf (pfile,"\\\n");
			}

			fprintf (pfile, "%02x", ((BYTE*) pvalue) [n]);
		}

		fprintf (pfile, "\n");
		break;
	}

	// Add a blank line
	fprintf (pfile, "\n");

	ok = TRUE;

Exit:
	if (pfile != NULL)
		fclose (pfile);

	return ok;
}

/******************************************************************************/
BOOL DeleteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting)
{
	BOOL ok;
	HKEY hkey;
	LONG error;

	ok = FALSE;
	hkey = 0;

	if ((error = RegOpenKeyEx (hroot, psubkey, 0, 0, &hkey)) != ERROR_SUCCESS)
	{
		WRITELOG (L"RegOpenKeyEx returned %ld", error);
		goto Exit;
	}

	// Delete value, ignore error if setting doesn't exist
	error = RegDeleteValue (hkey, psetting);
	if (error != ERROR_SUCCESS && error != 2)
	{
		WRITELOG (L"RegDeleteValue returned %ld", error);
		goto Exit;
	}

	ok = TRUE;

Exit:
	if (hkey)
		RegCloseKey(hkey);

	return ok;
}

/******************************************************************************/
BOOL DeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting)
{
	BOOL ok;
	WCHAR *pfilename, *pc;
	WCHAR *proot;

	ok = FALSE;

	if (hroot == HKEY_LOCAL_MACHINE)
		proot = L"HKEY_LOCAL_MACHINE";
	else if (hroot == HKEY_CURRENT_USER)
		proot = L"HKEY_CURRENT_USER";
	else if (hroot == HKEY_CLASSES_ROOT)
		proot = L"HKEY_CLASSES_ROOT";
	else if (hroot == HKEY_USERS)
		proot = L"HKEY_USERS";
	else
	{
		WRITELOG (L"Unknown hive: 0x%08X", hroot);
		goto Exit;
	}

	pfilename = new WCHAR [128];

	// Form the 'create' filename, replacing '\' with '_'
	StringCchPrintf (pfilename, 128, L"\\application\\pb_%s_%s_%s.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Delete the file
	DeleteFile (pfilename);

	// Form the 'delete' filename, replacing '\' with '_'
	StringCchPrintf (pfilename, 128, L"\\application\\pb_%s_%s_%s_delete.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Delete the file
	DeleteFile (pfilename);
	delete [] pfilename;

	ok = TRUE;

Exit:
	return ok;
}

/******************************************************************************/
BOOL CreateDeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting)
{
	BOOL ok;
	WCHAR *pfilename, *pc;
	WCHAR *proot;
	FILE *pfile;

	pfile = NULL;
	ok = FALSE;

	if (hroot == HKEY_LOCAL_MACHINE)
		proot = L"HKEY_LOCAL_MACHINE";
	else if (hroot == HKEY_CURRENT_USER)
		proot = L"HKEY_CURRENT_USER";
	else if (hroot == HKEY_CLASSES_ROOT)
		proot = L"HKEY_CLASSES_ROOT";
	else if (hroot == HKEY_USERS)
		proot = L"HKEY_USERS";
	else
	{
		WRITELOG (L"Unknown hive: 0x%08X", hroot);
		goto Exit;
	}

	// Form the filename, replacing '\' with '_'
	pfilename = new WCHAR [128];
	StringCchPrintf (pfilename, 128, L"\\application\\re_%s_%s_%s_delete.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Open file
	pfile = _wfopen (pfilename, L"wt");
	delete [] pfilename;

	// Opened OK?
	if (!pfile)
	{
		WRITELOG (L"Cannot open file: '%s'", pfilename);
		goto Exit;
	}

	// Write header
	fputs ("REGEDIT4\n\n", pfile);

	// Write root, key and setting
	fprintf (pfile, "[%S\\%S]\n\"%S\"=-", proot, psubkey, psetting);

	// Add a blank line
	fprintf (pfile, "\n");

	ok = TRUE;

Exit:
	if (pfile != NULL)
		fclose (pfile);

	return ok;
}

