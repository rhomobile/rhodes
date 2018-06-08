#include <windows.h>
#include <string>
#include <atlbase.h>
#include <algorithm>
#include <Strsafe.h>
#include "Registry.h"


//bool WriteRegHandler (HKEY proot, LPCTSTR psubkey, LPCTSTR psetting, DWORD ptype, LPCTSTR pvalue_string, bool persistent)
bool WriteRegHandler (rho::String proot, LPCTSTR psubkey, LPCTSTR psetting, rho::String ptype, LPCTSTR pvalue_string, bool persistent)
{
	HKEY hroot;
	LPWSTR pmulti_value;
	LPCWSTR cpc;
	const void *pvalue = "";
	DWORD type, dw_value;
	DWORD size = 0;
	BYTE *pbyte_value;
	bool result = false;
	BOOL got_slash;
	int n, m;

	pbyte_value = NULL;
	pmulti_value = NULL;

	// No blank values allowed
	if (wcslen (psubkey) == 0)
	{
		LOG(WARNING) + "Key must not be blank";
		goto Exit;
	}

	if (wcslen (psetting) == 0)
	{
		LOG(WARNING) + "Setting must not be blank";
		goto Exit;
	}

	hroot = ConvertToHKEY(proot);
	type = ConvertToType(ptype);

	// Get pointer to value and size in bytes
	switch (type)
	{
	case REG_DWORD:
		dw_value = _wtoi (pvalue_string);
		pvalue = &dw_value;
		size = sizeof (dw_value);
		break;

	case REG_SZ:
		pvalue = pvalue_string;
		size = (wcslen (pvalue_string) + 1) * 2;
		break;

	case REG_BINARY:
		// Input is a string of hex digits - must be an even number of characters
		if (wcslen (pvalue_string) % 2 != 0)
		{
			LOG(WARNING) + "Value must have an even number of digits: " + pvalue_string;
			goto Exit;
		}

		size = wcslen (pvalue_string) / 2;
		pbyte_value = new BYTE [size];
		for (n = 0; n < (int) size; n++)
		{
			swscanf (pvalue_string + (n * 2), L"%2x", &m);
			pbyte_value [n] = (BYTE) m;
		}
		pvalue = pbyte_value;
		break;

	case REG_MULTI_SZ:
		// Input is multiple strings separated by \n (use \\ to insert an acutal \)
		// Convert to registry format, i.e. each string terminated by null, with double null at the end
		// Allocate buffer for unescaped string - at most one character longer than original
		pmulti_value = new WCHAR [wcslen (pvalue_string) + 2];
		size = 0;
		got_slash = FALSE;
		for (cpc = pvalue_string; *cpc != L'\0'; cpc++)
		{
			if (got_slash)
			{
				switch (*cpc)
				{
				case L'\\':
					pmulti_value [size++] = L'\\';
					break;

				case L'n':
				case L'N':
					pmulti_value [size++] = L'\0';
					break;
				}

				got_slash = FALSE;
			}
			else
			{
				if (*cpc == L'\\')
					got_slash = TRUE;
				else
					pmulti_value [size++] = *cpc;
			}
		}
		pmulti_value [size++] = L'\0';
		pmulti_value [size++] = L'\0';

		// Point to unescaped value, set size in bytes
		pvalue = pmulti_value;
		size *= 2;

		break;
	}

	if (!WriteRegSetting (hroot, psubkey, psetting, type, pvalue, size))
		goto Exit;	// Log is written by WriteRegSetting

	if (persistent)
	{
		// Make sure any persistent .reg file is deleted
		if (!DeleteRegFile (hroot, psubkey, psetting))
			goto Exit;

		if (!WriteRegFile (hroot, psubkey, psetting, type, pvalue, size))
			goto Exit;	// Log is written by WriteRegFile
	}

	result = true;

Exit:
	if (pbyte_value)
		delete [] pbyte_value;

	if (pmulti_value)
		delete [] pmulti_value;

	return result;
}

