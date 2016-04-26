// Generic.cpp : Implementation of CGeneric


#include "stdafx.h"
#include "Generic.h"
#include "Ras.h"
#include "..\..\Common\Private\GenericHeader.h"
#include "uniqueid.h"

#include "RcmCAPI.h"
//#pragma comment(lib, "RcmAPI32.lib")
#include "../../common/Private/RcmLoader.h"

#include <msgqueue.h>



// CGeneric
HRASCONN g_hRasConn = NULL;

STDMETHODIMP CGeneric::PlayWave(BSTR strSound, LONG iFlags)
{
// SND_SYNC       0x00000000   // play synchronously (default)
// SND_ASYNC      0x00000001   // play asynchronously
// SND_NODEFAULT  0x00000002   // silence not default, if sound not found
// SND_MEMORY     0x00000004   // lpszSoundName points to a memory file
// SND_LOOP       0x00000008   // loop the sound until next sndPlaySound
// SND_NOSTOP     0x00000010   // don't stop any currently playing sound

	
	if (sndPlaySound (strSound, iFlags))
		return S_OK;

	return S_OK;
}

STDMETHODIMP CGeneric::PlaySound(BSTR strWaveFile)
{
	return PlayWave (strWaveFile, 0);
}

/******************************************************************************/
bool QueueMessageNoResponse (void *pdata, int size)
{
	bool ok;
	HANDLE out_queue;
	MSGQUEUEOPTIONS options;

	out_queue = NULL;

	ok = false;

    WCHAR szMessageQueueName[40];
    DWORD dwEngineProcId = GetCurrentProcessId();
    
    wsprintf(szMessageQueueName, L"%s%lu", NEON_TO_HOST, dwEngineProcId);    

	// Open queue to host
	memset (&options, 0, sizeof options);
	options.dwSize = sizeof options;
	options.dwFlags = MSGQUEUE_NOPRECOMMIT | MSGQUEUE_ALLOW_BROKEN;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = NEON_MAX_MESSAGE_LENGTH;
	options.bReadAccess = FALSE;
	if ((out_queue = CreateMsgQueue (szMessageQueueName, &options)) == NULL)
		goto Exit;

	// Send message
	if (!WriteMsgQueue (out_queue, pdata, size, 0, 0))
		goto Exit;

	ok = true;

Exit:
	if (out_queue)
		CloseMsgQueue (out_queue);

	return ok;
}

/******************************************************************************/
bool QueueMessage (void *pdata, int data_size, void *presponse, int response_size, int timeout)
{
	bool ok;
	HANDLE in_queue, out_queue;
	MSGQUEUEOPTIONS options;
	DWORD count, flags;

	in_queue = NULL;
	out_queue = NULL;

	ok = false;

    WCHAR szMessageQueueName[40];
    DWORD dwEngineProcId = GetCurrentProcessId();

    wsprintf(szMessageQueueName, L"%s%lu", NEON_TO_HOST, dwEngineProcId);

	// Open queue to host
	memset (&options, 0, sizeof options);
	options.dwSize = sizeof options;
	options.dwFlags = MSGQUEUE_NOPRECOMMIT | MSGQUEUE_ALLOW_BROKEN;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = NEON_MAX_MESSAGE_LENGTH;
	options.bReadAccess = FALSE;
	if ((out_queue = CreateMsgQueue (szMessageQueueName, &options)) == NULL)
		goto Exit;

    memset(szMessageQueueName, 0, 40);
    wsprintf(szMessageQueueName, L"%s%lu", NEON_FROM_HOST, dwEngineProcId);

	// Open queue from host
	options.bReadAccess = TRUE;
	if ((in_queue = CreateMsgQueue (szMessageQueueName, &options)) == NULL)
		goto Exit;

	// Send message
	if (!WriteMsgQueue (out_queue, pdata, data_size, 0, 0))
		goto Exit;

	// Get response
	if (!ReadMsgQueue (in_queue, presponse, response_size, &count, timeout, &flags))
		goto Exit;

	ok = true;

Exit:
	if (out_queue)
		CloseMsgQueue (out_queue);

	if (in_queue)
		CloseMsgQueue (in_queue);

	return ok;
}

