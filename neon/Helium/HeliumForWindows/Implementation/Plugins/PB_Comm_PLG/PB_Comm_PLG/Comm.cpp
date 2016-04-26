//////////////////////////////////////////////////////////////////////////////
// Authors:		Phil May (MAY)
//
// Change History:
//		July      2009 - MAY First Created
//		August    2009 - MAY Bug fixes and added logging
//		September 2009 - MAY Multi-instance support
//		October   2009 - MAY Fixes based on code review
//		Novenber  2009 - MAY Removed the page cancel thread
//////////////////////////////////////////////////////////////////////////////

#include "Comm.h"

// needed for wmemset()
#include <wchar.h>

#ifndef	LF
#define	LF	(0x0a)
#endif

#ifndef	CR
#define	CR	(0x0d)    
#endif

// storage for log message strings
#define ERROR_MESSAGE_LENGTH	200
TCHAR tcErrorMessage[ERROR_MESSAGE_LENGTH];


/*****************************************************************************/
CComm::CComm(PBStruct *pPBStructure, CCommModule *pWrapperClass) :
	m_iTimeout(0),
	m_iTempTimeout(0),
	m_cEndChar('\0'),
	m_cTempEndChar('\0'),
	m_iCharCount(0),
	m_iTempCharCount(0),
	m_bCRLF(FALSE),
	m_bTempCRLF(FALSE),
	m_hPort(INVALID_HANDLE_VALUE),
	m_hPortReadThread(INVALID_HANDLE_VALUE),
	m_hPortWriteThread(INVALID_HANDLE_VALUE),
	m_hUserTimeoutThread(INVALID_HANDLE_VALUE),
	m_hCancelPortWrite(INVALID_HANDLE_VALUE),
	m_hCancelUserTimeout(INVALID_HANDLE_VALUE),
	m_hRestartUserTimeout(INVALID_HANDLE_VALUE),
	m_hWriteDataAvailable(INVALID_HANDLE_VALUE),
	m_bSawCR(FALSE),
	m_iDataBufferIndex(0),
	m_hDataBufferMutex(INVALID_HANDLE_VALUE),
	m_pWriteQueueHead(NULL),
	m_pWriteQueueTail(NULL),
	m_hWriteQueueMutex(INVALID_HANDLE_VALUE),
	m_bAutoEnter(FALSE),
	m_bAutoTab(FALSE)
{	
	m_pPBStructure = pPBStructure;

	// set pointer to wrapper-class that instantiated this instance
	m_pWrapperClass = pWrapperClass;

	// clear members that are strings
	m_tcNavigateUrl[0] = NULL;
	m_tcPort[0] = NULL;
	m_cDataBuffer[0] = NULL;

	// default port is COM1
	_tcscpy(m_tcPort, L"COM1:");

	// clear temp inputOn state
	ResetTempCommInputOn();
	// default inputOn = CRLF
	m_bTempCRLF = TRUE;

	// initialize windows serial structs
	ZeroMemory(&m_DCB, sizeof(m_DCB));
	m_DCB.DCBlength = sizeof(m_DCB);
	m_DCB.fBinary = TRUE;

	// If hard-coded defaults are desired, do them here
	// default baud rate = 9600
	m_DCB.BaudRate = CBR_9600;
	// default handshake = none
	m_DCB.fOutxCtsFlow = FALSE;
	m_DCB.fOutxDsrFlow = FALSE;
	m_DCB.fInX = FALSE;
	// default data bits = 8
	m_DCB.ByteSize = 8;
	// default parity = none
	m_DCB.Parity = NOPARITY;
	// default stop bits = 1
	m_DCB.StopBits = ONESTOPBIT;

	// initialize comm port timeouts
	m_CommTimeouts.ReadIntervalTimeout = 0;
	m_CommTimeouts.ReadTotalTimeoutConstant = 0;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	m_CommTimeouts.WriteTotalTimeoutConstant = 0;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 0;

	// set mutex that protects the navigate URL
	m_hNavigateUrlMutex = CreateMutex(NULL, FALSE, NULL);

	// instantiate the CSIP class for handling sendkeys
	m_pCSIP = new CSIP();
}

/*****************************************************************************/
CComm::~CComm(void)
{
	// close the comm port if it is open
	Close();
	delete m_pCSIP;
}

////////////////////
// Public methods //
////////////////////

/*****************************************************************************/
BOOL CComm::SetNavigateUrl(const TCHAR *url)
{
	BOOL	bUrlValid;

	// first, protect the URL, as this value can be changed while the port
	// is open, and the read thread may be using the current URL to navigate
	WaitForSingleObject(m_hNavigateUrlMutex, INFINITE);
	bUrlValid = (_tcscpy(m_tcNavigateUrl, url) != NULL && (url[0] != NULL));
	ReleaseMutex(m_hNavigateUrlMutex);

	if (bUrlValid)
		return TRUE;
	else
	{
		if (url[0] == NULL)
		{
			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
					                     L"Warning -- Set empty navigate string", 
										 L"CComm::SetNavigateUrl(const TCHAR *)", 
										 m_pPBStructure->iTabID,
										 0,
										 __LINE__);
			return TRUE;
		}
		else
		{
			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
					                     L"Error occurred -- Could not copy URI string to m_tcNavigateUrl", 
										 L"CComm::SetNavigateUrl(const TCHAR *)", 
										 m_pPBStructure->iTabID,
										 0,
										 __LINE__);
			return FALSE;
		}
	}

}