bool DeleteRegHandler (const rho::String& proot, LPCTSTR psubkey, LPCTSTR psetting, bool persistent, bool create_reg_file)
{
	HKEY hroot;
	bool result;

	result = false;

	// No blank values allowed
	if (wcslen (psubkey) == 0)
	{
		LOG(WARNING) + "Key must not be blank";
		goto Exit;
	}

	if (wcslen (psetting) == 0)
	{
		LOG(WARNING) + "Setting must not be blank";
		goto Exit;
	}

	// Check for a valid root
	hroot = ConvertToHKEY(proot);

	if (!DeleteRegSetting (hroot, psubkey, psetting))
		goto Exit;

	if (persistent)
	{
		if (!DeleteRegFile (hroot, psubkey, psetting))
			goto Exit;

		if (create_reg_file)
			if (!CreateDeleteRegFile (hroot, psubkey, psetting))
				goto Exit;
	}

	result = true;

Exit:
	return result;
}

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
		LOG(WARNING) + "RegCreateKeyEx returned " + error;
		goto Exit;
	}

	if ((error = RegSetValueEx (hkey, psetting, 0, type, (const BYTE*) pvalue, size)) != ERROR_SUCCESS)
	{
		LOG(WARNING) + "RegSetValueEx returned " + error;
		goto Exit;
	}

	ok = TRUE;

Exit:
	if (hkey)
		RegCloseKey(hkey);

	return ok;
}

