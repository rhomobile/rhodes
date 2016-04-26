/******************************************************************************/
#include <windows.h>
#include <stdarg.h>
#include "..\..\..\Common\Public\PBplugin.h"
#include "..\..\..\Common\Public\PBUtil.h"
#include "Registry.h"

/******************************************************************************/
#define EXPORT_TYPE extern "C" __declspec(dllexport)

#define ADD_EXPORTS(x) \
	EXPORT_TYPE BOOL x##_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure) \
		{if (!pLogger) pLogger = pPBCoreStructure->pLoggingFunc; return TRUE;} \
	EXPORT_TYPE BOOL x##_Dispose (PPBSTRUCT pPBStructure) {return TRUE;}

/******************************************************************************/
pLogFunc pLogger;
WCHAR wszHive [64], wszKey [128], wszSetting [64], wszType [32];
BOOL bPersistent;
WCHAR wszConfigSetting [128];

/******************************************************************************/
void WriteLog (LPCWSTR pfunction, int line, LPCWSTR pformat, ...)
{
	if (!pLogger)
		return;

	LPWSTR pmessage = new WCHAR [256];
	va_list args;
	va_start (args, pformat);
	StringCchVPrintf (pmessage, 256, pformat, args);
	(*pLogger) (PB_LOG_WARNING, pmessage, pfunction, line, L"Config");
	delete [] pmessage;
}

/******************************************************************************/
void ResetParameters (void)
{
	wcscpy (wszHive, L"");
	wcscpy (wszKey, L"");
	wcscpy (wszSetting, L"");
	wcscpy (wszType, L"");
	bPersistent = FALSE;
}

/******************************************************************************/
void ResetConfig (void)
{
	wcscpy (wszConfigSetting, L"");
}

/******************************************************************************/
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	pLogger = NULL;
	ResetParameters ();
	ResetConfig ();

    return TRUE;
}

/******************************************************************************/
ADD_EXPORTS(WriteRegSetting)
ADD_EXPORTS(WritePersistentRegSetting)
ADD_EXPORTS(DeleteRegSetting)
ADD_EXPORTS(DeletePersistentRegSetting)
ADD_EXPORTS(Registry)
ADD_EXPORTS(Config)

/******************************************************************************/
BOOL ParseParameters (LPWSTR pparam, LPWSTR *pproot, LPWSTR *ppsubkey, LPWSTR *ppsetting, LPWSTR *pptype)
{
/*
Given a tag like: <META HTTP-Equiv="WriteRegSetting" Content="hklm\mykey\mysetting=dword:1000" />
we'll receive:

parameter = hklm\mykey\mysetting=dword
value = 1000
*/

	LPWSTR pc;
	BOOL ok;

	ok = FALSE;

	// Extract root name up to first '\'
	*pproot = pparam;
	pc = wcschr (*pproot, L'\\');
	if (!pc)
	{
		WRITELOG (L"Cannot find hive terminator: '%s'", *pproot);
		goto Exit;
	}
	*pc = L'\0';

	// Extract subkey up to final '\'
	*ppsubkey = pc + 1;
	pc = wcsrchr (*ppsubkey, L'\\');
	if (!pc)
	{
		WRITELOG (L"Cannot find key terminator: '%s'", *ppsubkey);
		goto Exit;
	}
	*pc = L'\0';

	*ppsetting = pc + 1;

	// Extract setting up to '=', or end of string if 'type' not required
	if (pptype)
	{
		pc = wcschr (*ppsetting, L'=');
		if (!pc)
		{
			WRITELOG (L"Cannot find setting terminator: '%s'", *ppsetting);
			goto Exit;
		}
		*pc = L'\0';

		*pptype = pc + 1;
	}

	ok = TRUE;

Exit:
	return ok;
}

