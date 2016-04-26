#include "SystemTimeModule.h"

/**
* \author	John Heywood (JHMH, VBC863)
* \date		April 2012
*/
BOOL CSystemTimeModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"SystemTime");
	return TRUE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		April 2012
*/
BOOL CSystemTimeModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create a new SNTP server and response object to associate with this
	//  module
	//  Create this module's instance data
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->pSystemTime = new CSystemTime(this);
	pData->pSNTPresponse = new NtpServerResponse;
	pInstStruct->pWrappedObj = pData;

	return TRUE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		April 2012
*/
BOOL CSystemTimeModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	
	if (pData)
	{
		delete pData->pSystemTime;
		delete pData->pSNTPresponse;
	}
	delete pData;

	return TRUE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		April 2012
*/
BOOL CSystemTimeModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;
	BOOL checkSNTPSeverIP = (cmp(pbMetaStructure->lpParameter, L"SntpServerIP") || cmp(pbMetaStructure->lpParameter, L"ServerIP"));
	if ( checkSNTPSeverIP && pbMetaStructure->lpValue != NULL && !cmp(pbMetaStructure->lpValue, L""))
	{
		char pcSNTPServer[MAX_URL];
		memset(pcSNTPServer, 0, MAX_URL);
		wcstombs(pcSNTPServer, pbMetaStructure->lpValue, 
			wcslen(pbMetaStructure->lpValue));
		if (pData->pSystemTime->GetSntpServerTime(pcSNTPServer, *pData->pSNTPresponse))
		{
			SYSTEMTIME sys = pData->pSNTPresponse->m_ReceiveTime;
			SetSystemTime(&sys);
		}
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetUtc") &&
		pbMetaStructure->lpValue != NULL && !cmp(pbMetaStructure->lpValue, L""))
	{
		pData->pSystemTime->SetTime(pbMetaStructure->lpValue, UTC_TIME);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetLocal") &&
		pbMetaStructure->lpValue != NULL && !cmp(pbMetaStructure->lpValue, L""))
	{
		pData->pSystemTime->SetTime(pbMetaStructure->lpValue, LOCAL_TIME);
	}
	else if (isIPv4Addr(pbMetaStructure->lpParameter))
	{
		// if the parameter is an IP address its a PB2 style command to set time via SNTP
		char pcSNTPServer[MAX_URL];
		memset(pcSNTPServer, 0, MAX_URL);
		wcstombs(pcSNTPServer, pbMetaStructure->lpParameter, 
			wcslen(pbMetaStructure->lpParameter));
		if (pData->pSystemTime->GetSntpServerTime(pcSNTPServer, *pData->pSNTPresponse))
		{
			SYSTEMTIME sys = pData->pSNTPresponse->m_ReceiveTime;
			SetSystemTime(&sys);
		}
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_INFO, L"Unrecognised Meta Tag Provided to SystemTime Module, possibly from Regex'd PB2.x tags",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		April 2012
*/
BOOL CSystemTimeModule::isIPv4Addr(LPCWSTR pIpAddr)
{
	LPWSTR pAddr = NULL;
	LPWSTR pSearch = NULL;
	BOOL bRet = FALSE;
	int iLen = wcslen(pIpAddr);
	if ((iLen > 6) && (iLen < 16)) // 4 decimals between 1 and 3 chars each, plus 3 '.' characters
	{
		pAddr = new WCHAR[iLen];
	}

	if (pAddr)
		wcscpy(pAddr, pIpAddr);

	pSearch = pAddr;

	// tokenize the string into 3 decimal values between 0 and 256, each followed by a '.'
	for (int i=0; i<3; i++)
	{
		pSearch = wcstok(pSearch, L"."); // safe to call wcstok with NULL pointer
		// if the '.' wasn't found exit
		if (!pSearch)
			break;

		// if the token is more than 3 chars (eg. it has too many leading zeros) exit
		if (wcslen(pSearch) > 3)
		{
			pSearch = NULL;
			break;
		}

		// there must be at least 1 decimal digit with a value between 0 and 255
		if (isdigit((WCHAR)*pSearch) && (wcstol(pSearch, &pSearch, 10) < 256))
		{
			// if there were no non-decimal characters found the search pointer
			// should be pointing to NULL (strtok will have replaced the . with NULL)
			if ((WCHAR)*pSearch == L'\0')
			{
				// skip over the NULL
				pSearch++;
			}
			else
			{
				// not a decimal between 0 and 255
				pSearch = NULL;
				break;
			}
		}
	}

	if (pSearch)
	{
		// if we've found 3 'x.', 'xx.' or 'xxx.' substrings, look for the final decimal value
		if ((wcslen(pSearch) <= 3) && (isdigit((WCHAR)*pSearch)) && (wcstol(pSearch, &pSearch, 10) < 256))
			bRet = TRUE;
	}

	delete [] pAddr;
	return bRet;
}