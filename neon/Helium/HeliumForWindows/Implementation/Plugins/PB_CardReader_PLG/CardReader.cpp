#include "stdafx.h"
#include "CardReader.h"


////////////////////////////////////////////////////////////////////////
// Function:	CCardReader
// Description:	CardReader-specific actions to be taken upon "Preloading" of the device,
//				essentially initializing the CardReader object, loading
//				the card reader DLL and setting object's method pointers to the DLL proper functions
//
// Scope:		Public
//								
// Info:		Called by PB_CardReader_PLG.cpp
//
// Return:		TRUE if successful at opening the card reader DLL
//				FALSE otherwise
// 	
//				
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
CCardReader::CCardReader(CCardReaderMod *pMod, int iID, HWND hWnd):
	m_hCardReaderLibrary(NULL),
	m_lpCardReaderOpen(NULL),
	m_lpCardReaderClose(NULL),
	m_lpCardReaderReadMsrUnbuffer(NULL),
	m_lpCardReaderFindFirst(NULL),
	m_lpCardReaderFindNext(NULL),
	m_lpCardReaderFindClose(NULL),
	m_lpCardReaderGetPinBlock(NULL),
	m_lpCardReaderMsrFlush(NULL),
	m_lpCardReaderGetVersion(NULL),

	m_bCardReaderHasBeenOpened(FALSE),

	m_hReader(INVALID_HANDLE_VALUE),
	m_hThreadRead(INVALID_HANDLE_VALUE),
	m_iCardReaderCount(0),
	m_bKeyEntry(FALSE),
	m_iWait_Time(DEFAULT_PIN_ENTRY_TIMEOUT),
	m_bPinEntry(FALSE),
	m_bAutoTab(FALSE),
	m_bAutoEnter(FALSE),
	m_pMod(pMod),
	m_iInstId(iID),
	m_hWnd(hWnd),
	m_bDCR(FALSE)
{

	// Load the DLL
	m_hCardReaderLibrary = LoadLibrary(DEFAULT_DLL_NAME);

	// If we loaded it, get function pointers
	if ( m_hCardReaderLibrary != NULL )
	{
		m_lpCardReaderOpen = (LPOPEN)GetProcAddress((HINSTANCE)m_hCardReaderLibrary,FUNC_NAME_OPEN);
		m_lpCardReaderClose = (LPCLOSE)GetProcAddress((HINSTANCE)m_hCardReaderLibrary,FUNC_NAME_CLOSE);
		m_lpCardReaderReadMsrUnbuffer = (LPREADMSRUNBUFFER)GetProcAddress((HINSTANCE)m_hCardReaderLibrary,FUNC_NAME_READMSRUNBUFFER);

		m_lpCardReaderFindFirst = (LPFINDFIRST) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FINDFIRST);
		m_lpCardReaderFindNext = (LPFINDNEXT) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FINDNEXT);
		m_lpCardReaderFindClose = (LPFINDCLOSE) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FINDCLOSE);
		m_lpCardReaderGetPinBlock	= (LPGETPINBLOCK) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_GETPINBLOCK);
		m_lpCardReaderMsrFlush		= (LPFLUSH)	GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FLUSH);	
		m_lpCardReaderGetVersion = (LPGETVERSION) GetProcAddress(m_hCardReaderLibrary, FUNC_NAME_GETVERSION);

		if ((m_lpCardReaderOpen == NULL) || (m_lpCardReaderClose == NULL) || (m_lpCardReaderReadMsrUnbuffer == NULL) ||
				(m_lpCardReaderFindFirst == NULL) || (m_lpCardReaderFindNext == NULL) || (m_lpCardReaderFindClose == NULL) ||
					  (m_lpCardReaderGetVersion == NULL))
		{
			// At least one function doesn't exist, we can't use the DLL
			UnloadTheCardReader();
			m_pMod->LogInst(PB_LOG_ERROR, L"ERROR: Could not find the CardReader functions - Unloading the CardReader.", L"OpenTheCardReader", __LINE__); 
		}
		else
			m_pMod->LogInst(PB_LOG_INFO, L"Loaded the CardReader DLL", L"CCardReader", __LINE__);
	}
	else
	{
		m_pMod->LogInst(PB_LOG_ERROR, L"Warning: Could not load the CardReader DLL", L"CCardReader", __LINE__);
	}

	m_pCSIP = new CSIP();

	if (m_pCSIP == NULL)
		m_pMod->LogInst(PB_LOG_WARNING, L"Failed to allocate memory for CSIP", L"CCardReader", __LINE__);

	wcscpy(m_lpszDriverName, L"");
	m_pMod->LogInst(PB_LOG_INFO, L"Initialized the CardReader object", L"CCardReader", __LINE__);
	return;
}

////////////////////////////////////////////////////////////////////////
// Function:	~CCardReader
// Description:	CardReader-specific actions to be taken upon "Disposing" of the device,
//				including stopping the read thread, deinitializing the object 
//				referring to the CardReader object, and unloading the CardReader DLL.
//
// Scope:		Public
//								
// Info:		Called by PB_CardReader_PLG.cpp
// 
// Return:		TRUE always
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
CCardReader::~CCardReader()
{
	m_pMod->LogInst(PB_LOG_INFO, L"Destroying the CardReader object", L"~CCardReader", __LINE__);
	StopTheCardReader(NO_FOCUS_SWITCH); 
	UnloadTheCardReader();
}