/******************************************************************************/
BOOL WriteRegHandler (LPWSTR proot, LPWSTR psubkey, LPWSTR psetting, LPWSTR ptype, LPCWSTR pvalue_string, BOOL persistent)
{
	HKEY hroot;
	LPWSTR pmulti_value;
	LPCWSTR cpc;
	const void *pvalue;
	DWORD type, size, dw_value;
	BYTE *pbyte_value;
	BOOL result, got_slash;
	int n, m;

	result = FALSE;
	pbyte_value = NULL;
	pmulti_value = NULL;

	// No blank values allowed
	if (wcslen (proot) == 0)
	{
		WRITELOG (L"Hive must not be blank");
		goto Exit;
	}

	if (wcslen (psubkey) == 0)
	{
		WRITELOG (L"Key must not be blank");
		goto Exit;
	}

	if (wcslen (psetting) == 0)
	{
		WRITELOG (L"Setting must not be blank");
		goto Exit;
	}

	if (wcslen (ptype) == 0)
	{
		WRITELOG (L"Type must not be blank");
		goto Exit;
	}

	// Check for a valid root
	if (!wcsicmp (proot, L"hklm"))
		hroot = HKEY_LOCAL_MACHINE;
	else if (!wcsicmp (proot, L"hkcu"))
		hroot = HKEY_CURRENT_USER;
	else if (!wcsicmp (proot, L"hkcr"))
		hroot = HKEY_CLASSES_ROOT;
	else if (!wcsicmp (proot, L"hku"))
		hroot = HKEY_USERS;
	else
	{
		WRITELOG (L"Unknown hive: '%s'", proot);
		goto Exit;
	}

	// Check for valid type
	if (!wcsicmp (ptype, L"dword"))
		type = REG_DWORD;
	else if (!wcsicmp (ptype, L"binary"))
		type = REG_BINARY;
	else if (!wcsicmp (ptype, L"string"))
		type = REG_SZ;
	else if (!wcsicmp (ptype, L"multisz"))
		type = REG_MULTI_SZ;
	else
	{
		WRITELOG (L"Unknown type: '%s'", ptype);
		goto Exit;
	}

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
			WRITELOG (L"Value must have an even number of digits: '%s'", pvalue_string);
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

	result = TRUE;

Exit:
	if (pbyte_value)
		delete [] pbyte_value;

	if (pmulti_value)
		delete [] pmulti_value;

	return result;
}

/******************************************************************************/
EXPORT_TYPE BOOL WritePersistentRegSetting_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	LPWSTR pparam, proot, psubkey, psetting, ptype;
	BOOL ok;

	ok = FALSE;
	pparam = _wcsdup (pbMetaStructure->lpParameter);

	if (!ParseParameters (pparam, &proot, &psubkey, &psetting, &ptype))
		goto Exit;

	if (!WriteRegHandler (proot, psubkey, psetting, ptype, pbMetaStructure->lpValue, TRUE))
		goto Exit;

	ok = TRUE;

Exit:
	free (pparam);
	return ok;
}

/******************************************************************************/
EXPORT_TYPE BOOL WriteRegSetting_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	LPWSTR pparam, proot, psubkey, psetting, ptype;
	BOOL ok;

	ok = FALSE;
	pparam = _wcsdup (pbMetaStructure->lpParameter);

	if (!ParseParameters (pparam, &proot, &psubkey, &psetting, &ptype))
		goto Exit;

	if (!WriteRegHandler (proot, psubkey, psetting, ptype, pbMetaStructure->lpValue, FALSE))
		goto Exit;

	ok = TRUE;

Exit:
	free (pparam);
	return ok;
}