/******************************************************************************/
STDMETHODIMP CGeneric::InvokeMETAFunction(BSTR strHTTPEquiv, BSTR strContent, LONG* iReturn)
{
	NEONHEADER *pheader;
	BYTE *pbuffer;
	int nequiv, ncontent, size;
	HRESULT result;

	// Get lengths of parameters in bytes
	nequiv = (wcslen (strHTTPEquiv) + 1) * 2;
	ncontent = (wcslen (strContent) + 1) * 2;

	// Allocate complete buffer
	size = sizeof (NEONHEADER) + nequiv + ncontent;
	pbuffer = new BYTE [size];

	// Fill in header at start of buffer
	pheader = (NEONHEADER*) pbuffer;
	pheader->uType = NHT_INVOKE;

	// Copy parameters into buffer
	wcscpy ((WCHAR*) (pbuffer + sizeof (NEONHEADER)), strHTTPEquiv);
	wcscpy ((WCHAR*) (pbuffer + sizeof (NEONHEADER) + nequiv), strContent);

	// Send message but do not expect to receive a response
	if (QueueMessageNoResponse (pbuffer, size))
	{
		*iReturn = 1;
		result = S_OK;
	}
	else
	{
		Error (L"Generic ActiveX component unable to communicate with the core");
		*iReturn = 0;
		result = E_FAIL;
	}
			
	delete [] pbuffer;

	return result;
}

/******************************************************************************/
STDMETHODIMP CGeneric::HideHourglass(void)
{
	NEONHEADER header;

	header.uType = NHT_HIDE_HOURGLASS;
	
	if (QueueMessageNoResponse (&header, sizeof header))
		return S_OK;
	else
		return S_FALSE;
}

/******************************************************************************/
STDMETHODIMP CGeneric::ShowHourglass(void)
{
	NEONHEADER header;

	header.uType = NHT_SHOW_HOURGLASS;
	
	if (QueueMessageNoResponse (&header, sizeof header))
		return S_OK;
	else
		return S_FALSE;
}


