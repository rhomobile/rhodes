#include "CardReader.h"


////////////////////////////////////////////////////////////////////////
// Function:	CCardReader
// Description:	CardReader-specific actions to be taken upon "Preloading" of the device,
//				essentially initializing the CardReader object, loading
//				the card reader DLL and setting object's method pointers to the DLL proper functions
//
// Scope:		Public
//								
// Info:		Called by cardreader_imp.cpp
//
// Return:		TRUE if successful at opening the card reader DLL
//				FALSE otherwise
// 	
//				
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
CCardReader::CCardReader(int iID):
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
	m_pCSIP(NULL),

	m_bCardReaderHasBeenOpened(FALSE),
	m_bDllPresent(FALSE),

	m_hReader(INVALID_HANDLE_VALUE),
	m_hThreadRead(INVALID_HANDLE_VALUE),
	m_hStopEvent(INVALID_HANDLE_VALUE),
	m_hKeypadMutex(INVALID_HANDLE_VALUE),
	m_iCardReaderCount(0),
	m_bKeyEntry(FALSE),
	m_iWait_Time(DEFAULT_PIN_ENTRY_TIMEOUT),
	m_bPinEntry(FALSE),
	m_bAutoTab(FALSE),
	m_bAutoEnter(FALSE),
	m_pSwipeEvent(NULL),
	m_iInstId(iID),
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
			LOG(ERROR) +  L"ERROR: Could not find the CardReader functions - Unloading the CardReader."; 
		}
		else
		{
			LOG(INFO) +  L"Loaded the CardReader DLL", L"CCardReader";
			m_bDllPresent = TRUE;
		}
	}
	else
	{
		LOG(ERROR) +  L"Error: Could not load the CardReader DLL";		
		return;
	}

	m_pCSIP = new CSIP();

	if (m_pCSIP == NULL)
		LOG(WARNING) +  L"Failed to allocate memory for CSIP", L"CCardReader";
	
	wcscpy(m_lpszDriverName, L"");
	wcscpy(m_lpszDeviceName, L"");
	wcscpy(m_lpszPanData, L"");

	LOG(INFO) +  L"Initialized the CardReader object", L"CCardReader";
	return;
}

////////////////////////////////////////////////////////////////////////
// Function:	GetStopEventHandle
// Description:	Get the value of the attribute m_hStopEvent
//
// Scope:		Public
//
// Return:		value of the attribute m_hStopEvent
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
HANDLE CCardReader::GetStopEventHandle()
{
	return m_hStopEvent;
}

////////////////////////////////////////////////////////////////////////
// Function:	GetKeypadMutexHandle
// Description:	Get the value of the attribute m_hKeypadMutex
//
// Scope:		Public
//
// Return:		value of the attribute m_hKeypadMutex
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
HANDLE CCardReader::GetKeypadMutexHandle()
{
	return m_hKeypadMutex;
}

////////////////////////////////////////////////////////////////////////
// Function:	SetStopEventHandle
// Description:	Set the value of the attribute m_hStopEvent
//
// Scope:		Public
//
// Return:		value of the attribute m_hStopEvent
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::SetStopEventHandle(HANDLE hHandle)
{
	m_hStopEvent = hHandle;
}

////////////////////////////////////////////////////////////////////////
// Function:	SetKeypadMutexHandle
// Description:	Set the value of the attribute m_hKeypadMutex
//
// Scope:		Public
//
// Return:		value of the attribute m_hKeypadMutex
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::SetKeypadMutexHandle(HANDLE hHandle)
{
	m_hKeypadMutex = hHandle;
}

////////////////////////////////////////////////////////////////////////
// Function:	~CCardReader
// Description:	CardReader-specific actions to be taken upon "Disposing" of the device,
//				including stopping the read thread, deinitializing the object 
//				referring to the CardReader object, and unloading the CardReader DLL.
//
// Scope:		Public
//								
// Info:		Called by cardreader_imp.cpp
// 
// Return:		TRUE always
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
CCardReader::~CCardReader()
{
	LOG(INFO) +  L"Destroying the CardReader object", L"~CCardReader";

	// No more stop event
	if (m_hStopEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hStopEvent);
		m_hStopEvent = INVALID_HANDLE_VALUE;
	}

	// No more keypad mutex
	if (m_hKeypadMutex != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hKeypadMutex);
		m_hKeypadMutex = INVALID_HANDLE_VALUE;
	}

	if (m_pCSIP)
	{
		delete m_pCSIP;
		m_pCSIP = NULL;
	}

	StopTheCardReader(NO_FOCUS_SWITCH); 
	UnloadTheCardReader();
}