////////////////////////////////////////////////////////////////////////
// Function:	CardReaderMessage
// Description: Sends data back to the PB core. Under test conditions 
//				(PLUGINTESTER_DEBUG defined) prints this data to the 
//				console in a message box
//
// Scope:		Public
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::CardReaderMessage(int iID, LPTSTR lpszBuffer, LPTSTR lpszMessageType)
{
#ifdef PLUGINTESTER_DEBUG
		MessageBox(m_hWnd, (LPCWSTR) lpszBuffer, L"DEBUG MESSAGE", MB_OK);
#else
	m_pMod->LogInst(PB_LOG_INFO, L"Entering CardReaderMessage", L"CardReaderMessage", __LINE__);
	//If nav string is null and this is a CARDREADER_READ message, then we do sendkeys
	//if (cmp(lpszMessageType, CARDREADER_READ) && cmp(m_lpszCardReaderNavString, L""))
	if (m_lpszCardReaderNavString[0] == '\0')
	{
	    m_pMod->LogInst(PB_LOG_INFO, L"Send out like scan wedge", L"CardReaderMessage", __LINE__);
		//send it out like scanwedge
		m_pCSIP->SendString(lpszBuffer);

		if (m_bAutoEnter )
		{
 			m_pCSIP->SendKey(VK_RETURN);
		}
		else if(m_bAutoTab )
		{
			m_pCSIP->SendKey(VK_TAB);
		}
		else
		{
		}
	}
	else
	{
	    m_pMod->LogInst(PB_LOG_INFO, L"Call SendPBNavigate", L"CardReaderMessage", __LINE__);
		m_pMod->SendPBNavigate(tcReadEventNames, iID, m_lpszCardReaderNavString, lpszBuffer, lpszMessageType, NULL);
	}
#endif
}

////////////////////////////////////////////////////////////////////////
// Function:	EnumerateCardReaders
// Description:	Counts the number of CardReader driver DLL's found
//
// Scope:		Private
//
// Return:		Number of CardReader drivers found (0 if none found 
//				or CardReader library not open)			
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
int CCardReader::EnumerateCardReaders()
{
	HANDLE hFindHandle;
	DWORD dwStatus;
	int iCardReaderCount=0;
	TCHAR tcLogText[1024];
	
	// Make sure that the CardReader Library (MSRAPI32.dll) has been loaded
	if(m_hCardReaderLibrary == NULL)
		return iCardReaderCount;

	// Find first available MSR 3000 API compliant driver info
	dwStatus = m_lpCardReaderFindFirst(&m_CardReaderFindInfo[0], &hFindHandle);

	if(MSR_SUCCESS != dwStatus)
	{
		m_pMod->LogInst(PB_LOG_WARNING, L"No card reader drivers found on this device, please install drivers", _T(__FUNCTION__), __LINE__);
		return iCardReaderCount;
	}
	else
	{
		wsprintf(tcLogText, L"Found Card Reader Driver: %s (%s)", m_CardReaderFindInfo[0].szFriendlyName, m_CardReaderFindInfo[0].szDeviceName);
		m_pMod->LogInst(PB_LOG_INFO, tcLogText, _T(__FUNCTION__), __LINE__);
	}

	// Find all additional drivers
	for(iCardReaderCount=1; iCardReaderCount<MAX_DLLS; iCardReaderCount++)
	{
		dwStatus = m_lpCardReaderFindNext(&m_CardReaderFindInfo[iCardReaderCount], hFindHandle);

		if(MSR_SUCCESS != dwStatus)
			break;
		else
		{
			wsprintf(tcLogText, L"Found Card Reader Driver: %s (%s)", m_CardReaderFindInfo[iCardReaderCount].szFriendlyName, m_CardReaderFindInfo[iCardReaderCount].szDeviceName);
			m_pMod->LogInst(PB_LOG_INFO, tcLogText, _T(__FUNCTION__), __LINE__);
		}
	}

	// Close FindHandle
	m_lpCardReaderFindClose(hFindHandle);

	return iCardReaderCount;
}