/******************************************************************************/
STDMETHODIMP CGeneric::RasConnect(BSTR strEntry, BSTR strUser, BSTR strPwd, BSTR strDomain)
{
	RASDIALPARAMS RasDialParams;
	DWORD dwRet;
	RASENTRY rasentry;
	DWORD dwEntrySize = sizeof(rasentry);
	RASCONNSTATUS RasStatus;
	RASCONN ras[20];
	DWORD dwSize, dwNumber, dwCount, dwReturn;

	for (dwCount = 0; dwCount <20; dwCount++) ras[dwCount].dwSize = sizeof(RASCONN);
	dwSize = sizeof(ras);
	dwReturn = -1;
	// Get active RAS - Connection
	if( RasEnumConnections( ras, &dwSize, &dwNumber ) == 0 ) {
		for( dwCount = 0; dwCount < dwNumber; dwCount++ ) {
			if (!_tcscmp (ras[dwCount].szEntryName, strEntry)) {
				RasStatus.dwSize = sizeof (RASCONNSTATUS);
				RasGetConnectStatus (ras[dwCount].hrasconn, &RasStatus);
				if (RASCS_Connected == RasStatus.rasconnstate) return S_OK;
				break;
			}
		}
	}


	// No connection so make one
	rasentry.dwSize = sizeof(RASENTRY);
	dwEntrySize = sizeof(rasentry);
	RasGetEntryProperties(NULL, strEntry, &rasentry, &dwEntrySize, NULL, NULL);
	
	rasentry.dwfOptions = RASEO_DialAsLocalCall;
	wcscpy(rasentry.szAreaCode, _T(""));

	dwEntrySize = sizeof(rasentry);
	RasSetEntryProperties( NULL, strEntry, &rasentry, dwEntrySize, NULL, NULL); 

	// Configure the RASDIALPARAMS structure. 
    RasDialParams.dwSize = sizeof (RASDIALPARAMS);
    RasDialParams.szPhoneNumber[0] = TEXT('\0');
    RasDialParams.szCallbackNumber[0] = TEXT('\0');
    wcscpy (RasDialParams.szEntryName, strEntry);
    wcscpy (RasDialParams.szUserName, strUser);
    wcscpy (RasDialParams.szPassword, strPwd);
    wcscpy (RasDialParams.szDomain, strDomain); 
    
	dwRet = RasDial(NULL, NULL, &RasDialParams, 0xFFFFFFFF, NULL, &g_hRasConn);
	if (dwRet != 0) {
		TCHAR tcErr[256];
		wsprintf(tcErr, L"Error Connecting %d", dwRet);
		MessageBox(NULL, tcErr, L"Error connecting", MB_OK);
		g_hRasConn = NULL;
		return S_FALSE;
	}

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::RasDisconnect(BSTR strEntry)
{
	RASCONN ras[20];
	DWORD dwSize, dwNumber, dwCount, dwReturn;

	for (dwCount = 0; dwCount <20; dwCount++) ras[dwCount].dwSize = sizeof(RASCONN);
	dwSize = sizeof(ras);
	dwReturn = -1;
	// Get active RAS - Connection
	if( RasEnumConnections( ras, &dwSize, &dwNumber ) == 0 )
	{
		for( dwCount = 0; dwCount < dwNumber; dwCount++ )
		{
			if (!_tcscmp (ras[dwCount].szEntryName, strEntry))
			{
				RasHangUp(ras[dwCount].hrasconn);
				ras[dwCount].hrasconn = NULL;
				break;
			}				
		}
	}
	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::get_ConnectionStatus(BSTR strEntry, long *pVal)
{
	RASCONNSTATUS RasStatus;
	RASCONN ras[20];
	DWORD dwSize, dwNumber, dwCount, dwReturn;

	for (dwCount = 0; dwCount <20; dwCount++) ras[dwCount].dwSize = sizeof(RASCONN);
	dwSize = sizeof(ras);
	dwReturn = -1;
	// Get active RAS - Connection
	if( RasEnumConnections( ras, &dwSize, &dwNumber ) == 0 )
	{
		for( dwCount = 0; dwCount < dwNumber; dwCount++ )
		{
			if (!_tcscmp (ras[dwCount].szEntryName, strEntry))
			{
				// Get the connection status.
				RasStatus.dwSize = sizeof (RASCONNSTATUS);
				RasGetConnectStatus (ras[dwCount].hrasconn, &RasStatus);
				dwReturn = RasStatus.rasconnstate;
				break;
			}				
		}
	}
	*pVal = dwReturn;
	return S_OK;
}



/******************************************************************************/
STDMETHODIMP CGeneric::SetRegistrySetting(LONG lKey, LONG lType, BSTR sSubKey, BSTR sSetting, BSTR psData)
{
	bool ok;
	HKEY hkey;
	DWORD disposition, value;
	LONG result;
	WCHAR *ps;

	hkey = 0;

	ok = false;

	switch (lKey)
	{
		case 1:
			result = RegCreateKeyEx( HKEY_CURRENT_USER, sSubKey, 0, NULL, 0, 0, 0, &hkey, &disposition ); 
			break;

		case 2:
			result = RegCreateKeyEx( HKEY_LOCAL_MACHINE, sSubKey, 0, NULL, 0, 0, 0, &hkey, &disposition ); 
			break;

		case 3:
			result = RegCreateKeyEx( HKEY_USERS, sSubKey, 0, NULL, 0, 0, 0, &hkey, &disposition ); 
			break;

		default:
			result = RegCreateKeyEx( HKEY_CLASSES_ROOT, sSubKey, 0, NULL, 0, 0, 0, &hkey, &disposition ); 
			break;
	}	

	if (result != ERROR_SUCCESS)
	{
		ps = new WCHAR [wcslen (sSubKey) + 64];
		wsprintf (ps, L"Can't create key %s", sSubKey);
		Error (ps);
		delete [] ps;
		goto Exit;
	}

	switch (lType)
	{
		case REG_DWORD:
			value = (DWORD) wcstol (psData, NULL, 10);
			result = RegSetValueEx (hkey, sSetting, 0, REG_DWORD, (const BYTE*) &value, sizeof value); 
			break;

		default:
			result = RegSetValueEx(hkey, sSetting, 0, lType, (const BYTE*) psData, (wcslen (psData) + 1) * 2); 
			break;
	}

	if (result != ERROR_SUCCESS)
	{
		ps = new WCHAR [wcslen (sSetting) + 64];
		wsprintf (ps, L"Can't write setting %s", sSetting);
		Error (ps);
		delete [] ps;
		goto Exit;
	}

	ok = true;

Exit:
	if (hkey)
		RegCloseKey(hkey);

	if (!ok)
		return E_FAIL;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::GetRegistrySetting(LONG lKey, BSTR sSubKey, BSTR sSetting, BSTR* psData)
{
	bool ok;
	HKEY hkey;
	LONG result;
	DWORD type, size;
	BYTE *pbuffer;
	WCHAR *ps;

	hkey = 0;
	pbuffer = 0;

	ok = false;

	// Open registry key
	switch (lKey)
	{
	case 1:
		result = RegOpenKeyEx (HKEY_CURRENT_USER, sSubKey, 0, 0, &hkey);
		break;

	case 2:
		result = RegOpenKeyEx (HKEY_LOCAL_MACHINE, sSubKey, 0, 0, &hkey);
		break;

	case 3:
		result = RegOpenKeyEx (HKEY_USERS, sSubKey, 0, 0, &hkey);
		break;

	default:
		result = RegOpenKeyEx (HKEY_CLASSES_ROOT, sSubKey, 0, 0, &hkey);
		break;
	}	
	
	// Return empty string if key can't be opened (e.g. it doesn't exist)
	if (result != ERROR_SUCCESS)
	{
		ps = new WCHAR [wcslen (sSubKey) + 64];
		wsprintf (ps, L"Can't open key %s", sSubKey);
		Error (ps);
		delete [] ps;
		goto Exit;
	}

	// Get size of registry data
	if (RegQueryValueEx (hkey, sSetting, NULL, &type, NULL, &size) != ERROR_SUCCESS)
	{
		ps = new WCHAR [wcslen (sSetting) + 64];
		wsprintf (ps, L"Can't find setting %s", sSetting);
		Error (ps);
		delete [] ps;
		goto Exit;
	}

	// Allocate memory for value
	pbuffer = new BYTE [size];

	// Get registry value itself
	if (RegQueryValueEx (hkey, sSetting, NULL, &type, pbuffer, &size) != ERROR_SUCCESS)
		goto Exit;

	switch (type)
	{
		/*case REG_BINARY:
		{
			TCHAR tcHex[3];
			for(UINT ui=0; ui<RetSize; ui++){
				wsprintf(tcHex, L"%02X", bValue[ui]);
				wcscat(tcTemp, tcHex);
			}
			break;
		}*/

		case REG_DWORD:
			ps = new WCHAR [32];
			wsprintf (ps, L"%lu", *((DWORD*) pbuffer));
			*psData = SysAllocString (ps);
			delete [] ps;

			break;

		default:
			*psData = SysAllocString ((WCHAR*) pbuffer);
			break;
	}

	ok = true;

Exit:
	if (hkey)
		RegCloseKey (hkey);

	if (pbuffer)
		delete [] pbuffer;

	if (!ok)
	{
		*psData = SysAllocString (L"");
		return E_FAIL;
	}

	return S_OK;
}

/******************************************************************************/
bool CGeneric::GetPPC_UUID (WCHAR *puuid)
{
	bool ok;
	DEVICE_ID device_id, *pdevice_id;
	DWORD size, dw;
	BYTE *pbuffer;

	pbuffer = 0;

	ok = false;

	// Get required size of DEVICE_ID
	memset (&device_id, 0, sizeof device_id);

	if (KernelIoControl (IOCTL_HAL_GET_DEVICEID, NULL, 0, &device_id, sizeof device_id, &size) ||
		GetLastError () != ERROR_INSUFFICIENT_BUFFER)
	{
		Error (L"Can't get DEVICE_ID size");
		goto Exit;
	}

	// Allocate buffer for device id
	pbuffer = new BYTE [size];
	pdevice_id = (DEVICE_ID*) pbuffer;
	pdevice_id->dwSize = size;

	if (!KernelIoControl (IOCTL_HAL_GET_DEVICEID, NULL, 0, pbuffer, size, NULL))
	{
		Error (L"Can't get DEVICE_ID");
		goto Exit;
	}

	// Add preset identifier, if any
	for (dw = 0; dw < pdevice_id->dwPresetIDBytes; dw++)
	{
		puuid += wsprintf (puuid, L"%02X", pbuffer [pdevice_id->dwPresetIDOffset + dw]);
	}

	wcscat (puuid, L"-");
	puuid++;

	// Add platform identifier, if any
	for (dw = 0; dw < pdevice_id->dwPlatformIDBytes; dw++)
	{
		puuid += wsprintf (puuid, L"%02X", pbuffer [pdevice_id->dwPlatformIDOffset + dw]);
	}

	ok = true;

Exit:
	if (pbuffer)
		delete [] pbuffer;

	return ok;
}

void CGeneric::BytesToHexStr(LPTSTR lpHexStr, LPBYTE lpBytes, int nSize)
{
	//this function is lifted from the EMDK and converts the UUID into a sensible HEX string
	int		i;
	TCHAR	szByteStr[5];
	
	lpHexStr[0] = 0;

	for (i=0; i<nSize; i++)
	{
		wsprintf(szByteStr, TEXT("%02X"), lpBytes[i]);
		_tcscat(lpHexStr, szByteStr);
	}
}

/******************************************************************************/
bool CGeneric::GetCE_UUID (WCHAR *tcUID)
{
	UNITID_EX uuid;
	TCHAR serial[128];

	memset(serial, 0, 128);
	DWORD dwRes = 0;
	memset(&uuid, 0, sizeof(uuid));
	uuid.StructInfo.dwAllocated = sizeof(uuid);
	wcscpy(tcUID, L"GetSerialNumber: error!");	
	
	CRcmLoader* pRcm = new CRcmLoader();
	if (pRcm && pRcm->LoadRcmDLL())
	{
		if (E_RCM_SUCCESS == (dwRes = pRcm->lpfn_RCM_GetUniqueUnitIdEx(&uuid))) 
		{
			BytesToHexStr(tcUID, uuid.byUUID, uuid.StructInfo.dwUsed - sizeof(STRUCT_INFO)); 
			TCHAR tc1[17], tc2[17];
			if(tcUID[16] == 0)
			{
				wcscpy(tc1, tcUID);
				wcscpy(&tc1[8], L"00000000");
				wcscpy(tc2, &tcUID[8]);
				wcscpy(&tc2[8], L"00000000");
				wcscpy(tcUID, tc1);
				wcscat(tcUID, tc2);
			}
		}
	}
	delete pRcm;

	return true;


	/*
	PB2 algorithms:
	UNITID id;
	UNITID_EX id_ex;
	DWORD result;
	int n;

	result = RCM_GetUniqueUnitId (&id);

	if (result == E_RCM_SUCCESS)
	{
//		for (n = 0; n < id.StructInfo.dwUsed; n++)
//			wsprintf (puuid + (n * 2), L"%02X", id.byUUID [n]);
		wsprintf (puuid, L"%02X%02X%02X%02X00000000%02X%02X%02X%02X00000000",
			id [0], id [1], id [2], id [3], id [4], id [5], id [6], id [7]);

		return true;
	}

	if (result == E_RCM_NOTSUPPORTED)
	{
		// Try ex version
		if (RCM_GetUniqueUnitIdEx (&id_ex) == E_RCM_SUCCESS)
		{
			for (n = 0; n < id_ex.StructInfo.dwUsed; n++)
				wsprintf (puuid + (n * 2), L"%02X", id_ex.byUUID [n]);

			return true;
		}
	}
*/
	Error (L"Can't get unique unit ID");
	return false;
}

/******************************************************************************/
STDMETHODIMP CGeneric::get_UUID(BSTR* pVal)
{
	bool ok;
	WCHAR platform [32], uuid [64];

	ok = false;

	if (SystemParametersInfo (SPI_GETPLATFORMTYPE, 32, platform, 0) == 0)
	{
		Error (L"Can't get platform type");
		goto Exit;
	}

		// Assume Windows CE
		if (!GetCE_UUID (uuid))
			goto Exit;

	*pVal = SysAllocString (uuid);

	ok = true;

Exit:
	if (!ok)
	{
		*pVal = SysAllocString (L"");
		return E_FAIL;
	}

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::LaunchProcess(BSTR strImageName, BSTR strCommandLine, LONG* iReturn)
{
	PROCESS_INFORMATION proc_info;
	DWORD exit;

	memset (&proc_info, 0, sizeof proc_info);

	if (!CreateProcess (strImageName, strCommandLine, NULL, NULL, false, 0, NULL, NULL, NULL, &proc_info))
	{
		Error(L"Cannot start process");
		return E_FAIL;
	}

	// Don't need thread handle
	CloseHandle (proc_info.hThread);

	// Wait for process to exit
	WaitForSingleObject (proc_info.hProcess, INFINITE);

	// Return the exit code
	GetExitCodeProcess (proc_info.hProcess, &exit);
	*iReturn = exit;

	// Clean up
	CloseHandle (proc_info.hProcess);

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::LaunchProcessNonBlocking(BSTR strImageName, BSTR strCommandLine, LONG* iReturn)
{
	PROCESS_INFORMATION proc_info;

	memset (&proc_info, 0, sizeof proc_info);

	if (!CreateProcess (strImageName, strCommandLine, NULL, NULL, false, 0, NULL, NULL, NULL, &proc_info))
	{
		Error(L"Cannot start process");
		return E_FAIL;
	}

	// Don't need thread handle
	CloseHandle (proc_info.hThread);

	// Return the process handle
	*iReturn = (LONG) proc_info.hProcess;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::GetProcessExitCode(LONG iProcess, LONG* iReturn)
{
	DWORD exit;

	if (!GetExitCodeProcess ((HANDLE) iProcess, &exit))
	{
		Error (L"Invalid process handle");
		return E_FAIL;
	}

	*iReturn = exit;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::CloseProcess(LONG iProcess)
{
	CloseHandle ((HANDLE) iProcess);

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::WaitProcess(LONG iProcess, LONG iTimeout, VARIANT_BOOL* Finished)
{
	// Timeout is in seconds
	*Finished = (WaitForSingleObject ((HANDLE) iProcess, iTimeout * 1000) != WAIT_TIMEOUT);

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::Log(BSTR strComment, LONG nSeverity, VARIANT_BOOL* pOK)
{
	NEONHEADER *pheader;
	NEONLOG *plog;
	BYTE *pbuffer;
	int size;

	// Check for valid severity (1 = low, 2 = medium, 3 = high)
	if (nSeverity < 1 || nSeverity > 3)
	{
		Error (L"Severity must be between 1 and 3");
		return E_FAIL;
	}

	// Get length of data block in bytes
	size = sizeof (NEONHEADER) + sizeof (NEONLOG);
	size += (wcslen (strComment) + 1) * 2;

	// Allocate complete buffer
	pbuffer = new BYTE [size];

	// Fill in header at start of buffer
	pheader = (NEONHEADER*) pbuffer;
	pheader->uType = NHT_LOG;

	// Fill in log header
	plog = (NEONLOG*) (pbuffer + sizeof (NEONHEADER));
	plog->uSeverity = (UINT) nSeverity;

	// Copy comment into buffer
	wcscpy ((WCHAR*) (pbuffer + sizeof (NEONHEADER) + sizeof (NEONLOG)), strComment);

	// Send message
	*pOK = QueueMessageNoResponse (pbuffer, size);
			
	delete [] pbuffer;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::SPBLog(BSTR strComment, BSTR strSeverity, int *iReturn)
{
	LONG severity;
	VARIANT_BOOL ok;

	if (!wcsicmp (strSeverity, L"low"))
		severity = 1;
	else if (!wcsicmp (strSeverity, L"medium"))
		severity = 2;
	else if (!wcsicmp (strSeverity, L"high"))
		severity = 3;
	else
	{
		WCHAR *pws = new WCHAR [64];
		swprintf (pws, L"Invalid severity: %s", (LPCWSTR) strSeverity);
		Error (pws);
		delete [] pws;
		return E_FAIL;
	}

	Log (strComment, severity, &ok);
	*iReturn = (int) ok;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CGeneric::WriteUserSetting(BSTR strSetting, BSTR strValue, VARIANT_BOOL* pOK)
{
	NEONHEADER *pheader;
	NEONRESPONSE *presponse;
	BYTE *pin_buffer, *pout_buffer;
	int nsetting, nvalue, size;
	HRESULT result;

	// Get lengths of parameters in bytes
	nsetting = (wcslen (strSetting) + 1) * 2;
	nvalue = (wcslen (strValue) + 1) * 2;

	// Allocate complete buffer
	size = sizeof (NEONHEADER) + nsetting + nvalue;
	pout_buffer = new BYTE [size];

	// Fill in header at start of buffer
	pheader = (NEONHEADER*) pout_buffer;
	pheader->uType = NHT_WRITE_USER_SETTING;

	// Copy parameters into buffer
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER)), strSetting);
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER) + nsetting), strValue);

	// Send message and get response into buffer
	pin_buffer = new BYTE [NEON_MAX_MESSAGE_LENGTH];

	if (QueueMessage (pout_buffer, size, pin_buffer, NEON_MAX_MESSAGE_LENGTH, 10000))
	{
		presponse = (NEONRESPONSE*) pin_buffer;

		// Response value is non-zero for error
		*pOK = (presponse->iValue == 0);
		result = S_OK;
	}
	else
	{
		Error (L"No response from the core");
		*pOK = FALSE;
		result = E_FAIL;
	}
		
	if (pin_buffer)
		delete [] pin_buffer;

	if (pout_buffer)
		delete [] pout_buffer;

	return result;
}

/******************************************************************************/
STDMETHODIMP CGeneric::ReadUserSetting(BSTR strSetting, BSTR *pstrValue)
{
	NEONHEADER *pheader;
	BYTE *pin_buffer, *pout_buffer;
	int size, nsetting;
	NEONRESPONSE *presponse;
	LPCWSTR pvalue;
	HRESULT result;

	pin_buffer = 0;
	pout_buffer = 0;

	// Get lengths of parameters in bytes
	nsetting = (wcslen (strSetting) + 1) * 2;

	// Allocate complete buffer
	size = sizeof (NEONHEADER) + nsetting;
	pout_buffer = new BYTE [size];

	// Fill in header at start of buffer
	pheader = (NEONHEADER*) pout_buffer;
	pheader->uType = NHT_READ_USER_SETTING;

	// Copy parameters into buffer
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER)), strSetting);

	// Send message and get response into buffer
	pin_buffer = new BYTE [NEON_MAX_MESSAGE_LENGTH];

	if (QueueMessage (pout_buffer, size, pin_buffer, NEON_MAX_MESSAGE_LENGTH, 10000))
	{
		presponse = (NEONRESPONSE*) pin_buffer;

		// Response value is non-zero for error (e.g. setting not found)
		if (presponse->iValue != 0)
			*pstrValue = SysAllocString (L"");
		else
		{
			// Value follows response structure
			pvalue = (LPCWSTR) (pin_buffer + sizeof (NEONRESPONSE));
			*pstrValue = SysAllocString (pvalue);
		}

		result = S_OK;
	}
	else
	{
		Error (L"No response from the core");
		*pstrValue = NULL;
		result = E_FAIL;
	}
		
	if (pin_buffer)
		delete [] pin_buffer;

	if (pout_buffer)
		delete [] pout_buffer;

	return result;
}

/******************************************************************************/
STDMETHODIMP CGeneric::WriteConfigSetting(BSTR strSetting, BSTR strValue, BSTR strApp, VARIANT_BOOL* pOK)
{
	NEONHEADER *pheader;
	NEONRESPONSE *presponse;
	BYTE *pin_buffer, *pout_buffer;
	int nsetting, nvalue, napp, size;
	HRESULT result;

	pin_buffer = 0;
	pout_buffer = 0;

	// Get lengths of parameters in bytes
	nsetting = (wcslen (strSetting) + 1) * 2;
	nvalue = (wcslen (strValue) + 1) * 2;
	napp = (wcslen (strApp) + 1) * 2;

	// Allocate complete buffer
	size = sizeof (NEONHEADER) + nsetting + nvalue + napp;
	pout_buffer = new BYTE [size];

	// Fill in header at start of buffer
	pheader = (NEONHEADER*) pout_buffer;
	pheader->uType = NHT_WRITE_CONFIG_SETTING;

	// Copy parameters into buffer
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER)), strSetting);
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER) + nsetting), strValue);
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER) + nsetting + nvalue), strApp);

	// Send message and get response into buffer
	pin_buffer = new BYTE [NEON_MAX_MESSAGE_LENGTH];

	if (QueueMessage (pout_buffer, size, pin_buffer, NEON_MAX_MESSAGE_LENGTH, 10000))
	{
		presponse = (NEONRESPONSE*) pin_buffer;

		// Response value is non-zero for error
		*pOK = (presponse->iValue == 0);
		result = S_OK;
	}
	else
	{
		Error (L"No response from the core");
		*pOK = FALSE;
		result = E_FAIL;
	}
		
	if (pin_buffer)
		delete [] pin_buffer;

	if (pout_buffer)
		delete [] pout_buffer;

	return result;
}