BOOL WriteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size)
{
	DWORD n;
	BOOL ok;
	WCHAR *pfilename, *pc;
	WCHAR *proot;
	FILE *pfile;

	pfile = NULL;
	ok = FALSE;
#ifndef CPP_ELEVEN
	if (hroot == HKEY_LOCAL_MACHINE)
		proot = (WCHAR *) L"HKEY_LOCAL_MACHINE";
	else if (hroot == HKEY_CURRENT_USER)
		proot = (WCHAR *) L"HKEY_CURRENT_USER";
	else if (hroot == HKEY_CLASSES_ROOT)
		proot = (WCHAR *) L"HKEY_CLASSES_ROOT";
	else if (hroot == HKEY_USERS)
		proot = (WCHAR *) L"HKEY_USERS";
	else
	{
		LOG(WARNING) + "Unknown hive: " + (DWORD)hroot;
		goto Exit;
	}
#else
    std::wstring temwstr;
    if (hroot == HKEY_LOCAL_MACHINE)
        temwstr = (WCHAR *) L"HKEY_LOCAL_MACHINE";
    else if (hroot == HKEY_CURRENT_USER)
        temwstr = (WCHAR *) L"HKEY_CURRENT_USER";
    else if (hroot == HKEY_CLASSES_ROOT)
        temwstr = (WCHAR *) L"HKEY_CLASSES_ROOT";
    else if (hroot == HKEY_USERS)
        temwstr = (WCHAR *) L"HKEY_USERS";
    else
    {
        LOG(WARNING) + "Unknown hive: " + (DWORD)hroot;
        goto Exit;
    }
    proot = const_cast<wchar_t *>(temwstr.c_str());
#endif
	// Form the filename, replacing '\' with '_'
	pfilename = new WCHAR [128];
	StringCchPrintf (pfilename, 128, L"\\application\\re_%s_%s_%s.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Open file
	pfile = _wfopen (pfilename, L"wt");

	// Opened OK?
	if (!pfile)
	{
		LOG(WARNING) + "Cannot open file: " + pfilename;
		goto Exit;
	}

	delete [] pfilename;

	// Write header
	fputs ("REGEDIT4\n\n", pfile);

	// Write root, key and setting
	fprintf (pfile, "[%S\\%S]\n\"%S\"=", proot, psubkey, psetting);

	// Write value according to type
	switch (type)
	{
	case REG_DWORD:
		fprintf (pfile, "dword:%08x\n", *((unsigned int*) pvalue));
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

BOOL DeleteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting)
{
	BOOL ok;
	HKEY hkey;
	LONG error;

	ok = FALSE;
	hkey = 0;

	if ((error = RegOpenKeyEx (hroot, psubkey, 0, 0, &hkey)) != ERROR_SUCCESS)
	{
		LOG(WARNING) + "RegOpenKeyEx returned " + error;
		goto Exit;
	}

	// Delete value, ignore error if setting doesn't exist
	error = RegDeleteValue (hkey, psetting);
	if (error != ERROR_SUCCESS && error != 2)
	{
		LOG(WARNING) + "RegDeleteValue returned " + error;
		goto Exit;
	}

	ok = TRUE;

Exit:
	if (hkey)
		RegCloseKey(hkey);

	return ok;
}

BOOL DeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting)
{
	BOOL ok;
	WCHAR *pfilename, *pc;
	WCHAR *proot;

	ok = FALSE;
#ifndef CPP_ELEVEN
	if (hroot == HKEY_LOCAL_MACHINE)
		proot = (WCHAR *) L"HKEY_LOCAL_MACHINE";
	else if (hroot == HKEY_CURRENT_USER)
        proot = (WCHAR *) L"HKEY_CURRENT_USER";
	else if (hroot == HKEY_CLASSES_ROOT)
		proot = (WCHAR *) L"HKEY_CLASSES_ROOT";
	else if (hroot == HKEY_USERS)
		proot = (WCHAR *) L"HKEY_USERS";
	else
	{
		LOG(WARNING) + "Unknown hive: " + (DWORD)hroot;
		goto Exit;
	}
#else
    std::wstring tempwstr;
    if (hroot == HKEY_LOCAL_MACHINE)
        tempwstr = (WCHAR *) L"HKEY_LOCAL_MACHINE";
    else if (hroot == HKEY_CURRENT_USER)
        tempwstr = (WCHAR *) L"HKEY_CURRENT_USER";
    else if (hroot == HKEY_CLASSES_ROOT)
        tempwstr = (WCHAR *) L"HKEY_CLASSES_ROOT";
    else if (hroot == HKEY_USERS)
        tempwstr = (WCHAR *) L"HKEY_USERS";
    else
    {
        LOG(WARNING) + "Unknown hive: " + (DWORD)hroot;
        goto Exit;
    }
    proot = const_cast<wchar_t *>(tempwstr.c_str());
#endif
	pfilename = new WCHAR [128];

	// Form the 'create' filename, replacing '\' with '_'
	StringCchPrintf (pfilename, 128, L"\\application\\re_%s_%s_%s.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Delete the file
	DeleteFile (pfilename);

	// Form the 'delete' filename, replacing '\' with '_'
	StringCchPrintf (pfilename, 128, L"\\application\\re_%s_%s_%s_delete.reg", proot, psubkey, psetting);
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

BOOL CreateDeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting)
{
	BOOL ok;
	WCHAR *pfilename, *pc;
	WCHAR *proot;
	FILE *pfile;

	pfile = NULL;
	ok = FALSE;
#ifndef CPP_ELEVEN
	if (hroot == HKEY_LOCAL_MACHINE)
		proot = (WCHAR *) L"HKEY_LOCAL_MACHINE";
	else if (hroot == HKEY_CURRENT_USER)
		proot = (WCHAR *) L"HKEY_CURRENT_USER";
	else if (hroot == HKEY_CLASSES_ROOT)
		proot = (WCHAR *) L"HKEY_CLASSES_ROOT";
	else if (hroot == HKEY_USERS)
		proot = (WCHAR *) L"HKEY_USERS";
	else
	{
		LOG(WARNING) + "Unknown hive: " + (DWORD)hroot;
		goto Exit;
	}
#else
    std::wstring tempstr;
    if (hroot == HKEY_LOCAL_MACHINE)
        tempstr = (WCHAR *) L"HKEY_LOCAL_MACHINE";
    else if (hroot == HKEY_CURRENT_USER)
        tempstr = (WCHAR *) L"HKEY_CURRENT_USER";
    else if (hroot == HKEY_CLASSES_ROOT)
        tempstr = (WCHAR *) L"HKEY_CLASSES_ROOT";
    else if (hroot == HKEY_USERS)
        tempstr = (WCHAR *) L"HKEY_USERS";
    else
    {
        LOG(WARNING) + "Unknown hive: " + (DWORD)hroot;
        goto Exit;
    }
    proot = const_cast<wchar_t *>(tempstr.c_str());
#endif
	// Form the filename, replacing '\' with '_'
	pfilename = new WCHAR [128];
	StringCchPrintf (pfilename, 128, L"\\application\\re_%s_%s_%s_delete.reg", proot, psubkey, psetting);
	for (pc = pfilename + 13; *pc; pc++)
		if (*pc == L'\\')
			*pc = L'_';

	// Open file
	pfile = _wfopen (pfilename, L"wt");

	// Opened OK?
	if (!pfile)
	{
		LOG(WARNING) + "Cannot open file: " + pfilename;
		goto Exit;
	}

	delete [] pfilename;

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

void GetRegistrySetting(const rho::String& retKey, const rho::String& subKey, const rho::String& setting, rho::apiGenerator::CMethodResult& oResult)
{
	HKEY hkey = 0;
	DWORD type, size;
	BYTE *pbuffer = 0;
	LONG regResult;

	// Open registry key
	regResult = RegOpenKeyEx (ConvertToHKEY(retKey), rho::common::convertToStringW(subKey).c_str(), 0, 0, &hkey);
	
	// Return empty string if key can't be opened (e.g. it doesn't exist)
	if (regResult != ERROR_SUCCESS)
	{
		LOG(WARNING) + "Unable to open Registry Key for setting " + setting;
		oResult.set("");
	}
	// Get size of registry data
	else if (RegQueryValueEx (hkey, rho::common::convertToStringW(setting).c_str(), NULL, &type, NULL, &size) != ERROR_SUCCESS)
	{
		//  RegQueryValueEx returned Error
		LOG(WARNING) + "Unable to open registry setting " + setting + " for subkey " + subKey;
		oResult.set("");
	}
	else
	{
		// Allocate memory for value
		pbuffer = new BYTE [size + sizeof(TCHAR)];
		if (pbuffer == NULL)
		{
			oResult.set("");
			return;
		}
		//  Add a NULL on the end to ensure any string we might receive back is NULL terminated
		memset(pbuffer, 0, size + sizeof(TCHAR));

		// Get registry value itself
		if (RegQueryValueEx (hkey, rho::common::convertToStringW(setting).c_str(), NULL, &type, pbuffer, &size) == ERROR_SUCCESS)
		{
			if (pbuffer == NULL)
			{
				//  For some reason pbuffer is still NULL (KW)
				LOG(WARNING) + "Unable to retrieve Registry setting " + setting;
				oResult.set("");
			}
			else
			{
				switch (type)
				{
					case REG_BINARY:
					{
						char tcHex[3];
						rho::String szBinaryReturn = "";
						for(UINT ui = 0; ui < size; ui++)
						{
							sprintf(tcHex, "%02X", pbuffer[ui]);
							szBinaryReturn.append(tcHex);
						}
						oResult.set(szBinaryReturn);
						break;
					}
					case REG_DWORD:
					{
						char szReturnBuffer[56];
						sprintf (szReturnBuffer, "%lu", *((DWORD*) pbuffer));
						oResult.set(szReturnBuffer);
						break;
					}
					case REG_MULTI_SZ:
						//  Loop through the pbuffer for size characters replacing
						//  \0 with \n (except last two characters as these are the \0\0's
						//  to terminate the string
						for (UINT i = 0; i < size - 4; i+=2)
						{
							if (pbuffer[i] == L'\0')
                            {
								pbuffer[i] = L'\n';
                            }
						}
						//  Don't break as we have the same string conversion logic 
						//  from this point on as normal strings
						//  break;
					default:
						rho::StringW szStringReturn = L"";
						szStringReturn.append((WCHAR*)pbuffer);
						oResult.set(szStringReturn);
				}
			}
		}
	}
	if (hkey)
		RegCloseKey (hkey);

	if (pbuffer)
		delete [] pbuffer;

}
HKEY ConvertToHKEY(rho::String neonKey)
{
	std::transform(neonKey.begin(), neonKey.end(), neonKey.begin(), ::tolower);
	if (neonKey.compare("hkcr") == 0)
		return HKEY_CLASSES_ROOT;
	else if (neonKey.compare("hkcu") == 0)
		return HKEY_CURRENT_USER;
	else if (neonKey.compare("hklm") == 0)
		return HKEY_LOCAL_MACHINE;
	else if (neonKey.compare("hku") == 0)
		return HKEY_USERS;
	else
	{
		LOG(WARNING) + "Unrecognised hive value, received " + neonKey;
		return HKEY_USERS;
	}
}

DWORD ConvertToType(rho::String neonType)
{
	std::transform(neonType.begin(), neonType.end(), neonType.begin(), ::tolower);
	if (neonType.compare("string") == 0)
		return 1;
	else if (neonType.compare("binary") == 0)
		return 3;
	else if (neonType.compare("dword") == 0)
		return 4;
	else if (neonType.compare("multisz") == 0)
		return 7;
	else
	{
		LOG(WARNING) + "Unrecognised registry type, received " + neonType;
		return 1;
	}
}