////////////////////////////////////////////////////////////////////////
// Function:	OpenTheCardReader
// Description:	Steps through all of the CardReader drivers in the DLL and finds the one
//				that successfully opens with the attached device.
//
// Scope:		Public
//
// Return:		0 if successful in opening CardReader driver and all CardReader-related 
//				method pointers are non-zero
//				Non-zero if unsuccessful in opening CardReader driver or any CardReader-related 
//				method pointers are 0
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
DWORD CCardReader::OpenTheCardReader(BOOL bFocusSwitch)
{
	int i = 0;
	HANDLE hDummy = NULL;
	TCHAR lpszInfo[100];
	DWORD Status = MSR_ERR_API_NOTSUPPORTED;

	m_pMod->LogInst(PB_LOG_INFO, L"Entering OpenTheCardReader", L"OpenTheCardReader", __LINE__); 
	// On a focus switch, OpenTheCardReader is called only when card reader MUST be reopened
	// m_iCardReaderFindIndex has already been determined by a prior opening of the device
	if (bFocusSwitch)
	{
		Status = m_lpCardReaderOpen(m_CardReaderFindInfo[m_iCardReaderFindIndex].szDeviceName, m_CardReaderFindInfo[m_iCardReaderFindIndex].szPortName, &hDummy);
		if(MSR_SUCCESS == Status)
			m_hReader = hDummy;

		wsprintf(lpszInfo, L"Attempted to open cardreader on focus switch - status #%.16d", Status);
		m_pMod->LogInst(PB_LOG_INFO,lpszInfo , L"OpenTheCardReader", __LINE__);
	}
	else
	{
		if (!m_bCardReaderHasBeenOpened)
		{
			m_iCardReaderCount = EnumerateCardReaders();
			wsprintf(lpszInfo, L"Number of Card Readers on Device: %i", m_iCardReaderCount);
			m_pMod->LogInst(PB_LOG_INFO, lpszInfo, _T(__FUNCTION__), __LINE__);

			// Open the card reader
			// The Browser Application does not provide the list of CardReader drivers available to 
			// select the correct CardReader driver based on the CardReader unit attached. Therefore, the user
			// is required to specify the attached card reader device prior to calling OpenTheCardReader. We look for
			// the presence of the appropriate driver and then open the card reader.
			if (m_iCardReaderCount == 0)
			{
				m_pMod->LogInst(PB_LOG_WARNING, L"Not opening card reader as no drivers are installed on this device.", _T(__FUNCTION__), __LINE__);
			}
			else if (m_iCardReaderCount > 1)
			{
				for(i=0; i<m_iCardReaderCount; i++) 
				{
					if (!_wcsicmp(m_lpszDriverName, m_CardReaderFindInfo[i].szDeviceName)) 
					{
						wsprintf(lpszInfo, L"Opening Card Reader (User Specified): %s", m_CardReaderFindInfo[i].szDeviceName);
						m_pMod->LogInst(PB_LOG_INFO, lpszInfo, _T(__FUNCTION__), __LINE__);	
						Status = m_lpCardReaderOpen(m_CardReaderFindInfo[i].szDeviceName, m_CardReaderFindInfo[i].szPortName, &hDummy);
					}
				}
				//  It is possible the user specified driver was not found or 
				//  the driver failed to load, test that here and open successive
				//  drivers in the case of failure
				if (Status != MSR_SUCCESS)
				{
					//  Failed to open user specified driver
					for (i=0; i < m_iCardReaderCount; i++)
					{
						wsprintf(lpszInfo, L"Attempting to find valid card reader driver: %s", m_CardReaderFindInfo[i].szDeviceName);
						m_pMod->LogInst(PB_LOG_WARNING, lpszInfo, _T(__FUNCTION__), __LINE__);	
						Status = m_lpCardReaderOpen(m_CardReaderFindInfo[i].szDeviceName, m_CardReaderFindInfo[i].szPortName, &hDummy);
						if (Status == MSR_SUCCESS)
						{
							break;
						}
						else
							m_lpCardReaderClose(hDummy);
					}
				}
			}
			else if (m_iCardReaderCount == 1)
			{
				//  Only one Card Reader on device, open that
				wsprintf(lpszInfo, L"Opening Card Reader: %s", m_CardReaderFindInfo[0].szDeviceName);
				m_pMod->LogInst(PB_LOG_INFO, lpszInfo, _T(__FUNCTION__), __LINE__);	
				Status = m_lpCardReaderOpen(m_CardReaderFindInfo[0].szDeviceName, m_CardReaderFindInfo[0].szPortName, &hDummy);
			}

			if ( Status == MSR_SUCCESS )
			{
				// Indicate we are now open
				m_bCardReaderHasBeenOpened = TRUE;
				m_iCardReaderFindIndex = i;
				m_hReader = hDummy;
				m_pMod->LogInst(PB_LOG_INFO, L"Card Reader successfully opened", _T(__FUNCTION__), __LINE__);	

				// Get the version info. The firmware version will be used for determining 
				// if the hardware is DCR. It is potentially usefull for determining the device 
				// attached is the device specified by the user. However, the list of FWVersion names is not 
				// available to me at this time so the code performs a weaker check. Basically, the m_bDCR is set 
				// to TRUE if the attached device is a DCR, as confirmed by FWVersion, and the user specifies 
				// the device as a DCR, through modulename. Otherwise, the m_bDCR is set to FALSE.
				// Both the FWVersion and modulename must agree that the device is an MSR or a DCR or an 
				// error is reported and the cardreader is closed.
				m_lpCardReaderGetVersion(m_hReader, &m_CardReaderVer);

				if (m_lpCardReaderGetPinBlock)
					m_bDCR = TRUE;
				else
					m_bDCR = FALSE;


				m_pMod->LogInst(PB_LOG_INFO, L"CardReader opened", L"OpenTheCardReader", __LINE__);
			}
			else
			{
				m_bDCR = FALSE;
			}
		}
		else
		{
			// Card reader already opened, so opening is an automatic success
			Status = MSR_SUCCESS;
			m_pMod->LogInst(PB_LOG_INFO, L"Card reader already opened", L"OpenTheCardReader", __LINE__); 
		}
	}

	// If open was a success, need to check to see that the attached hardware matches the type specified by
	// the user by modulename. Also need to set the m_bDCR flag appropriately.
	// Status could be MSR_ERR_STILLOPEN if we are trying to reopen the cardreader but it was not closed by
	// the transition to the licensing/timeout screen
	if (Status != MSR_SUCCESS && Status != MSR_ERR_STILLOPEN)
	{
		// Somehow the card reader was not opened
		m_hReader = INVALID_HANDLE_VALUE;
		m_pMod->LogInst(PB_LOG_ERROR, L"Unable to open driver for CardReader device, is it attached?", L"OpenTheCardReader", __LINE__); 
	}

	return Status;
}