/*****************************************************************************/
BOOL CComm::SetAutoEnter(const BOOL bEnabled)
{
	m_bAutoEnter = bEnabled;
	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetAutoTab(const BOOL bEnabled)
{
	m_bAutoTab = bEnabled;
	return TRUE;
}

/*****************************************************************************/
PBStruct* CComm::GetPBStruct()
{
	return m_pPBStructure;
}

/*****************************************************************************/
BOOL CComm::SetTimeout(int iTimeout)
{
	// write to temp location for later application to the port
	m_iTempTimeout = iTimeout;
	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetEndChar(const char cEndChar)
{
	// write to temp location for later application to the port
	m_cTempEndChar = cEndChar;
	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetCRLF(const BOOL bCRLF)
{
	// write to temp location for later application to the port
	m_bTempCRLF = bCRLF;
	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetCharCount(int iCharCount)
{
	// only set the m_iCharCount variable if the iCharCount param is less than
	// the input buffer size
	if (iCharCount <= RECV_BUFFER_SIZE)
	{
		// write to temp location for later application to the port
		m_iTempCharCount = iCharCount;
		return TRUE;
	}
	else
	{
		m_bTempCRLF = TRUE;
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Requested character count (%d) is greater than input buffer size (%d), setting CRLF navigate",
		         iCharCount, 
				 RECV_BUFFER_SIZE);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::SetCharCount(int)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}
}

/*****************************************************************************/
BOOL CComm::ResetTempCommInputOn()
{
	// reset temp locations
	m_iTempTimeout = 0;
	m_cTempEndChar = NULL;
	m_bTempCRLF = FALSE;
	m_iTempCharCount = 0;
	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetHandshake(const TCHAR *tcHandshake)
{
	// The m_DCB structure contains the comm port settings 

	if(_tcsicmp(tcHandshake, L"NONE") == 0){
		m_DCB.fOutxCtsFlow = FALSE;
		m_DCB.fOutxDsrFlow = FALSE;
		m_DCB.fInX = FALSE;
	}
	else if(_tcsicmp(tcHandshake, L"HW") == 0){
		m_DCB.fOutxCtsFlow = TRUE;
		m_DCB.fOutxDsrFlow = TRUE;
		m_DCB.fInX = FALSE;
	}
	else if(_tcsicmp(tcHandshake, L"SW") == 0){
		m_DCB.fOutxCtsFlow = FALSE;
		m_DCB.fOutxDsrFlow = FALSE;
		m_DCB.fInX = TRUE;
	}
	else {
		m_DCB.fOutxCtsFlow = FALSE;
		m_DCB.fOutxDsrFlow = FALSE;
		m_DCB.fInX = FALSE;
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Handshake string (%s), is not recognized, setting NONE",
		         tcHandshake);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::SetHandshake(const TCHAR)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetParity(const TCHAR *tcParity)
{
	// The m_DCB structure contains the comm port settings 

	if(_tcsicmp(tcParity, L"Even") == 0)
		m_DCB.Parity = EVENPARITY;
	else if(_tcsicmp(tcParity, L"Mark") == 0)
		m_DCB.Parity  = MARKPARITY;
	else if(_tcsicmp(tcParity, L"No Parity") == 0)
		m_DCB.Parity  = NOPARITY;
	else if(_tcsicmp(tcParity, L"Odd") == 0)
		m_DCB.Parity  = ODDPARITY;
	else if(_tcsicmp(tcParity, L"Space") == 0)
		m_DCB.Parity  = SPACEPARITY;
	else
	{
		m_DCB.Parity  = NOPARITY;	// no match, set default
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Parity string (%s), is not recognized, setting NOPARITY",
		         tcParity);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::SetParity(const TCHAR)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetDataBits(const TCHAR *tcDataBits)
{
	// The m_DCB structure contains the comm port settings 

	BYTE	cByteSize;

	// The allowable data word sizes are between 5 bits and 9 bits inclusive
	cByteSize = (BYTE)_tcstol(tcDataBits, NULL, 10);
	if (cByteSize >= 5 && cByteSize <= 9)
		m_DCB.ByteSize = cByteSize;
	else
	{
		m_DCB.ByteSize = 8;
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Requested data-bits value (%s) is out of range, setting 8 data bits",
		         tcDataBits);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::SetDataBits(const TCHAR)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetStopBits(const TCHAR *tcStopBits)
{
	// The m_DCB structure contains the comm port settings 

	if(_tcscmp(tcStopBits, L"1") == 0)
		m_DCB.StopBits = ONESTOPBIT;
	else if(_tcscmp(tcStopBits, L"1.5") == 0)
		m_DCB.StopBits = ONE5STOPBITS;
	else if(_tcscmp(tcStopBits, L"2") == 0)
		m_DCB.StopBits = TWOSTOPBITS;
	else
	{
		m_DCB.StopBits = ONESTOPBIT;	// no match, use default
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Stop-bits string (%s), is not recognized, setting ONESTOPBIT",
		         tcStopBits);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::SetStopBits(const TCHAR)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetBaudRate(const TCHAR *tcBaudRate)
{
	// The m_DCB structure contains the comm port settings 

	if(_tcscmp(tcBaudRate, L"110") == 0)
		m_DCB.BaudRate = CBR_110;
	else if(_tcscmp(tcBaudRate, L"300") == 0)
		m_DCB.BaudRate = CBR_300;
	else if(_tcscmp(tcBaudRate, L"600") == 0)
		m_DCB.BaudRate = CBR_600;
	else if(_tcscmp(tcBaudRate, L"1200") == 0)
		m_DCB.BaudRate = CBR_1200;
	else if(_tcscmp(tcBaudRate, L"2400") == 0)
		m_DCB.BaudRate = CBR_2400;
	else if(_tcscmp(tcBaudRate, L"4800") == 0)
		m_DCB.BaudRate = CBR_4800;
	else if(_tcscmp(tcBaudRate, L"9600") == 0)
		m_DCB.BaudRate = CBR_9600;
	else if(_tcscmp(tcBaudRate, L"14400") == 0)
		m_DCB.BaudRate = CBR_14400;
	else if(_tcscmp(tcBaudRate, L"19200") == 0)
		m_DCB.BaudRate = CBR_19200;
	else if(_tcscmp(tcBaudRate, L"38400") == 0)
		m_DCB.BaudRate = CBR_38400;
	else if(_tcscmp(tcBaudRate, L"56000") == 0)
		m_DCB.BaudRate = CBR_56000; 
	else if(_tcscmp(tcBaudRate, L"57600") == 0)
		m_DCB.BaudRate = CBR_57600; 
	else if(_tcscmp(tcBaudRate, L"115200") == 0)
		m_DCB.BaudRate = CBR_115200;
	else if(_tcscmp(tcBaudRate, L"128000") == 0)
		m_DCB.BaudRate = CBR_128000;
	else if(_tcscmp(tcBaudRate, L"256000") == 0)
		m_DCB.BaudRate = CBR_256000; 
	else
	{
		m_DCB.BaudRate = CBR_9600;
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Baud-rate string (%s), is not recognized, setting CBR_9600",
		         tcBaudRate);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::SetBaudRate(const TCHAR)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::SetPort(const TCHAR *tcPort)
{
	wmemset(m_tcPort, 0, PORT_NAME_CHAR_COUNT + 1);
	wsprintf(m_tcPort, L"%s%s", tcPort, L":");
	return TRUE;
/*
	if (_tcsncpy(m_tcPort, tcPort, PORT_NAME_CHAR_COUNT) != NULL)
		return TRUE;
	else
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
									 L"Error occurred -- Could not copy port identifier string to m_tcPort", 
									 L"CComm::SetPort(const TCHAR *)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);		
		return FALSE;	// copy failed
	}
*/
}


/*****************************************************************************/
BOOL CComm::Open()
{
	// if port is already open, just return TRUE
	if (m_hPort != INVALID_HANDLE_VALUE)
		return TRUE;

	// open serial port if not already opened
	if (m_hPort == INVALID_HANDLE_VALUE)
	{
		m_hPort = CreateFile(m_tcPort, 
			                 GENERIC_READ | GENERIC_WRITE,
							 0,
							 NULL,
							 OPEN_EXISTING,
							 0,
							 NULL);
	}

	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		// configure the open port and set DTR and RTS
		SetCommState(m_hPort, &m_DCB);
		EscapeCommFunction(m_hPort, SETDTR);
		EscapeCommFunction(m_hPort, SETRTS);
		SetCommMask(m_hPort, EV_RXCHAR);

		// transfer temp inputOn state to working locations
		m_iTimeout = m_iTempTimeout;
		m_cEndChar = m_cTempEndChar;
		m_bCRLF = m_bTempCRLF;
		m_iCharCount = m_iTempCharCount;

		// setup signalling events

		// autoreset - only one thread will be waiting on these events
		// write thread waiting for data to write
		m_hWriteDataAvailable = CreateEvent(NULL, FALSE, FALSE, NULL);
		// write thread also waits for this event (port closing)
		m_hCancelPortWrite    = CreateEvent(NULL, FALSE, FALSE, NULL);

		// create mutex to protect write queue pointers, no initial ownership
		m_hWriteQueueMutex    = CreateMutex(NULL, FALSE, NULL);

		// launch listening thread
		m_hPortReadThread     = CreateThread(NULL, 0, PortReadThread, this, 0, NULL);
		// launch write thread
		m_hPortWriteThread    = CreateThread(NULL, 0, PortWriteThread, this, 0, NULL);

		// setup timeout infrastructure if app-specified timeout is non-zero
		if (m_iTimeout > 0)
		{
			// the timeout thread can mess with the input data array 
			// while the read thread writes it
			m_hDataBufferMutex    = CreateMutex(NULL, FALSE, NULL);
			// event for stopping the timeout thread - autoreset
			m_hCancelUserTimeout  = CreateEvent(NULL, FALSE, FALSE, NULL);
			// timeout restart event - autoreset
			m_hRestartUserTimeout = CreateEvent(NULL, FALSE, FALSE, NULL);

			// launch timeout thread
			m_hUserTimeoutThread  = CreateThread(NULL, 0, UserTimeoutThread, this, 0, NULL);
		}
		return TRUE;
	}
	else
	{
		// report trouble opening the port
		DWORD dwErrorCode = GetLastError();
		wsprintf(tcErrorMessage, 
		         L"Error Occurred -- Could not open %s.",
		         m_tcPort);
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     tcErrorMessage, 
									 L"CComm::Open(PBStruct *)", 
									 m_pPBStructure->iTabID,
									 dwErrorCode,
									 __LINE__);
		return FALSE;
	}
}

/*****************************************************************************/
BOOL CComm::PortWriteString(const TCHAR *tcWriteData)
{
	// Create a new write queue element and populate it for this type of write

	WRITEELEMENT	*queueElement = new WRITEELEMENT;

	if (queueElement == NULL)
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     L"Error occurred - Could not allocate memory for write queueElement", 
				                     L"CComm::PortWriteString(const TCHAR *)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	// the number of ASCII characters can be up to 2x the number of unicode
	int iWriteBufferSize = _tcslen(tcWriteData) * 2;
	queueElement->szWriteBuffer = new char[iWriteBufferSize + 1];
	
	if (queueElement->szWriteBuffer == NULL)
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     L"Error occurred - Could not allocate memory for write queueElement->szWriteBuffer", 
				                     L"CComm::PortWriteString(const TCHAR *)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	// convert input string to ASCII for writing to UART
	wcstombs(queueElement->szWriteBuffer, tcWriteData, iWriteBufferSize);
	queueElement->writeType = COMM_WRITE_STRING;
	queueElement->next = NULL;
	queueElement->tcWriteFileName[0] = NULL;

	// mutex since write thread also manipulates queue pointers
	WaitForSingleObject(m_hWriteQueueMutex, INFINITE);

	// insert new write at tail of queue
	if (m_pWriteQueueHead)
		m_pWriteQueueTail->next = queueElement;
	else
		m_pWriteQueueHead = queueElement;
	m_pWriteQueueTail = queueElement;

	ReleaseMutex(m_hWriteQueueMutex);

	// signal data is ready
	SetEvent(m_hWriteDataAvailable);

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::PortWriteBytes(const TCHAR *tcWriteData)
{
	// Create a new write queue element and populate it for this type of write

	WRITEELEMENT	*queueElement = new WRITEELEMENT;

	if (queueElement == NULL)
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     L"Error occurred - Could not allocate memory for write queueElement", 
				                     L"CComm::PortWriteBytes(const TCHAR *)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	// the number of ASCII characters can be up to 2x the number of unicode
	int iWriteBufferSize = _tcslen(tcWriteData) * 2;
	queueElement->szWriteBuffer = new char[iWriteBufferSize + 1];

	if (queueElement->szWriteBuffer == NULL)
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     L"Error occurred - Could not allocate memory for write queueElement->szWriteBuffer", 
				                     L"CComm::PortWriteBytes(const TCHAR *)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	// converting this string to ASCII, because it's handled by the same
	// routine that handles PortWriteStrint, which needs ASCII
	wcstombs(queueElement->szWriteBuffer, tcWriteData, iWriteBufferSize);
	queueElement->writeType = COMM_WRITE_BYTES;
	queueElement->next = NULL;
	queueElement->tcWriteFileName[0] = NULL;

	// mutex since write thread also manipulates queue pointers
	WaitForSingleObject(m_hWriteQueueMutex, INFINITE);

	// insert new write at tail of queue
	if (m_pWriteQueueHead)
		m_pWriteQueueTail->next = queueElement;
	else
		m_pWriteQueueHead = queueElement;
	m_pWriteQueueTail = queueElement;

	ReleaseMutex(m_hWriteQueueMutex);

	// signal data is ready
	SetEvent(m_hWriteDataAvailable);

	return TRUE;
}

/*****************************************************************************/
BOOL CComm::PortWriteFile(const TCHAR *tcWriteFileName)
{
	// Create a new write queue element and populate it for this type of write

	WRITEELEMENT	*queueElement = new WRITEELEMENT;

	if (queueElement == NULL)
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                     L"Error occurred - Could not allocate memory for write queueElement", 
				                     L"CComm::PortWriteFile(const TCHAR *)", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		return FALSE;
	}

	queueElement->szWriteBuffer = NULL;
	queueElement->writeType = COMM_WRITE_FILE;
	queueElement->next = NULL;
	wmemset(queueElement->tcWriteFileName, 0, FILE_NAME_CHAR_COUNT + 1);
	_tcsncpy(queueElement->tcWriteFileName, tcWriteFileName, FILE_NAME_CHAR_COUNT);

	// mutex since write thread also manipulates queue pointers
	WaitForSingleObject(m_hWriteQueueMutex, INFINITE);

	// insert new write at tail of queue
	if (m_pWriteQueueHead)
		m_pWriteQueueTail->next = queueElement;
	else
		m_pWriteQueueHead = queueElement;
	m_pWriteQueueTail = queueElement;

	ReleaseMutex(m_hWriteQueueMutex);

	// signal data is ready
	SetEvent(m_hWriteDataAvailable);

	return TRUE;
}

/*****************************************************************************/
void CComm::Close()
{
	// SIGNAL PORT CLOSING
	// signal to the write thread that we are closing
	if (m_hCancelPortWrite != INVALID_HANDLE_VALUE)
		SetEvent(m_hCancelPortWrite);

	// STOP TIMEOUT THREAD IF IT EXISTS and clean up associated member variables
	if (m_hUserTimeoutThread != INVALID_HANDLE_VALUE)
	{
		if (m_hRestartUserTimeout != INVALID_HANDLE_VALUE)
			CloseHandle(m_hRestartUserTimeout);
	
		// first, signal timeout to stop early
		if (m_hCancelUserTimeout != INVALID_HANDLE_VALUE)
			SetEvent(m_hCancelUserTimeout);

		// second, wait for the timeout thread to finish
		// if not back after 1 second, kill it
		if (WaitForSingleObject(m_hUserTimeoutThread, 1000) == WAIT_TIMEOUT)
		{
			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
				                         L"TimeoutThread not finishing.  Resorting to TerminateThread()", 
					                     L"CComm::Close(PBStruct *)", 
									     m_pPBStructure->iTabID,
										 0,
										 __LINE__);
			TerminateThread(m_hUserTimeoutThread, 0);
		}
		if (m_hUserTimeoutThread != INVALID_HANDLE_VALUE)
			CloseHandle(m_hUserTimeoutThread);

		// close the event resource
		if (m_hCancelUserTimeout != INVALID_HANDLE_VALUE)
			CloseHandle(m_hCancelUserTimeout);

		// invalidate timeout-related handles
		m_hRestartUserTimeout = INVALID_HANDLE_VALUE;
		m_hCancelUserTimeout =  INVALID_HANDLE_VALUE;
		m_hUserTimeoutThread =  INVALID_HANDLE_VALUE;
		m_hDataBufferMutex =    INVALID_HANDLE_VALUE;
	}

	// CLOSE PORT
	// closing port will unblock the read wait on the serial port
	if (m_hPort != INVALID_HANDLE_VALUE)
		CloseHandle(m_hPort);
	m_hPort = INVALID_HANDLE_VALUE;

	// STOP WRITE THREAD and clean up associated member variables
	if (m_hPortWriteThread != INVALID_HANDLE_VALUE)
	{
		// wait for write thread to stop
		// if not back after 1 second, kill it
		if (WaitForSingleObject(m_hPortWriteThread, 1000) == WAIT_TIMEOUT)
		{
			WRITEELEMENT	*pWriteElement;

			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
						                 L"PortWriteThread not finishing.  Resorting to TerminateThread()", 
							             L"CComm::Close(PBStruct *)", 
									     m_pPBStructure->iTabID,
										 0,
										 __LINE__);
			TerminateThread(m_hPortWriteThread, 0);

			// if the thread was terminated, the write queue will need cleanup
			while (m_pWriteQueueHead)
			{
				pWriteElement = m_pWriteQueueHead;
				m_pWriteQueueHead = m_pWriteQueueHead->next;
				if (pWriteElement->szWriteBuffer)
					delete[] pWriteElement->szWriteBuffer;
				delete pWriteElement;
			}
		}
		CloseHandle(m_hPortWriteThread);
		m_hPortWriteThread = INVALID_HANDLE_VALUE;
	}
	// eliminate the write-related events, 
	// as the write thread has already been stopped at this point, 
	// and they are no longer needed
	CloseHandle(m_hCancelPortWrite);
	m_hCancelPortWrite = INVALID_HANDLE_VALUE;
	CloseHandle(m_hWriteDataAvailable);
	m_hWriteDataAvailable = INVALID_HANDLE_VALUE;

	// STOP READ THREAD and clean up associated member variables
	// check if it exists just to be safe
	if (m_hPortReadThread != INVALID_HANDLE_VALUE)
	{
		// wait for read thread to stop
		// if not back after 1 second, kill it
		if (WaitForSingleObject(m_hPortReadThread, 1000) == WAIT_TIMEOUT)
		{
			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
						                 L"PortReadThread not finishing.  Resorting to TerminateThread()", 
							             L"CComm::Close(PBStruct *)", 
									     m_pPBStructure->iTabID,
										 0,
										 __LINE__);
			TerminateThread(m_hPortReadThread, 0);
		}
		CloseHandle(m_hPortReadThread);
		m_hPortReadThread = INVALID_HANDLE_VALUE;
	}
	// clear commevent URI
//	m_tcNavigateUrl[0] = NULL;
}

/////////////////////
// Private Methods //
/////////////////////

/*****************************************************************************/
DWORD WINAPI CComm::UserTimeoutThread(LPVOID instancePointer)
{
	// use instance pointer to call the thread function, 
	// which will have 'this' pointer
	return ((CComm *)instancePointer)->UserTimeoutFunction();
}

/*****************************************************************************/
DWORD WINAPI CComm::PortReadThread(LPVOID instancePointer)
{
	// use instance pointer to call the thread function, 
	// which will have 'this' pointer
	return ((CComm *)instancePointer)->PortReadFunction();
}

/*****************************************************************************/
DWORD WINAPI CComm::PortWriteThread(LPVOID instancePointer)
{
	// use instance pointer to call the thread function, 
	// which will have 'this' pointer
	return ((CComm *)instancePointer)->PortWriteFunction();
}

/*****************************************************************************/
DWORD WINAPI CComm::PortCloseThread(LPVOID instancePointer)
{
	// use instance pointer to call the Close() method, 
	((CComm *)instancePointer)->Close();
	return 0;
}

/*****************************************************************************/
// this is the functional core of the UserTimeoutThread.  It should only 
// be called from the UserTimeoutThread() method.
//
DWORD CComm::UserTimeoutFunction()
{
	DWORD	dwWaitResult;
	HANDLE	hEventHandles[2] = {m_hRestartUserTimeout, m_hCancelUserTimeout};
	int		iRestartEvent = WAIT_OBJECT_0;
	int		iCancelEvent  = WAIT_OBJECT_0 + 1;

	// wait until data arrives for the first time to start the timer loop,
	// or until the thread is cancelled
	if (WaitForMultipleObjects(2, hEventHandles, FALSE, INFINITE) == iCancelEvent)
		return 0;
	
	// timer loop
	while (TRUE)
	{
		// now, wait until the timer fires, 
		// or until more data arrives to restart the timer,
		// or until the port closing routing cancels the timeout thread
		dwWaitResult = WaitForMultipleObjects(2, hEventHandles, FALSE, m_iTimeout);
		if (dwWaitResult == WAIT_TIMEOUT)
		{
			// normal timeout processing
			// mutex prevents simultaneous read-thread and timeout-thread access
			WaitForSingleObject(m_hDataBufferMutex, INFINITE);
			// if there is data in the buffer, send it
			if (m_iDataBufferIndex != 0)
				SendNavigateData();
			// reset the buffer pointers
			m_iDataBufferIndex = 0;
			m_cDataBuffer[0] = '\0';
			ReleaseMutex(m_hDataBufferMutex);

			// wait for more data to arrive to restart the timer, 
			// or for the thread to be cancelled
			if (WaitForMultipleObjects(2, hEventHandles, FALSE, INFINITE) == iRestartEvent)
				continue;
			else
				return 0;
		}
		else if (dwWaitResult == iRestartEvent)
			// got an m_hRestartUserTimeout, so just go back to the wait again
			// via the while loop
			continue;
		else
			// got an m_hCancelUserTimeout, so just return
			return 0;
	}
}

/*****************************************************************************/
// this is the functional core of the PortReadThread.  It should only 
// be called from the PortReadThread() method.
//
DWORD CComm::PortReadFunction()
{
	DWORD		 dwNumBytesToRead = 0;
	DWORD		 dwNumBytesRead = 0;
	DWORD		 dwNumBytesLeft = 0;

	DWORD		 dwCommEventBits;
	COMSTAT		 commStatus;
	DWORD		 dwCommErrorValue;
	int			 iRemainingByteCount = 0;
	char		 cRecvChar;
	BYTE		*pcBuffPos = NULL;

	BOOL		 bReportedOverflow;

	DWORD		 dwError;

	BYTE		*cRecvCharBuffer = new BYTE[RECV_BUFFER_SIZE];

	m_cDataBuffer[0] = '\0';
	m_iDataBufferIndex = 0;
	m_bSawCR = FALSE;
	
	// First, make sure we have an open serial port, although this should never happen
	if (m_hPort == INVALID_HANDLE_VALUE)
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
					                 L"Error occurred - Read thread called with no valid serial port.", 
						             L"CComm::PortReadFunction()", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
		delete[] cRecvCharBuffer;
		return -1;
	}

	// enter read loop

	// check for an open port
	// if port is closed, return, which ends thread
	while (m_hPort != INVALID_HANDLE_VALUE)
	{
		// wait on the comm port to receive characters or to close
		// a port closing event puts '0' in dwCommEventBits
		if (WaitCommEvent(m_hPort, &dwCommEventBits, NULL) == TRUE)
		{
			// determine how many bytes have been received, if any
			ClearCommError(m_hPort, &dwCommErrorValue, &commStatus);
			iRemainingByteCount = commStatus.cbInQue;

			// restart timeout counter if timeouts are setup
			if (m_iTimeout > 0)
				SetEvent(m_hRestartUserTimeout);

			// if port is closed, exit this loop, which will then exit outer loop and thread
			while ((iRemainingByteCount > 0) && (m_hPort != INVALID_HANDLE_VALUE))
			{
				// limit read request to the size of the internal buffer
				dwNumBytesToRead = min(iRemainingByteCount, RECV_BUFFER_SIZE);

				if (ReadFile(m_hPort, cRecvCharBuffer, dwNumBytesToRead, &dwNumBytesRead, NULL) == TRUE)
				{
					iRemainingByteCount -= dwNumBytesRead;
					pcBuffPos = cRecvCharBuffer;
					dwNumBytesLeft = dwNumBytesRead;
					bReportedOverflow = FALSE;

					// process the input buffer into the data buffer one byte at a time
					while (dwNumBytesLeft--)
					{
						cRecvChar = (char)(*pcBuffPos);
						pcBuffPos++;

						// end-character based trigger if m_cEndChar is not NULL
						if (m_cEndChar != NULL)
						{
							// check each byte for the end character
							if (cRecvChar == m_cEndChar)
							{
								// call SendInputData to send contents of m_cDataBuffer
								// then cleanup m_cDataBuffer & pointers
								m_cDataBuffer[m_iDataBufferIndex] = cRecvChar;
								m_cDataBuffer[m_iDataBufferIndex + 1] = '\0';
								SendNavigateData();
								m_iDataBufferIndex = 0;
								m_cDataBuffer[0] = '\0';
							}
							else if (m_iDataBufferIndex < DATA_BUFFER_SIZE - 1)
							{
								// NOTE: backwards compatibility
								// only accumulate bytes if the buffer is not full
								// if the buffer is full, drop the input bytes
								m_cDataBuffer[m_iDataBufferIndex++] = cRecvChar;
								m_cDataBuffer[m_iDataBufferIndex] = '\0';
							}
							else if (!bReportedOverflow)
							{
								bReportedOverflow = TRUE;
								if (m_pWrapperClass)
									m_pWrapperClass->LogInst(PB_LOG_WARNING, 
											                 L"Warning -- Read data buffer full on serial port read.  Data not stored.", 
												             L"CComm::PortReadFunction()", 
															 m_pPBStructure->iTabID,
															 0,
													         __LINE__);		
							}
						}
						else if (m_bCRLF)
						{
							// check characters for CRLF
							if (m_bSawCR)
							{
								// check for LF following CR
								if (cRecvChar == LF)
								{
									SendNavigateData();
									m_iDataBufferIndex = 0;
									m_cDataBuffer[0] = '\0';
									m_bSawCR = FALSE;
								}
								// NOTE:
								// this state machine did nothing after having
								// seen a CR until a LF was encountered.  This has been
								// tested and confirmed.
							}
							else
							{
								// still looking for CR
								if (cRecvChar == CR)
									m_bSawCR = TRUE;
								else if (m_iDataBufferIndex < DATA_BUFFER_SIZE - 1)
								{
									// NOTE: backwards compatibility
									// only accumulate bytes if the buffer is not full
									// if the buffer is full, drop the input bytes
									m_cDataBuffer[m_iDataBufferIndex++] = cRecvChar;
									m_cDataBuffer[m_iDataBufferIndex] = '\0';
								}
								else if (!bReportedOverflow)
								{
									bReportedOverflow = TRUE;
									if (m_pWrapperClass)
										m_pWrapperClass->LogInst(PB_LOG_WARNING, 
												                 L"Warning -- Read data buffer full on serial port read.  Data not stored.", 
													             L"CComm::PortReadFunction()", 
																 m_pPBStructure->iTabID,
																 0,
														         __LINE__);		
								}
							}
						}
						else if (m_iCharCount) // count-based 
						{
							// check character count
							if (m_iDataBufferIndex == (m_iCharCount - 1))
							{
								// call SendInputData to send contents of m_cDataBuffer
								// then cleanup m_cDataBuffer & pointers
								m_cDataBuffer[m_iDataBufferIndex] = cRecvChar;
								m_cDataBuffer[m_iDataBufferIndex + 1] = '\0';
								SendNavigateData();
								m_iDataBufferIndex = 0;
								m_cDataBuffer[0] = '\0';
							}
							else if (m_iDataBufferIndex < DATA_BUFFER_SIZE - 1)
							{
								// NOTE: backwards compatibility
								// only accumulate bytes if the buffer is not full
								// if the buffer is full, drop the input bytes
								m_cDataBuffer[m_iDataBufferIndex++] = cRecvChar;
								m_cDataBuffer[m_iDataBufferIndex] = '\0';
							}
							else if (!bReportedOverflow)
							{
								bReportedOverflow = TRUE;
								if (m_pWrapperClass)
									m_pWrapperClass->LogInst(PB_LOG_WARNING, 
										                     L"Warning -- Read data buffer full on serial port read.  Data not stored.", 
											                 L"CComm::PortReadFunction()", 
															 m_pPBStructure->iTabID,
															 0,
												             __LINE__);		
							}
						}
						else	// timeout-based
						{
							// just accumulate, don't check for anything
							// make sure the commnavigate timeout thread is not 
							// sending previous data before writing input data
							// mutex is not needed for other conditions, because
							// all buffer activity is occurring in the main thread
							if (m_iDataBufferIndex < DATA_BUFFER_SIZE - 1)
							{
								// NOTE: backwards compatibility
								// only accumulate bytes if the buffer is not full
								// if the buffer is full, drop the input bytes & log warning
								WaitForSingleObject(m_hDataBufferMutex, INFINITE);
								m_cDataBuffer[m_iDataBufferIndex++] = cRecvChar;
								m_cDataBuffer[m_iDataBufferIndex] = '\0';
								ReleaseMutex(m_hDataBufferMutex);
							}
							else if (!bReportedOverflow)
							{
								bReportedOverflow = TRUE;
								if (m_pWrapperClass)
									m_pWrapperClass->LogInst(PB_LOG_WARNING, 
										                     L"Warning -- Read data buffer full on serial port read.  Data not stored.", 
											                 L"CComm::PortReadFunction()", 
															 m_pPBStructure->iTabID,
															 0,
												             __LINE__);		
							}
						}
					}
				}
				else // read error occurred
				{
					// This is a bad error, so close the port and return
					dwError = GetLastError();
					if (m_pWrapperClass)
						m_pWrapperClass->LogInst(PB_LOG_WARNING, 
												 L"Error occurred -- ReadFile on m_hPort returned FALSE.", 
												 L"CComm::PortReadFunction()", 
												 m_pPBStructure->iTabID,
												 dwError,
												 __LINE__);
					cRecvCharBuffer[0] = '\0';
					iRemainingByteCount = 0;

					// close the port via a thread to avoid deadlock where Close() waits for
					// this thread to finish while it's blocked on Close()
					HANDLE hPortCloseThread = CreateThread(NULL, 0, PortCloseThread, this, 0, NULL);
					CloseHandle(hPortCloseThread);
					hPortCloseThread = INVALID_HANDLE_VALUE;

					delete[] cRecvCharBuffer;
					return -1;
				}
			} // while ((iRemainingByteCount > 0) && (m_hPort != INVALID_HANDLE_VALUE))
		}
		else // wait error occurred
		{
			// This is a bad error, so close the port and return
			dwError = GetLastError();
			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
										 L"Error occurred -- WaitCommEvent on m_hPort returned FALSE.", 
										 L"CComm::PortReadFunction()", 
										 m_pPBStructure->iTabID,
										 dwError,
										 __LINE__);
			cRecvCharBuffer[0] = '\0';
			iRemainingByteCount = 0;

			// close the port via a thread to avoid deadlock where Close() waits for
			// this thread to finish while it's blocked on Close()
			HANDLE hPortCloseThread = CreateThread(NULL, 0, PortCloseThread, this, 0, NULL);
			CloseHandle(hPortCloseThread);
			hPortCloseThread = INVALID_HANDLE_VALUE;
			
			delete[] cRecvCharBuffer;
			return -1;
		}

	} // while (m_hPort != INVALID_HANDLE_VALUE)
	
	return 0;
}

/*****************************************************************************/
BOOL CComm::SendNavigateData()
{
	char		cSendData[DATA_BUFFER_SIZE * 2];
	TCHAR		tcSendData[DATA_BUFFER_SIZE * 2];
	UINT		j = 0;
	UINT		i = 0;

	tcSendData[0] = NULL;
	cSendData[0] = NULL;

	// look for the following list of characters and escape them if found
	for (i = 0; i < strlen(m_cDataBuffer); i++)
	{
		switch (m_cDataBuffer[i])
		{
		case '\f':
			strcat(cSendData, "\\f");
			cSendData[j+=2] = NULL;
			break;
		case '\n':
			strcat(cSendData, "\\n");
			cSendData[j+=2] = NULL;
			break;
		case '\r':
			strcat(cSendData, "\\r");
			cSendData[j+=2] = NULL;
			break;
		case '\t':
			strcat(cSendData, "\\t");
			cSendData[j+=2] = NULL;
			break;
		case '\"':
			strcat(cSendData, "\\\"");
			cSendData[j+=2] = NULL;
			break;
		case '\'':
			strcat(cSendData, "\\'");
			cSendData[j+=2] = NULL;
			break;
		case '\\':
			strcat(cSendData, "\\\\");
			cSendData[j+=2] = NULL;
			break;
		case '\b':
			strcat(cSendData, "\\b");
			cSendData[j+=2] = NULL;
			break;
		case '\?':
			strcat(cSendData, "\\?");
			cSendData[j+=2] = NULL;
			break;
		case '%':
			strcat(cSendData, "\\045");
			cSendData[j+=4] = NULL;
			break;
		default :
			cSendData[j] = m_cDataBuffer[i];
			cSendData[++j] = NULL;
			break;
		}
	}  // for loop

	// convert from char to TCHAR first, then navigate
	mbstowcs(tcSendData, cSendData, DATA_BUFFER_SIZE * 2);

	// first check for a NULL URL, and do a SendKey sequence if no URL

	if (m_tcNavigateUrl[0] == NULL)
	{
		m_pCSIP->SendString(tcSendData);
		if (m_bAutoEnter)
		{
			m_pCSIP->SendKey(VK_RETURN);
		}
		if (m_bAutoTab)
		{
			m_pCSIP->SendKey(VK_TAB);
		}
		return TRUE;
	}

	// if the URL is valid, send data and URL to core
	if (m_pWrapperClass)
	{
		// first, protect URL from being updated while the
		// navigate operation is in process
		WaitForSingleObject(m_hNavigateUrlMutex, INFINITE);
		m_pWrapperClass->SendPBNavigateInst(m_pPBStructure, m_tcNavigateUrl, tcSendData);
		ReleaseMutex(m_hNavigateUrlMutex);
	}
	else
	{
		if (m_pWrapperClass)
			m_pWrapperClass->LogInst(PB_LOG_WARNING, 
									 L"Error occurred -- No navigate function defined (NULL pNavigateFunc pointer)", 
									 L"CComm::SendInputData()", 
									 m_pPBStructure->iTabID,
									 0,
									 __LINE__);
	}
	return TRUE;
}

/*****************************************************************************/
// this is the functional core of the PortWriteThread.  It should only 
// be called from the PortWriteThread() method.
//
DWORD CComm::PortWriteFunction()
{
	DWORD			 dwNumBytesRead = 0;
	DWORD			 dwBytesWritten = 0;
	DWORD			 dwError;

	HANDLE			 hEventHandles[2] = {m_hCancelPortWrite, m_hWriteDataAvailable};
	int				 iCancelEvent = WAIT_OBJECT_0;
		
	WRITEELEMENT	*pWriteElement;

	char			szWriteBuffer[WRITE_BUFFER_SIZE];
	
	// First, make sure we have an open serial port, although this should never happen
	if (m_hPort == INVALID_HANDLE_VALUE)
	{
			if (m_pWrapperClass)
				m_pWrapperClass->LogInst(PB_LOG_WARNING, 
					                     L"Error occurred - Write thread called with no valid serial port.", 
						                 L"CComm::PortWriteFunction()", 
										 m_pPBStructure->iTabID,
										 0,
									     __LINE__);
		return -1;
	}

	// the port can be closed while writes are processing, if this happens, this
	// outer loop will catch it
	while (m_hPort != INVALID_HANDLE_VALUE)
	{
		// if the wait returns a WAIT_OBJECT_0, then it was triggered by a m_hPortClosing signal
		// and the queue, if it exists, should be freed and the loop exited
		if (WaitForMultipleObjects(2, hEventHandles, FALSE, INFINITE) == iCancelEvent)
		{
			// if the port is closing, just cleanup the queue
			while (m_pWriteQueueHead)
			{
				pWriteElement = m_pWriteQueueHead;
				m_pWriteQueueHead = m_pWriteQueueHead->next;
				if (pWriteElement->szWriteBuffer)
					delete[] pWriteElement->szWriteBuffer;
				delete pWriteElement;
			}
			break;
		}
		// if the wait does not return a WAIT_OBJECT_0, then it was triggered by the 
		// availability of data, and the data should be written
		else
		{
			// as long as there is data in the queue, continue in inner loop
			while (m_pWriteQueueHead)
			{
				if (m_pWriteQueueHead->writeType == COMM_WRITE_FILE)
				{
					// open the file containing the data to write
					m_hWriteFile = CreateFile(m_pWriteQueueHead->tcWriteFileName, 
		                            GENERIC_READ, 
									0, NULL, 
									OPEN_EXISTING, 
									FILE_ATTRIBUTE_NORMAL, 
									NULL);

					// make sure the file actually opened
					if (m_hWriteFile == INVALID_HANDLE_VALUE)
					{
						dwError = GetLastError();
						if (m_pWrapperClass)
							m_pWrapperClass->LogInst(PB_LOG_WARNING, 
									                 L"Error occurred -- CreateFile on m_hWriteFile returned INVALID_HANDLE_VALUE.", 
												     L"CComm::PortWriteFunction()", 
													 m_pPBStructure->iTabID,
													 dwError,
												     __LINE__);
						
						// skip this queue element, and go on to process the next queue element
						WaitForSingleObject(m_hWriteQueueMutex, INFINITE);
	
						pWriteElement = m_pWriteQueueHead;
						m_pWriteQueueHead = m_pWriteQueueHead->next;
						if (pWriteElement->szWriteBuffer)
							delete[] pWriteElement->szWriteBuffer;
						delete pWriteElement;

						ReleaseMutex(m_hWriteQueueMutex);

						continue;
					}
					while (TRUE)
					{
						// read the data to be written from the file and check for read errors
						if (!ReadFile(m_hWriteFile, szWriteBuffer, WRITE_BUFFER_SIZE - 1, &dwNumBytesRead, NULL))
						{
							dwError = GetLastError();
							if (m_pWrapperClass)
								m_pWrapperClass->LogInst(PB_LOG_WARNING, 
											             L"Error occurred -- ReadFile on m_hWriteFile returned FALSE.", 
														 L"CComm::PortWriteFunction()", 
														 m_pPBStructure->iTabID,
														 dwError,
														 __LINE__);
							// skip this queue element, and go on to process the next queue element
							WaitForSingleObject(m_hWriteQueueMutex, INFINITE);
	
							pWriteElement = m_pWriteQueueHead;
							m_pWriteQueueHead = m_pWriteQueueHead->next;
							if (pWriteElement->szWriteBuffer)
								delete[] pWriteElement->szWriteBuffer;
							delete pWriteElement;

							ReleaseMutex(m_hWriteQueueMutex);

							break;
						}

						// check first that the port has not been closed by another thread 
						// between the top of this loop and here
						if (m_hPort != INVALID_HANDLE_VALUE)
						{
							// do the write and check the result
							if (!WriteFile(m_hPort, szWriteBuffer, dwNumBytesRead, &dwBytesWritten, NULL))
							{
								dwError = GetLastError();
								if (m_pWrapperClass)
									m_pWrapperClass->LogInst(PB_LOG_WARNING, 
											                 L"Error occurred -- WriteFile call on m_hPort returned FALSE (COMM_WRITE_FILE).", 
														     L"CComm::PortWriteFunction()", 
															 m_pPBStructure->iTabID,
															 dwError,
															 __LINE__);
								break;
							}
						}
						else // port has been closed since the top of the loop
						{
							if (m_pWrapperClass)
								m_pWrapperClass->LogInst(PB_LOG_WARNING, 
									                     L"Warning -- m_hPort has been closed prior to writing data (COMM_WRITE_FILE).", 
														 L"CComm::PortWriteFunction()", 
														 m_pPBStructure->iTabID,
														 0,
														 __LINE__);
						}

						// finish writing the file when reading the data file does not fill the buffer
						if (dwNumBytesRead < WRITE_BUFFER_SIZE - 1)
							break;

					}	// while (TRUE)

					// close the file containing the data to write
					CloseHandle(m_hWriteFile);
					m_hWriteFile = INVALID_HANDLE_VALUE;
				}
				else	// either COMM_WRITE_BYTES or COMM_WRITE_STRING
				{
					// if bytes are being written rather than a string, then
					// some pre-processing of the bytes is required
					char *wptr = m_pWriteQueueHead->szWriteBuffer;
					char *rptr = m_pWriteQueueHead->szWriteBuffer;
					int   hexValue;
					if (m_pWriteQueueHead->writeType == COMM_WRITE_BYTES)
					{
						// convert the buffer in-place.  rptr will lead wptr, as the
						// values to be converted plus delimiting '%' are 
						// always greater than one byte combined
						while (*rptr)
						{
							// skip to the next '%' character
							if (*rptr != '%')
								++rptr;
							// process block of characters
							else
							{
								// convert block of chars to hex and advance pointers
								++rptr;
								sscanf(rptr, "%x", &hexValue);
								*wptr++ = (char)hexValue;
								++rptr;
								// skip to the next '%' character
								while ((*rptr) && (*rptr != '%'))
									++rptr;
							}
						}
					}
					else
					{
						//COMM_WRITE_STRING
						wptr = m_pWriteQueueHead->szWriteBuffer + strlen(wptr);
					}

					// do the write and check the result
					if (!WriteFile(m_hPort, m_pWriteQueueHead->szWriteBuffer,  wptr-m_pWriteQueueHead->szWriteBuffer, &dwBytesWritten, NULL))
					{
						dwError = GetLastError();
						if (m_pWrapperClass)
							m_pWrapperClass->LogInst(PB_LOG_WARNING, 
									                 L"Error occurred -- WriteFile call on m_hPort returned FALSE (COMM_WRITE_BYTES/COMM_WRITE_STRING).", 
										             L"CComm::PortWriteFunction()", 
													 m_pPBStructure->iTabID,
													 dwError,
											         __LINE__);
					}
				} //if (m_pWriteQueueHead->writeType == COMM_WRITE_FILE)
		
				// surround pointer manipulation with mutex since main thread can
				// modify this as well as this write thread
				WaitForSingleObject(m_hWriteQueueMutex, INFINITE);
	
				pWriteElement = m_pWriteQueueHead;
				m_pWriteQueueHead = m_pWriteQueueHead->next;
				if (pWriteElement->szWriteBuffer)
					delete[] pWriteElement->szWriteBuffer;
				delete pWriteElement;

				ReleaseMutex(m_hWriteQueueMutex);

			} // while (m_pWriteQueueHead)
		} // if (WaitForMultipleObjects)

	} // while (m_hPort != INVALID_HANDLE_VALUE)
	// try one last time to free the queue in case the m_hPort == INVALID_HANDLE_VALUE got us
	// here rather than the WaitForMultipleObjects call
	while (m_pWriteQueueHead)
	{
		pWriteElement = m_pWriteQueueHead;
		m_pWriteQueueHead = m_pWriteQueueHead->next;
		if (pWriteElement->szWriteBuffer)
			delete[] pWriteElement->szWriteBuffer;
		delete pWriteElement;
	}

	return 0;
}
