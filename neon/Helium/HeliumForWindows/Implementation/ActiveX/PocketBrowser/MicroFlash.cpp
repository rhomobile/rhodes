////////////////////////////////////////////////////////////////////////////////
// Filename: MicroFlash.cpp
// Author:   PB2.x
// Date:     PB2.x
//
// Description: Implementation of the CMicroFlash class, this has been 
//				unmodified from PB2.x
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MicroFlash.h"


/////////////////////////////////////////////////////////////////////////////
// CMicroFlash

/******************************************************************************/
BOOL CMicroFlash::OpenPort (void)
{
	DCB ComSet;

	m_hComPort = CreateFile(m_strPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
							OPEN_EXISTING, 0, NULL);

	DWORD err = GetLastError();

	if(m_hComPort==INVALID_HANDLE_VALUE) return FALSE;

	if (!GetCommState(m_hComPort, &ComSet)) return FALSE;

	ComSet.BaudRate = 9600;
	ComSet.fBinary = TRUE;
	ComSet.fOutxCtsFlow = FALSE;//TRUE;
	ComSet.fOutxDsrFlow = FALSE;//TRUE;
	ComSet.fDtrControl = DTR_CONTROL_ENABLE;
	ComSet.fTXContinueOnXoff = FALSE;
	ComSet.fRtsControl = RTS_CONTROL_ENABLE;
	
	ComSet.fOutX = TRUE;
	ComSet.fInX = TRUE;
	ComSet.ByteSize = 8;
	ComSet.Parity = NOPARITY;
	ComSet.StopBits = ONESTOPBIT;

	if (!SetCommState(m_hComPort, &ComSet)) return FALSE;

	return TRUE;
}

/******************************************************************************/
void CMicroFlash::ClosePort (void)
{
 	CloseHandle(m_hComPort);
	m_hComPort = INVALID_HANDLE_VALUE;
	return;
}

/******************************************************************************/
STDMETHODIMP CMicroFlash::AddData(BSTR strData)
{
	m_strData.Append (strData);

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CMicroFlash::AddLine(BSTR strLine)
{
	m_strData.Append (strLine);
	m_strData.Append (TEXT("\r\n"));

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CMicroFlash::Clear()
{
	m_strData.Empty ();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CMicroFlash::Print(int iCopies)
{
	HRESULT r;
	char endLine [] = "\r\n";
	DWORD dwBytesWritten;
	char *pcPrintBuf;
	int iLineCount, n;

	// Assume failure
	r = S_FALSE;

	pcPrintBuf = NULL;
	iLineCount = 0;

	if(!OpenPort()) goto Exit;

	// Add formfeed to data
	m_strData.Append (TEXT("\x0C"));

	// Convert to char data
	n = m_strData.Length ();
	pcPrintBuf = new char [n + 1];
	wcstombs (pcPrintBuf, m_strData, n);
	pcPrintBuf [n] = 0;

	// Wakeup the printer
	for(iLineCount = 0; iLineCount < 10; iLineCount++)
		WriteFile(m_hComPort, endLine, strlen(endLine), &dwBytesWritten, NULL);

	Sleep(500);

	// Print the buffer
	for(iLineCount = 0; iLineCount < iCopies; iLineCount++)
		if(!WriteFile(m_hComPort, pcPrintBuf, strlen(pcPrintBuf), 
			&dwBytesWritten, NULL)) goto Exit;

	m_strData.Empty ();

	r = S_OK;

Exit:
	ClosePort ();
	delete [] pcPrintBuf;

	return r;
}

STDMETHODIMP CMicroFlash::SetPort(BSTR strPort)
{
	m_strPort = strPort;

	return S_OK;
}