////////////////////////////////////////////////////////////////////////
// Function:	CardReaderReadThread
// Description:	Responsible for forwarding data from the CardReader to the 
//				PB core in the form of a callback via SendPBNavigate(). 
//				CardReaderReadThread continually loops, waiting on the blocking 
//				function m_lpCardReaderReadMsrUnbuffer. Once data is received, 
//				m_lpCardReaderReadMsrUnbuffer returns and CardReaderReadThread sends
//				the data back to the PB core. If the CardReader buffer cannot 
//				be successfully read, then it resets the CardReader and tries 
//				again. If the device is a debit card reader, then CardReaderReadThread 
//				continues to PIN entry and forwards that data back to the PB core.	
//
// Scope:		Public					
//
// Return: 		0 if thread to terminate normally
//				1 if error in reading CardReader read buffer
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
DWORD WINAPI CCardReader::CardReaderReadThread (LPVOID param)
{
	TCHAR lpszLocalBuffer[MAX_CARD_DATA];
	DWORD dwReadStatus = 1;
	TCHAR lpszSendData[MAX_CARD_DATA];
	TCHAR lpszPANdata[50];

	CCardReader* pObj = (CCardReader*) param;
	pObj->m_pMod->LogInst(PB_LOG_INFO, L"Started CardReader Read Thread", L"CardReaderReadThread", __LINE__);

	for(;;)
	{
		// Clear the buffers
		memset(pObj->m_CardReaderReadDataBuffer,0,MAX_CARD_DATA * sizeof(TCHAR));
		memset(lpszLocalBuffer,0,MAX_CARD_DATA * sizeof(TCHAR));

		// Test to see if CardReader is opened. It could have been closed since the last
		// call to ReadMsrUnbuffer
		if (pObj->m_hReader == INVALID_HANDLE_VALUE)
			return 0;

		// Read the CardReader in unbuffered mode. Put result in lpszLocalBuffer
		// This is a blocking function, so the thread will wait here until data is read or the
		// MsrClose or MsrFlush functions are invoked
		dwReadStatus = pObj->m_lpCardReaderReadMsrUnbuffer(pObj->m_hReader, lpszLocalBuffer, 0);
		
		// If we are asked to stop, end execution of read thread
		if(WaitForSingleObject(pObj->m_pMod->GetStopEventHandle(),0) == WAIT_OBJECT_0)
			return 0;	// exit thread
		
		// Check for a read or manual (keyed in) PAN input
		if(pObj->m_bKeyEntry)
		{
			// The PAN has been set manually (keyed in)
			// Copy PAN data to lpszLocalBuffer
			_tcscpy(lpszLocalBuffer,pObj->m_CardReaderReadDataBuffer);
		}
		else if(dwReadStatus != MSR_SUCCESS)
		{
			// If there is an error 
			// Try to reset the CardReader to clear the error
			dwReadStatus = pObj->ResetTheCardReader();

			// If it is still an error
			if ( dwReadStatus != MSR_SUCCESS )
			{
				// Show the error
				pObj->m_pMod->LogInst(PB_LOG_WARNING, lpszLocalBuffer, L"CardReaderReadThread", __LINE__);
				pObj->CardReaderMessage(pObj->m_iInstId, lpszLocalBuffer, L"CARDREADER_MSG");
				return 1;
			}

			pObj->m_pMod->LogInst(PB_LOG_INFO, L"Continuing after reset", L"CardReaderReadThread", __LINE__);
			continue;
		}
		//we should now have our card data so let's process it for output
		UINT iLen = wcslen(lpszLocalBuffer);
		UINT j=0;
		memset (lpszSendData, 0, MAX_CARD_DATA * sizeof(TCHAR));

		//copy local buffer into send data buffer, while "escaping" special characters
		for(UINT i=0; i < iLen; i++){
			switch(lpszLocalBuffer[i]){
			case L'\f':
				wcscat(lpszSendData, L"\\f");
				j+=2;
				break;
			case L'\n':
				wcscat(lpszSendData, L"\\n");
				j+=2;
				break;
			case L'\r':
				wcscat(lpszSendData, L"\\r");
				j+=2;
				break;
			case L'\t':
				wcscat(lpszSendData, L"\\t");
				j+=2;
				break;
			case L'\"':
				wcscat(lpszSendData, L"\\\"");
				j+=2;
				break;
			case L'\'':
				wcscat(lpszSendData, L"\\'");
				j+=2;
				break;
			case L'\\':
				wcscat(lpszSendData, L"\\\\");
				j+=2;
				break;
			case L'\b':
				wcscat(lpszSendData, L"\\b");
				j+=2;
				break;
			case L'\?':
				wcscat(lpszSendData, L"\\?");
				j+=2;
				break;
			case L'%':
				wcscat(lpszSendData, L"\\045");
				j+=4;
				break;
			default:
				lpszSendData[j] = lpszLocalBuffer[i];
				++j;
				break;
			}
		}
	
		pObj->CardReaderMessage(pObj->m_iInstId, lpszSendData, CARDREADER_READ);
		_tcscpy(lpszPANdata,L"");

		// Validation of PAN data
		if(pObj->m_bDCR && pObj->m_bPinEntry && pObj->m_lpCardReaderGetPinBlock!=NULL){
			if(pObj->retPANData(lpszSendData,lpszPANdata)){
				// Valid PAN data
				pObj->CardReaderMessage(pObj->m_iInstId, lpszPANdata, DEBITCARDREADER_PAN);
				pObj->PinTransactionDebitCardReader(lpszPANdata);
			}
			else{
				// Invalid PAN data
				pObj->CardReaderMessage(pObj->m_iInstId, L"PAN undefined", CARDREADER_MSG);
			}
		}

		pObj->m_bKeyEntry = FALSE;
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////
// Function:	ResetTheCardReader
// Description:	Resets the CardReader by closing and then reopening driver to clear 
//				any error, if necessary
//
// Scope:		Private
//
// Return:		0 if successful
//				Non-zero if unable to close CardReader or successfully reopen
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
DWORD CCardReader::ResetTheCardReader()
{
	DWORD dwStatus;

	m_pMod->LogInst(PB_LOG_INFO, L"Resetting the CardReader", L"ResetTheCardReader", __LINE__);

	// Try to close and reopen the CardReader
	dwStatus = CloseTheCardReader(FALSE);

	// Reset m_hReader, as ResetTheCardReader is called from CardReaderReadThread, which
	// means it's not currently using m_hReader
	m_hReader = INVALID_HANDLE_VALUE;

	// If the close succeeded
	if ( dwStatus == MSR_SUCCESS )
	{
		// Try and reopen
		dwStatus = OpenTheCardReader(FALSE);
	}

	// Return the new status
	return dwStatus;
}


////////////////////////////////////////////////////////////////////////
// Function:	StopTheCardReader
// Description:	Stops the CardReader by telling the read thread to terminate, 
//				closing the CardReader driver, and clearing the thread pointer 
//				and handle. Terminates the read thread if it does not terminate 
//				on its own.
//
// Scope:		Public
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::StopTheCardReader(BOOL bFocusSwitch) 
{
	HANDLE hStopEvent;

	m_pMod->LogInst(PB_LOG_INFO, L"Stopping the Card Reader", L"StopTheCardReader", __LINE__);
	hStopEvent = m_pMod->GetStopEventHandle();

	// Tell thread we want it to stop
	if (hStopEvent != INVALID_HANDLE_VALUE)
		SetEvent(hStopEvent);

	// Close it
	CloseTheCardReader(bFocusSwitch);

	// If the read thread exists
	if (m_hThreadRead != INVALID_HANDLE_VALUE)
	{
		// Hold up the primary thread for no more than READ_THREAD_TIMEOUT milliseconds waiting for thread to stop
		if (WaitForSingleObject(m_hThreadRead, READ_THREAD_TIMEOUT) == WAIT_TIMEOUT)
		{
			// Kill it if it did not stop
			TerminateThread(m_hThreadRead, 0);
			m_pMod->LogInst(PB_LOG_WARNING, L"Read thread not responding - terminated", L"StopTheCardReader",  __LINE__);
		}
	}

	// Reset StopEvent in case above timeout failed or multiple consecutive "close" Metatags have been issued
	ResetEvent(hStopEvent);

	// No more read thread
	if (m_hThreadRead != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hThreadRead);
		m_hThreadRead = INVALID_HANDLE_VALUE;
	}

	// Reset m_hReader, as CardReaderReadThread is now ended
	m_hReader = INVALID_HANDLE_VALUE;
	m_pMod->LogInst(PB_LOG_INFO, L"Cardreader stopped", L"StopTheCardReader", __LINE__);
}


////////////////////////////////////////////////////////////////////////
// Function:	CloseTheCardReader
// Description:	Simply closes the CardReader driver by calling its MsrClose() method
//				Does not unload it. m_hReader must be set to INVALID_HANDLE_VALUE after calling
//				CloseTheCardReader, but only after the read thread has been terminated.
//
// Scope:		Private
//
// Return:		0 if able to close the CardReader or it was already closed
//				Non-zero if unable to close the CardReader
//								
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
DWORD CCardReader::CloseTheCardReader(BOOL bFocusSwitch) 
{
	DWORD Status = MSR_SUCCESS;
	HANDLE hKeypadMutex;

	m_pMod->LogInst(PB_LOG_INFO, L"Closing the CardReader", L"CloseTheCardReader", __LINE__); 

	// If the CardReader is open
	if ( m_bCardReaderHasBeenOpened )
	{
		//Grab mutex to make sure we are not closing card reader while waiting for keypad input
		hKeypadMutex = m_pMod->GetKeypadMutexHandle();
		WaitForSingleObject(hKeypadMutex, INFINITE);
		// Close it
		Status = m_lpCardReaderClose(m_hReader);

		if (!ReleaseMutex(hKeypadMutex))
			m_pMod->LogInst(PB_LOG_WARNING, L"Error: Can't release the keypad mutex", L"CloseTheCardReader", __LINE__); 

		// Don't set m_hReader to INVALID_HANDLE_VALUE yet as CardReaderReadThread may still be active
		// Instead, reset it in the code following a call to CloseTheCardReader

		// If we got an error
		if (( Status != MSR_SUCCESS ))
		{
			// Report it
			m_pMod->LogInst(PB_LOG_WARNING, L"Error: Can't close the CardReader", L"CloseTheCardReader", __LINE__); 
		}
		
		// Note we are no longer open, as long as this is not a focus switch
		if (!bFocusSwitch)
			m_bCardReaderHasBeenOpened = FALSE;
	}

	return Status;
}



////////////////////////////////////////////////////////////////////////
// Function:	UnloadTheCardReader
// Description:	Unloads the CardReader driver and sets all of the driver 
//				method pointers to NULL. Further use of the card reader 
//				will require creation of a new CardReader object.
//
// Scope:		Private
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::UnloadTheCardReader()
{
	m_pMod->LogInst(PB_LOG_INFO, L"Unloading the CardReader", L"UnloadTheCardReader", __LINE__); 

	if ( m_hCardReaderLibrary != NULL )
	{
		if (m_hCardReaderLibrary != INVALID_HANDLE_VALUE)
		{
			FreeLibrary((HINSTANCE)m_hCardReaderLibrary);
		}

		m_hCardReaderLibrary = NULL;
		m_lpCardReaderOpen = NULL;
		m_lpCardReaderClose = NULL;
		m_lpCardReaderReadMsrUnbuffer = NULL;
		m_lpCardReaderFindFirst = NULL;
		m_lpCardReaderFindNext = NULL;
		m_lpCardReaderFindClose = NULL;
		m_lpCardReaderGetPinBlock = NULL;
		m_lpCardReaderMsrFlush = NULL;
		m_lpCardReaderGetVersion = NULL;
	}

}

////////////////////////////////////////////////////////////////////////
// Function:	GetCardReaderHasBeenOpened
// Description:	Get the value of the attribute m_bCardReaderHasBeenOpened
//
// Scope:		Public
//
// Return:		value of the attribute m_bCardReaderHasBeenOpened
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::GetCardReaderHasBeenOpened()
{
	return m_bCardReaderHasBeenOpened;
}

////////////////////////////////////////////////////////////////////////
// Function:	SetCardReaderNavString
// Description:	Set the value of the attribute m_lpszCardReaderNavString 
//				to the first MAX_URL chars of lpszString
//
// Scope:		Public
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::SetCardReaderNavString(LPCTSTR lpszString)
{
	memset(m_lpszCardReaderNavString, 0, MAXURL);
	wcsncpy(m_lpszCardReaderNavString, lpszString, MAXURL);
}

////////////////////////////////////////////////////////////////////////
// Function:	GetThreadReadHandle
// Description:	Get the value of the attribute m_hThreadRead
//
// Scope:		Public
//
//
// Return:		value of the attribute m_hThreadRead
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
HANDLE CCardReader::GetThreadReadHandle()
{
	return m_hThreadRead;
}

////////////////////////////////////////////////////////////////////////
// Function:	SetThreadReadHandle
// Description:	Set the value of the attribute m_hThreadRead
//
// Scope:		Public
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::SetThreadReadHandle(HANDLE hHandle)
{
	m_hThreadRead = hHandle;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetAutoTab
// Description:	Set the value of the attribute bAutoTab
//
// Scope:		Public
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetAutoTab(LPCTSTR lpszValue)
{
	if (!_wcsicmp(lpszValue, L"disabled") || !_wcsicmp(lpszValue, L"disable"))  //  RegEx of 'Disabled'
		m_bAutoTab = FALSE;
	else if (!_wcsicmp(lpszValue, L"enabled") || !_wcsicmp(lpszValue, L"enable"))  //  RegEx of 'Enabled'
		m_bAutoTab = TRUE;
	else
	{
		m_pMod->LogInst(PB_LOG_INFO, L"Request to set Autotab to invalid value", L"SetAutoTab", __LINE__);
		return FALSE;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetAutoEnter
// Description:	Set the value of the attribute m_bAutoEnter
//
// Scope:		Public
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetAutoEnter(LPCTSTR lpszValue)
{
	if (!_wcsicmp(lpszValue, L"disabled") || !_wcsicmp(lpszValue, L"disable"))	//  RegEx of 'Disabled'
		m_bAutoEnter = FALSE;
	else if (!_wcsicmp(lpszValue, L"enabled") || !_wcsicmp(lpszValue, L"enable"))	//  RegEx of 'Enabled'
		m_bAutoEnter = TRUE;
	else
	{
		m_pMod->LogInst(PB_LOG_INFO, L"Request to set Autoenter to invalid value", L"SetAutoEnter", __LINE__);
        return FALSE;
	}

	return TRUE;
}


/***********************************************************************/
/********************** DebitCardReader Support ************************/
/***********************************************************************/
////////////////////////////////////////////////////////////////////////
// Function:	retPANData
// Description:	Parses the card data and picks out the PAN (personal 
//				account number) Data and uses it to populate 'lpSzPANData'
//
// Scope:		Private
//								
// Return:		TRUE if able to successfully parse lpSzCardData and put results in lpSzPANData
//				FALSE otherwise
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::retPANData(LPCTSTR lpszCardReaderData, LPTSTR lpszPANData)
{
	LPTSTR	lpszTempCardData;
	int		iLen;
	BOOL	bRet = FALSE;
/*
Track 1 (IATA) Start Sentinal "%" Separator "^" max 19 chars
Track 1 ("International Air Transport Association") stores more information than Track 2, and contains the 
cardholder's name as well as account number and other discretionary data. This track is sometimes used by the 
airlines when securing reservations with a credit card. 

Track 2 (ABA) Start Sentinal ";" Separator "=" max 19 chars
Track 2 ("American Banking Association,") is currently most commonly used, though credit card companies have 
been pushing for everyone to move to Track 1. This is the track that is read by ATMs and credit card checkers. 
The ABA designed the specifications of this track and all world banks must abide by it. It contains the cardholder's
account, encrypted PIN, plus other discretionary data.
*/

	LPTSTR lpszCardData = new TCHAR[wcslen(lpszCardReaderData)+ 1];

	if(lpszCardData){
		lpszTempCardData = wcsstr(lpszCardReaderData,L"\\045B");
		if(lpszTempCardData ){//if the card has the IATA delimiters
			lpszTempCardData+=5;
			_tcscpy(lpszCardData,lpszTempCardData);
			// Find first ^, after which is the PAN data
			lpszTempCardData = wcschr (lpszCardData,L'^' );
			if(lpszTempCardData){
				*lpszTempCardData = NULL;
				iLen = wcslen(lpszCardData);
				// Copy PAN data into lpszPANData
				if(iLen <= CARDREADER_DATA_MAX_PANLENGTH && iLen >= CARDREADER_DATA_MIN_PANLENGTH){
					_tcscpy(lpszPANData, lpszCardData);
					bRet =  TRUE;
				}

			}
		}
		// Support for ABA card, see above  
		else{
			lpszTempCardData = wcsstr(lpszCardReaderData,L";");
			if(lpszTempCardData){
				// Find first ;, copy chars immediately after, up until =
				_tcscpy(lpszCardData,++lpszTempCardData);
				lpszTempCardData = wcschr (lpszCardData,L'=' );
				if(lpszTempCardData){
					*lpszTempCardData = NULL;
					iLen = wcslen(lpszCardData);
					// Copy PAN data into lpSzPANData
					if(iLen <= CARDREADER_DATA_MAX_PANLENGTH && iLen >= CARDREADER_DATA_MIN_PANLENGTH){
						_tcscpy(lpszPANData,lpszCardData);
						bRet =  TRUE;
					}
				}
			}
		}
	}

	delete [] lpszCardData;

	return bRet;
}


////////////////////////////////////////////////////////////////////////
// Function:	PinTransactionDebitCardReader
// Description:	Submits the CardReader PAN data to the reader to produce an 
//				encrypted string for server validation 
//
// Scope:		Private
//
// Return:		1 if able to successfully encrypt CardReader PAN data
//				0 otherwise
// 	
//				
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
DWORD  CCardReader::PinTransactionDebitCardReader(LPCTSTR lpszPANData)
{
	UBYTE ubEncryptedPinBuffer[DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE + 1];
	DWORD dwReadStatus;
	DWORD iErr = 0;
	HANDLE hKeypadMutex;
	
	TCHAR byteToHexString[]={L"0123456789ABCDEF"};
	TCHAR lpszHexVal[DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE*2];	

	// Grab mutex to prevent closing of card reader while we wait for keypad input
	hKeypadMutex = m_pMod->GetKeypadMutexHandle();
	WaitForSingleObject(hKeypadMutex, INFINITE);

	if(m_hReader != INVALID_HANDLE_VALUE)
	{
		memset(ubEncryptedPinBuffer,0,sizeof(UBYTE) * (DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE) + 1);

		// Have DebitCardReader encrypt PAN data and put in ubEncryptedPinBuffer
		dwReadStatus = m_lpCardReaderGetPinBlock(m_hReader,lpszPANData, 0, m_iWait_Time, ubEncryptedPinBuffer);

		if (!ReleaseMutex(hKeypadMutex))
			m_pMod->LogInst(PB_LOG_WARNING, L"Error: Can't release the keypad mutex", L"PinTransactionDebitCardReader", __LINE__); 

		if(dwReadStatus == MSR_SUCCESS)
		{
			//Convert hex bytes to hex string for display
			int j;

			for(j = 0; j < DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE; j++){
				lpszHexVal[j*2] =		byteToHexString[((ubEncryptedPinBuffer[j] >> 4) & 0xF)];
				lpszHexVal[(j*2) + 1] =	byteToHexString[(ubEncryptedPinBuffer[j]) & 0x0F];
			}
				
			lpszHexVal[(j*2)]=NULL;
				
			// Send the message to the main proc
			// wParam = data; lParam = mode
			CardReaderMessage(m_iInstId, lpszHexVal, DEBITCARDREADER_ENC);
			iErr = 1;
		}	
		else{//error
			switch(dwReadStatus){
			case MSR_ERR_NOTOPEN :
				CardReaderMessage(m_iInstId, L"MSR_ERR_NOTOPEN", CARDREADER_MSG);
			break;
			case MSR_ERR_NODATA :
				CardReaderMessage(m_iInstId, L"MSR_ERR_NODATA", CARDREADER_MSG);
			break;
			case MSR_ERR_PARAMETER:
				CardReaderMessage(m_iInstId, L"MSR_ERR_PARAMETER", CARDREADER_MSG);
			break;
			case MSR_ERR_TIMEOUT:
				CardReaderMessage(m_iInstId, L"MSR_ERR_TIMEOUT", CARDREADER_MSG);
			break;
			default:
				CardReaderMessage(m_iInstId, L"UNKNOWN DCR ERROR", CARDREADER_MSG);
				break;
			};

			iErr = 0;
		}
	}
	else
	{
		if (!ReleaseMutex(hKeypadMutex))
			m_pMod->LogInst(PB_LOG_WARNING, L"Error: Can't release the keypad mutex", L"PinTransactionDebitCardReader", __LINE__); 

		iErr =  INVALIDREADERHANDLE;
	}

	return iErr;
}

////////////////////////////////////////////////////////////////////////
// Function:	ManualPANEntry
// Description:	Adds some formatting characters and the PAN data to the beginning 
// 				of the data buffer to be sent back to the PB core
//
// Scope:		Private
//
// Info			Called by Meta.cpp
//				
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::ManualPANEntry(LPCTSTR lpszPAN)
{
int iCharCount;
BOOL bValidString;
LPCTSTR lpszPANtmp;

	if (m_bPinEntry==TRUE)
	{
		iCharCount = 0;
		bValidString = TRUE;

		m_pMod->LogInst(PB_LOG_INFO, lpszPAN, L"ManualPANEntry", __LINE__); 

		lpszPANtmp = lpszPAN;
		if (lpszPANtmp!=NULL)
		{
		    while (*lpszPANtmp!='\0')
		    {
			    iCharCount++;

                if (*lpszPANtmp<'0' || *lpszPANtmp>'9')
				bValidString = FALSE;

			    lpszPANtmp++;
		    };
		}
		else
			bValidString = FALSE;

		if (bValidString==FALSE || iCharCount!=16)
		{
			m_pMod->LogInst(PB_LOG_INFO, L"PAN string invalid.", L"ManualPANEntry", __LINE__); 
            return FALSE;
		}

		// Set the data up
		memset(m_CardReaderReadDataBuffer,0,MAX_CARD_DATA*sizeof(TCHAR));
		wsprintf(m_CardReaderReadDataBuffer,L"%s%s%c",L"%B",lpszPAN,L'^'); //add the delimiting chars	
		m_bKeyEntry = TRUE;
		m_lpCardReaderMsrFlush(m_hReader);
	}
	else
	{
		m_pMod->LogInst(PB_LOG_WARNING, L"Debit Card Reader functionality not enabled.", L"ManualPANEntry", __LINE__); 
	    return FALSE;
	}

	return TRUE;
}	


////////////////////////////////////////////////////////////////////////
// Function:	SetPinTimeout
// Description:	Converts the number string into a value of milliseconds
//				and sets the PIN entry timeout to this value
//
// Scope:		Public
//
// Info:		Called by Meta.cpp
//				
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetPinTimeout(LPCTSTR lpMilliStr)
{
int value;

	value = _wtoi(lpMilliStr);

	if (value<0 || value > 65535)
	{
	    m_pMod->LogInst(PB_LOG_INFO, L"Pin timeout invalid.", L"SetPinTimeout", __LINE__); 
		return FALSE;
	}
	else
	{
		if (value == 65535)
		{
			m_iWait_Time = (UHWORD) INFINITE;//the API uses the value 0xFFFF to set an infinite timeout	
		}
		else
		{
			m_iWait_Time = (UHWORD) value;
		}
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetPinEntry
// Description:	turns on/off PIN entry 
//
// Scope:		Private
//
// Info:		Called by Meta.cpp
// Info:		In the some countries, the customer is asked whether 
//				their card is PIN entry or not.
//				This method sets the mode accordingly	 				
// 	
//				
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetPinEntry(LPCTSTR lpszSW)
{
	if(!_wcsicmp(lpszSW,PIN_ENTRY_ON))
	{
		m_bPinEntry = TRUE;
		return TRUE;
	}
	else if(!_wcsicmp(lpszSW,PIN_ENTRY_OFF))
	{
		m_bPinEntry = FALSE;
		return TRUE;
	}
	else
	{
		m_pMod->LogInst(PB_LOG_INFO, L"Invalid PIN entry setting.", L"SetPinEntry", __LINE__);
		return FALSE;
	}
}


////////////////////////////////////////////////////////////////////////
// Function:	SetDCR
// Description:	sets m_bDCR flag 
//
// Scope:		Private
// 	
//				
// Author:		Mike Schuette
// Change History:
//				Mar 2010 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::SetDCR(BOOL bValue)
{
	m_bDCR = bValue;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetCardReaderModule
// Description:	Sets the m_lpszCardReaderModule variable which holds the 
//				name of the MSR/DCR device to be used in the loading of 
//				the proper driver.
//
// Scope:		Private
// 	
//				
// Author:		Mike Schuette
// Change History:
//				Mar 2010 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetDriverName(LPCTSTR lpszModuleName)
{
	if (!m_bCardReaderHasBeenOpened)
	{
		if (lpszModuleName!=NULL) {
			wcscpy(m_lpszDeviceName, lpszModuleName);

			if (!_wcsicmp(lpszModuleName, L"MSR9500")) {
				wcscpy(m_lpszDriverName, L"MSRMC50.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"MSR9000")) {
				wcscpy(m_lpszDriverName, L"MSR3000.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"MSR9001")) {
				wcscpy(m_lpszDriverName, L"MSR3000.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"MSRCAMEO")) {
				wcscpy(m_lpszDriverName, L"MSRCAMEO.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"MSR7000")) {
				wcscpy(m_lpszDriverName, L"MSRMC50.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"DCR7000")) {
				wcscpy(m_lpszDriverName, L"MSRMC50.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"MSR55")) {
				wcscpy(m_lpszDriverName, L"MSRMC55.DLL");
			}
			else if (!_wcsicmp(lpszModuleName, L"MSR3000")) {
				wcscpy(m_lpszDriverName, L"MSR3000.DLL");
			}
			else {
				wcscpy(m_lpszDeviceName, L"");
				m_pMod->LogInst(PB_LOG_WARNING, L"Invalid card reader module name", L"SetDriverName", __LINE__);
				return FALSE;
			}
		}
	}
	else
	{
		m_pMod->LogInst(PB_LOG_WARNING, L"Attempt to change card reader module name while card reader is still open", L"SetDriverName", __LINE__);
		return FALSE;
	}

	return TRUE;
}