BOOL CCardReader::Initialise(bool bLaunchingAppHasFocus, const WCHAR* szID)
{
	if (m_hStopEvent == INVALID_HANDLE_VALUE)
		// Create an event so that main routine can signal CardReaderReadThread to exit
		m_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (m_hKeypadMutex == INVALID_HANDLE_VALUE)
		// Create a mutex to suspend a card reader close until the user's PIN has been entered
		m_hKeypadMutex = CreateMutex(NULL, FALSE, NULL);

	//by default close the reader during a page navigate
	m_bReaderStayOpen = FALSE;

	
	//Setting m_bAppHasFocus to bLaunchingAppHasFocus i.e. true, to enable the open functionality of cardreader in wince or wm device.
	//If this is set to false then at the launch of cardreader application in wince or wm device, it will not open the cardreader.
	m_bAppHasFocus = bLaunchingAppHasFocus;

	m_eResumeAction = NONE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Function:	CardReaderMessage
// Description: Under test conditions 
//				(PLUGINTESTER_DEBUG defined) prints this data to the 
//				console in a message box
//
// Scope:		Public
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::CardReaderMessage(int iID, LPTSTR lpszBuffer, LPTSTR lpszMessageType)
{
#ifdef PLUGINTESTER_DEBUG
		//MessageBox(m_hWnd, (LPCWSTR) lpszBuffer, L"DEBUG MESSAGE", MB_OK);
#else
	LOG(INFO) +  L"Entering CardReaderMessage", L"CardReaderMessage";
	//If nav string is null and this is a CARDREADER_READ message, then we do sendkeys
	//if (cmp(lpszMessageType, CARDREADER_READ) && cmp(m_lpszCardReaderNavString, L""))
	//if (m_lpszCardReaderNavString[0] == '\0')
	if (m_pSwipeEvent == NULL)
	{
	    LOG(INFO) +  L"Send out like scan wedge", L"CardReaderMessage";
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
	    LOG(INFO) +  L"Call SendPBNavigate";				
		rho::Hashtable<rho::String, rho::String> cardreaderData;
		cardreaderData.put( rho::common::convertToStringA(L"data"), 
			rho::common::convertToStringA(lpszBuffer) );
		cardreaderData.put( rho::common::convertToStringA(L"mode"), 
			rho::common::convertToStringA(lpszMessageType) );			
		m_pSwipeEvent->set(cardreaderData);
		LOG(INFO) +  L"Sent swiped card data";		
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
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
		LOG(WARNING) +  L"No card reader drivers found on this device, please install drivers";
		return iCardReaderCount;
	}
	else
	{
		wsprintf(tcLogText, L"Found Card Reader Driver: %s (%s)", m_CardReaderFindInfo[0].szFriendlyName, m_CardReaderFindInfo[0].szDeviceName);
		LOG(INFO) +  tcLogText;
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
			LOG(INFO) +  tcLogText;
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
DWORD CCardReader::OpenTheCardReader(BOOL bFocusSwitch)
{
	int i = 0;
	HANDLE hDummy = NULL;
	TCHAR lpszInfo[100];
	DWORD Status = MSR_ERR_API_NOTSUPPORTED;

	LOG(INFO) +  L"Entering OpenTheCardReader"; 
	// On a focus switch, OpenTheCardReader is called only when card reader MUST be reopened
	// m_iCardReaderFindIndex has already been determined by a prior opening of the device
	if (bFocusSwitch)
	{
		Status = m_lpCardReaderOpen(m_CardReaderFindInfo[m_iCardReaderFindIndex].szDeviceName, m_CardReaderFindInfo[m_iCardReaderFindIndex].szPortName, &hDummy);
		if(MSR_SUCCESS == Status)
			m_hReader = hDummy;

		wsprintf(lpszInfo, L"Attempted to open cardreader on focus switch - status #%.16d", Status);
		LOG(INFO) + lpszInfo;
	}
	else
	{
		if (!m_bCardReaderHasBeenOpened)
		{
			m_iCardReaderCount = EnumerateCardReaders();
			wsprintf(lpszInfo, L"Number of Card Readers on Device: %i", m_iCardReaderCount);
			LOG(INFO) +  lpszInfo;

			// Open the card reader
			// We look for
			// the presence of the appropriate driver and then open the card reader.
			if (m_iCardReaderCount == 0)
			{
				LOG(WARNING) +  L"Not opening card reader as no drivers are installed on this device.";
			}
			else if (m_iCardReaderCount > 1)
			{
				for(i=0; i<m_iCardReaderCount; i++) 
				{
					if (!_wcsicmp(m_lpszDriverName, m_CardReaderFindInfo[i].szDeviceName)) 
					{
						wsprintf(lpszInfo, L"Opening Card Reader (User Specified): %s", m_CardReaderFindInfo[i].szDeviceName);
						LOG(INFO) +  lpszInfo;	
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
						LOG(WARNING) +  lpszInfo;	
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
				LOG(INFO) +  lpszInfo;	
				Status = m_lpCardReaderOpen(m_CardReaderFindInfo[0].szDeviceName, m_CardReaderFindInfo[0].szPortName, &hDummy);
			}

			if ( Status == MSR_SUCCESS )
			{
				// Indicate we are now open
				m_bCardReaderHasBeenOpened = TRUE;
				m_iCardReaderFindIndex = i;
				m_hReader = hDummy;
				LOG(INFO) +  L"Card Reader successfully opened";	

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


				LOG(INFO) +  L"CardReader opened";
			}
			else
			{
				m_bDCR = FALSE;
				LOG(ERROR) +  L"Card Reader open failed";	
			}
		}
		else
		{
			// Card reader already opened, so opening is an automatic success
			Status = MSR_SUCCESS;
			LOG(INFO) +  L"Card reader already opened"; 
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
		LOG(ERROR) +  L"Unable to open driver for CardReader device, is it attached?" + Status; 
	}

	return Status;
}


////////////////////////////////////////////////////////////////////////
// Function:	CardReaderReadThread	
//
// Scope:		Public					
//
// Return: 		0 if thread to terminate normally
//				1 if error in reading CardReader read buffer
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
DWORD WINAPI CCardReader::CardReaderReadThread (LPVOID param)
{
	TCHAR lpszLocalBuffer[MAX_CARD_DATA];
	DWORD dwReadStatus = 1;
	TCHAR lpszSendData[MAX_CARD_DATA];
	TCHAR lpszPANdata[50];

	CCardReader* pObj = (CCardReader*) param;
	LOG(INFO) +  L"Started CardReader Read Thread";

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
		if(WaitForSingleObject(pObj->GetStopEventHandle(),0) == WAIT_OBJECT_0)
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
				//pObj->LOG(WARNING) +  lpszLocalBuffer;
				pObj->CardReaderMessage(pObj->m_iInstId, lpszLocalBuffer, L"CARDREADER_MSG");
				return 1;
			}

			//pObj->LOG(INFO) +  L"Continuing after reset";
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
DWORD CCardReader::ResetTheCardReader()
{
	DWORD dwStatus;

	LOG(INFO) +  L"Resetting the CardReader";

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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::StopTheCardReader(BOOL bFocusSwitch) 
{
	HANDLE hStopEvent;

	LOG(INFO) +  L"Stopping the Card Reader";
	hStopEvent = GetStopEventHandle();

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
			LOG(WARNING) +  L"Read thread not responding - terminated";
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
	LOG(INFO) +  L"Cardreader stopped";
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
DWORD CCardReader::CloseTheCardReader(BOOL bFocusSwitch) 
{
	DWORD Status = MSR_SUCCESS;
	HANDLE hKeypadMutex;

	LOG(INFO) +  L"Closing the CardReader"; 

	// If the CardReader is open
	if ( m_bCardReaderHasBeenOpened )
	{
		//Grab mutex to make sure we are not closing card reader while waiting for keypad input
		hKeypadMutex =GetKeypadMutexHandle();
		WaitForSingleObject(hKeypadMutex, INFINITE);
		// Close it
		Status = m_lpCardReaderClose(m_hReader);

		if (!ReleaseMutex(hKeypadMutex))
			LOG(WARNING) +  L"Error: Can't release the keypad mutex 1"; 

		// Don't set m_hReader to INVALID_HANDLE_VALUE yet as CardReaderReadThread may still be active
		// Instead, reset it in the code following a call to CloseTheCardReader

		// If we got an error
		if (( Status != MSR_SUCCESS ))
		{
			// Report it
			LOG(WARNING) +  L"Error: Can't close the CardReader"; 
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::UnloadTheCardReader()
{
	LOG(INFO) +  L"Unloading the CardReader"; 

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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::GetCardReaderHasBeenOpened()
{
	return m_bCardReaderHasBeenOpened;
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetAutoTab(LPCTSTR lpszValue)
{
	if (!_wcsicmp(lpszValue, L"false"))  //  RegEx of 'Disabled'
		m_bAutoTab = FALSE;
	else if (!_wcsicmp(lpszValue, L"true"))  //  RegEx of 'Enabled'
		m_bAutoTab = TRUE;
	else
	{
		LOG(INFO) +  L"Request to set Autotab to invalid value";
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetAutoEnter(LPCTSTR lpszValue)
{
	if (!_wcsicmp(lpszValue, L"false"))	//  RegEx of 'Disabled'
		m_bAutoEnter = FALSE;
	else if (!_wcsicmp(lpszValue, L"true"))	//  RegEx of 'Enabled'
		m_bAutoEnter = TRUE;
	else
	{
		LOG(INFO) +  L"Request to set Autoenter to invalid value";
        return FALSE;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Function:	GetAutoTab
// Description:	Get the value of the attribute bAutoTab
//
// Scope:		Public
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::GetAutoTab(LPTSTR lpszValue)
{
	if (m_bAutoTab)
	{
		wcscpy(lpszValue, L"true");
	}
	else
	{
		wcscpy(lpszValue, L"false");
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Function:	GetAutoEnter
// Description:	Get the value of the attribute m_bAutoEnter
//
// Scope:		Public
//
//
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::GetAutoEnter(LPTSTR lpszValue)
{
	if (m_bAutoEnter)
	{
		wcscpy(lpszValue, L"true");
	}
	else
	{
		wcscpy(lpszValue, L"false");
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
DWORD  CCardReader::PinTransactionDebitCardReader(LPCTSTR lpszPANData)
{
	UBYTE ubEncryptedPinBuffer[DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE + 1];
	DWORD dwReadStatus;
	DWORD iErr = 0;
	HANDLE hKeypadMutex;
	
	TCHAR byteToHexString[]={L"0123456789ABCDEF"};
	TCHAR lpszHexVal[DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE*2 + 1];	

	// Grab mutex to prevent closing of card reader while we wait for keypad input
	hKeypadMutex = GetKeypadMutexHandle();
	WaitForSingleObject(hKeypadMutex, INFINITE);

	if(m_hReader != INVALID_HANDLE_VALUE)
	{
		memset(ubEncryptedPinBuffer,0,sizeof(UBYTE) * (DUKPT_ENCRYPTED_KSNECAndEPB_MAX_SIZE) + 1);

		// Have DebitCardReader encrypt PAN data and put in ubEncryptedPinBuffer
		dwReadStatus = m_lpCardReaderGetPinBlock(m_hReader,lpszPANData, 0, m_iWait_Time, ubEncryptedPinBuffer);

		if (!ReleaseMutex(hKeypadMutex))
			LOG(WARNING) +  L"Error: Can't release the keypad mutex 2"; 

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
			LOG(WARNING) +  L"Error: Can't release the keypad mutex 3"; 

		iErr =  INVALIDREADERHANDLE;
	}

	return iErr;
}

////////////////////////////////////////////////////////////////////////
// Function:	ManualPANEntry
//
// Scope:		Private
//
// Info			Called by Meta.cpp
//				
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
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

		LOG(INFO) +  lpszPAN; 
		
		lpszPANtmp = lpszPAN;
		if (lpszPANtmp!=NULL)
		{
			wcscpy(m_lpszPanData,lpszPAN);
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
			LOG(INFO) +  L"PAN string invalid."; 
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
		LOG(WARNING) +  L"Debit Card Reader functionality not enabled."; 		
	    return FALSE;
	}

	return TRUE;
}	


////////////////////////////////////////////////////////////////////////
// Function:	ManualPANEntry
//
// Scope:		Private
//
// Info			Called by Meta.cpp
//				
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::GetManualPANEntry(LPTSTR lpszPAN)
{
	if (m_lpszPanData)
	{		
		wcscpy(lpszPAN, m_lpszPanData);
		return TRUE;
	}
	else
	{
		wcscpy(lpszPAN, L"Unavailable");
		return FALSE;
	}	
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::SetPinTimeout(LPCTSTR lpMilliStr)
{
	int value;

	value = _wtoi(lpMilliStr);

	if (value<0 || value > 65535)
	{
	    LOG(INFO) +  L"Pin timeout invalid."; 
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
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
		LOG(INFO) +  L"Invalid PIN entry setting.";
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2010 - Created
////////////////////////////////////////////////////////////////////////
void CCardReader::SetDCR(BOOL bValue)
{
	m_bDCR = bValue;
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
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::GetPinEntry(LPTSTR lpszSW)
{
	if (m_bPinEntry == TRUE)
	{
		wcscpy(lpszSW, L"true");		
	}
	else if (m_bPinEntry == FALSE)
	{
		wcscpy(lpszSW, L"false");		
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Function:	GetPinTimeout
// Description:	Converts the number string into a value of milliseconds
//				and sets the PIN entry timeout to this value
//
// Scope:		Public
//
// Info:		Called by Meta.cpp
//				
// Author:		Harish Pulimi
// Change History:
//				Mar 2013 - Created
////////////////////////////////////////////////////////////////////////
int CCardReader::GetPinTimeout(LPTSTR lpMilliStr)
{	
	_itow(m_iWait_Time, lpMilliStr, 10);
	if (lpMilliStr)
	{
		return TRUE;
	}
	return FALSE;
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
// Author:		Harish Pulimi
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
				LOG(WARNING) +  L"Invalid card reader module name";
				return FALSE;
			}
		}
	}
	else
	{
		LOG(WARNING) +  L"Attempt to change card reader module name while card reader is still open";
		return FALSE;
	}

	return TRUE;
}

BOOL CCardReader::GetDriverName(LPTSTR lpszModuleName)
{
	if (m_lpszDriverName)
	{		
		wcscpy(lpszModuleName, m_lpszDriverName);
		return TRUE;
	}
	else
	{
		wcscpy(lpszModuleName, L"Unavailable");
		return FALSE;
	}
}

////////////////////////////////////////////////////////////////////////
/// Function:		cmp
/// Description:	compares to TCHARs - case insensitive
/// Returns:		TRUE if successful
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;
	return !_wcsicmp(tc1, tc2);

}

BOOL CCardReader::BeforeNavigate()
{
	if(!m_bReaderStayOpen)
	{
		// If focused app had opened the device, stop it and close it 
		if (GetCardReaderHasBeenOpened())
			StopTheCardReader(NO_FOCUS_SWITCH);

		
		// Set all parameters to their default values
		SetAutoTab(L"false");
		SetAutoEnter(L"false");
		SetPinTimeout(DEFAULT_PIN_ENTRY_TIMEOUT_S);
		SetPinEntry(PIN_ENTRY_OFF);
		SetDCR(FALSE);
	}
	// Set nav string to empty string
	SetCallback(NULL);
	return TRUE;
}

BOOL CCardReader::DocumentComplete()
{	
	if(!m_bReaderStayOpen)
	{
		// If focused app had opened the device, stop it and close it 
		if (GetCardReaderHasBeenOpened())
			StopTheCardReader(NO_FOCUS_SWITCH);

		
		// Set all parameters to their default values
		SetAutoTab(L"false");
		SetAutoEnter(L"false");
		SetPinTimeout(DEFAULT_PIN_ENTRY_TIMEOUT_S);
		SetPinEntry(PIN_ENTRY_OFF);
		SetDCR(FALSE);
	}
	// Set nav string to empty string
	SetCallback(NULL);
	return TRUE;
}

BOOL CCardReader::ClearAllProperties()
{
	SetAutoTab(L"false");
	SetAutoEnter(L"false");
	SetPinTimeout(DEFAULT_PIN_ENTRY_TIMEOUT_S);
	SetPinEntry(PIN_ENTRY_OFF);
	SetDCR(FALSE);
	SetCallback(NULL);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
/// Function:		isInt
/// Description:	Method that checks if a string contains any non digit characters. 
///
/// Returns:		TRUE if there are no non-digit characters in the string. 
/// Author:			Dinka Ranns
/// Date:			July 2011
////////////////////////////////////////////////////////////////////////
BOOL CCardReader::isFloat (const wchar_t * str)
{
	const wchar_t *temp = str;
	bool hasPoint = false;
	int num_digit=0;

	if (temp == NULL) return false;

		
	if (((*temp)=='+')|| ((*temp)=='-')) temp++;

	while (*temp)
	{
		if (!isdigit(*temp)) 
		{
			if ((*temp =='.') && (!hasPoint))
			{
				hasPoint = true;
			}
			else 
			{
				return false;
			}
		}
		else 
		{
			++num_digit;
		}

		++temp;

	}
	if (num_digit >0 ) return true;
	else return false;
}

BOOL CCardReader::SetPropertyOrMethod(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	DWORD Status;

	// start the parsing of the Meta tag info
	if(cmp(szPropertyName, L"close"))
	{
		//shut down the card reader, terminate all signals/threads associated with it
		StopTheCardReader(NO_FOCUS_SWITCH);
		SetCallback(NULL);
		m_eResumeAction = NONE;
		return TRUE;
	}
	else if(cmp(szPropertyName, L"open"))
	{
		if (!m_bAppHasFocus)
		{
			if (m_eResumeAction == NONE)
				m_eResumeAction = ACTIVATE;
			return TRUE;
		}

		//open the card reader, create signals/threads to handle processing of card reader data
		Status = OpenTheCardReader(FALSE);
		
		if (Status != MSR_SUCCESS)
			return FALSE;

		if (GetThreadReadHandle() == INVALID_HANDLE_VALUE)
			// Create a thread to read the card swipe data.
			SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CardReaderReadThread, this, 0, NULL));
	}
	else if(cmp(szPropertyName, L"pintimeout"))
	{
		if (isFloat(szPropertyValue))
			return(SetPinTimeout(szPropertyValue));
		else
			return FALSE;
	}
	else if(cmp(szPropertyName, L"pinentry"))
	{
		return(SetPinEntry(szPropertyValue));
	}
	else if(cmp(szPropertyName, L"pandata"))
	{
		return(ManualPANEntry(szPropertyValue));
	}
	else if(cmp(szPropertyName, L"autotab"))
	{
		return(SetAutoTab(szPropertyValue));
	}
	else if(cmp(szPropertyName, L"autoenter"))
	{
		return(SetAutoEnter(szPropertyValue));
	}
	else if(cmp(szPropertyName, L"modulename"))
	{
		return(SetDriverName(szPropertyValue));
	}	
	return FALSE;
}

//  Call this to retrieve the value of an EMML Tag
BOOL CCardReader::RetrieveProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)
{
	if(cmp(szParameterName, L"pintimeout"))
	{
		return(GetPinTimeout(szParameterValue));
	}
	else if(cmp(szParameterName, L"pinentry"))
	{
		return(GetPinEntry(szParameterValue));
	}
	else if(cmp(szParameterName, L"pandata"))
	{		
		return(GetManualPANEntry(szParameterValue));
	}
	else if(cmp(szParameterName, L"autotab"))
	{
		return(GetAutoTab(szParameterValue));
	}
	else if(cmp(szParameterName, L"autoenter"))
	{
		return(GetAutoEnter(szParameterValue));
	}
	else if(cmp(szParameterName, L"modulename"))
	{
		return(GetDriverName(szParameterValue));
	}
	else
	{		
		return FALSE;
	}
}

void CCardReader::SetCallback(rho::apiGenerator::CMethodResult* pCallback)
{	
	m_pSwipeEvent = pCallback;	
}

BOOL CCardReader::ApplicationFocusChange(bool bActivated)
{
	//  An Application focus change is received when this card reader instance either
	//  goes to the background (bActivated False) or comes to the foreground (bActivated True).	

	if (m_bAppHasFocus && !bActivated)
	{
		// we've just lost focus
		m_eResumeAction = NONE;
		if (GetCardReaderHasBeenOpened())
			m_eResumeAction = RESUME;
		if (GetCardReaderHasBeenOpened())
		{
			DWORD Status = OpenTheCardReader(FOCUS_SWITCH);
			
			if (Status != MSR_SUCCESS)
					return FALSE;

			if (GetThreadReadHandle() == INVALID_HANDLE_VALUE)
				// Create a thread to read the card swipe data.
				SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CardReaderReadThread, this, 0, NULL));
		}
	}
	else if (!m_bAppHasFocus && bActivated)
	{
		// we've just regained focus
		if (m_eResumeAction == RESUME)
		{
			if (GetCardReaderHasBeenOpened())
			{
				DWORD Status = OpenTheCardReader(FOCUS_SWITCH);
				
				if (Status != MSR_SUCCESS)
						return FALSE;

				if (GetThreadReadHandle() == INVALID_HANDLE_VALUE)
					// Create a thread to read the card swipe data.
					SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CardReaderReadThread, this, 0, NULL));
			}
		}
		else if (m_eResumeAction == ACTIVATE)
		{
			OpenTheCardReader(FALSE);
			SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CardReaderReadThread, this, 0, NULL));
		}
	}
	m_bAppHasFocus = bActivated;
	return TRUE;
}