/******************************************************************************/
BOOL DeleteRegHandler (LPWSTR proot, LPWSTR psubkey, LPWSTR psetting, BOOL persistent, BOOL create_reg_file)
{
	HKEY hroot;
	BOOL result;

	result = FALSE;

	// No blank values allowed
	if (wcslen (proot) == 0)
	{
		WRITELOG (L"Hive must not be blank");
		goto Exit;
	}

	if (wcslen (psubkey) == 0)
	{
		WRITELOG (L"Key must not be blank");
		goto Exit;
	}

	if (wcslen (psetting) == 0)
	{
		WRITELOG (L"Setting must not be blank");
		goto Exit;
	}

	// Check for a valid root
	if (!wcsicmp (proot, L"hklm"))
		hroot = HKEY_LOCAL_MACHINE;
	else if (!wcsicmp (proot, L"hkcu"))
		hroot = HKEY_CURRENT_USER;
	else if (!wcsicmp (proot, L"hkcr"))
		hroot = HKEY_CLASSES_ROOT;
	else if (!wcsicmp (proot, L"hku"))
		hroot = HKEY_USERS;
	else
	{
		WRITELOG (L"Unknown hive: '%s'", proot);
		goto Exit;
	}

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

	result = TRUE;

Exit:
	return result;
}

/******************************************************************************/
EXPORT_TYPE BOOL DeletePersistentRegSetting_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	LPWSTR pparam, proot, psubkey, psetting;
	BOOL ok;

	ok = FALSE;
	pparam = _wcsdup (pbMetaStructure->lpParameter);

	if (!ParseParameters (pparam, &proot, &psubkey, &psetting, NULL))
		goto Exit;

	if (!DeleteRegHandler (proot, psubkey, psetting, TRUE, FALSE))
		goto Exit;

	ok = TRUE;

Exit:
	free (pparam);
	return ok;
}

/******************************************************************************/
EXPORT_TYPE BOOL DeleteRegSetting_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	LPWSTR pparam, proot, psubkey, psetting;
	BOOL ok;

	ok = FALSE;
	pparam = _wcsdup (pbMetaStructure->lpParameter);

	if (!ParseParameters (pparam, &proot, &psubkey, &psetting, NULL))
		goto Exit;

	if (!DeleteRegHandler (proot, psubkey, psetting, FALSE, FALSE))
		goto Exit;

	ok = TRUE;

Exit:
	free (pparam);
	return ok;
}

/******************************************************************************/
EXPORT_TYPE BOOL Registry_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	if (!pbMetaStructure->lpValue)
		return FALSE;

	// Store values according to parameter name
	if (!wcsicmp (pbMetaStructure->lpParameter, L"hive"))
		WSAFECOPY (wszHive, pbMetaStructure->lpValue);
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"key"))
		WSAFECOPY (wszKey, pbMetaStructure->lpValue);
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"setting"))
		WSAFECOPY (wszSetting, pbMetaStructure->lpValue);
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"type"))
		WSAFECOPY (wszType, pbMetaStructure->lpValue);
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"persistent"))
		bPersistent = !wcsicmp (pbMetaStructure->lpValue, L"true");
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"value"))
	{
		// Write registry setting
		if (!WriteRegHandler (wszHive, wszKey, wszSetting, wszType, pbMetaStructure->lpValue, bPersistent))
			return FALSE;

		// Reset parameters on success
		ResetParameters ();
	}
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"delete"))
	{
		// Delete registry setting
		if (!DeleteRegHandler (wszHive, wszKey, wszSetting, bPersistent, TRUE))
			return FALSE;

		// Reset parameters on success
		ResetParameters ();
	}
	else
	{
		WRITELOG (L"Unknown parameter: '%s'", pbMetaStructure->lpParameter);
		return FALSE;
	}

	return TRUE;
}

/******************************************************************************/
EXPORT_TYPE BOOL Config_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	// Store values according to parameter name
	if (!wcsicmp (pbMetaStructure->lpParameter, L"write"))
		WSAFECOPY (wszConfigSetting, pbMetaStructure->lpValue);
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"value"))
	{
		// Write config setting

		// Reset parameters on success
		ResetConfig ();
	}
	else if (!wcsicmp (pbMetaStructure->lpParameter, L"delete"))
	{
		// Delete pbMetaStructure->lpValue setting
	}
	else
	{
		WRITELOG (L"Unknown parameter: '%s'", pbMetaStructure->lpParameter);
		return FALSE;
	}

	return TRUE;
}