/******************************************************************************/
STDMETHODIMP CGeneric::ReadConfigSetting(BSTR strSetting, BSTR strApp, BSTR *pstrValue)
{
	NEONHEADER *pheader;
	BYTE *pin_buffer, *pout_buffer;
	int size, nsetting, napp;
	NEONRESPONSE *presponse;
	LPCWSTR pvalue;
	HRESULT result;

	pin_buffer = 0;
	pout_buffer = 0;

	// Get lengths of parameters in bytes
	nsetting = (wcslen (strSetting) + 1) * 2;
	napp = (wcslen (strApp) + 1) * 2;

	// Allocate complete buffer
	size = sizeof (NEONHEADER) + nsetting + napp;
	pout_buffer = new BYTE [size];

	// Fill in header at start of buffer
	pheader = (NEONHEADER*) pout_buffer;
	pheader->uType = NHT_READ_CONFIG_SETTING;

	// Copy parameters into buffer
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER)), strSetting);
	wcscpy ((WCHAR*) (pout_buffer + sizeof (NEONHEADER) + nsetting), strApp);

	// Send message and get response into buffer
	pin_buffer = new BYTE [NEON_MAX_MESSAGE_LENGTH];

	if (QueueMessage (pout_buffer, size, pin_buffer, NEON_MAX_MESSAGE_LENGTH, 10000))
	{
		presponse = (NEONRESPONSE*) pin_buffer;

		// Response value is non-zero for error (e.g. setting not found)
		if (presponse->iValue != 0)
			*pstrValue = SysAllocString (L"");
		else
		{
			// Value follows response structure
			pvalue = (LPCWSTR) (pin_buffer + sizeof (NEONRESPONSE));
			*pstrValue = SysAllocString (pvalue);
		}

		result = S_OK;
	}
	else
	{
		Error (L"No response from the core");
		*pstrValue = NULL;
		result = E_FAIL;
	}
		
	if (pin_buffer)
		delete [] pin_buffer;

	if (pout_buffer)
		delete [] pout_buffer;

	return result;
}

/******************************************************************************/
STDMETHODIMP CGeneric::get_OEMInfo(BSTR* pVal)
{
	WCHAR info [64];

	if (SystemParametersInfo (SPI_GETOEMINFO, 64, info, 0))
		*pVal = SysAllocString (info);
	else
		*pVal = SysAllocString (L"Unknown");

	return S_OK;
}
