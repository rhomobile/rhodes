#include "Network.h"


#define DEFAULT_NETWORK_POLL_INTERVAL 5000
#define DEFAULT_CONNECTION_TIMEOUT_SECONDS 1
#define DEFAULT_NETWORK_URL L"www.symbol.com"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
CNetwork::CNetwork(int iInstanceID, PBModule* pModule)
{
	//  Initialise Variables
	m_tcHost = NULL;
	m_tcProxy = NULL;
	m_hNetworkChecker = NULL;
	m_iPort = INTERNET_DEFAULT_HTTP_PORT;
	m_iNetworkPollInterval = DEFAULT_NETWORK_POLL_INTERVAL;
	m_connectionTimeout.tv_sec = DEFAULT_CONNECTION_TIMEOUT_SECONDS;
	m_connectionTimeout.tv_usec = 0;
	m_bShuttingDown = FALSE;
	m_pModule		= pModule;
	m_iInstanceID	= iInstanceID;
	memset(m_tcNetworkEventURI, 0, MAXURL * sizeof(TCHAR));
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
CNetwork::~CNetwork()
{
	//  Terminate separate thread
	m_bShuttingDown = TRUE;
	SetEvent(m_hStopNetworkCheckerEvent);

	//  and Wait for threads to stop
	if (WaitForSingleObject(m_hNetworkChecker, 1000) == WAIT_TIMEOUT)
	{
		//  Terminate Thread if it didn't shut down within a second
		TerminateThread(m_hNetworkChecker, 0);
	}

	//  Free member variable memory
	delete[] m_tcHost;
	delete[] m_tcProxy;

	//  Close all the Handles
	CloseHandle(m_hNetworkChecker);
	CloseHandle(m_hStopNetworkCheckerEvent);
	CloseHandle(m_hCheckFrequencyChangedEvent);

	//  Set member pointers to null
	m_hStopNetworkCheckerEvent = NULL;
	m_hCheckFrequencyChangedEvent = NULL;
	m_hNetworkChecker = NULL;	

	WSACleanup();

}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::Initialise() 
{
	m_tcHost = new TCHAR[wcslen(DEFAULT_NETWORK_URL) + 1];
	if (!m_tcHost)
		return FALSE;
	wcscpy(m_tcHost, DEFAULT_NETWORK_URL);

	LPCTSTR pProxySetting = m_pModule->GetAppSettingPtr(m_iInstanceID, L"HTTP_Proxy", L"Value");
	if (pProxySetting && pProxySetting[0])
	{
		m_tcProxy = new TCHAR[wcslen(pProxySetting) + 1];
		if (!m_tcProxy)
			return FALSE;
		memset(m_tcProxy, 0, (wcslen(pProxySetting) + 1) * sizeof(TCHAR));
		wcscpy(m_tcProxy, pProxySetting);
	}

	m_hStopNetworkCheckerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hCheckFrequencyChangedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (!m_hStopNetworkCheckerEvent)
		return FALSE;
	else
		return TRUE;
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::SetNetworkEventURI(LPCWSTR szNavURI)
{
	//  store the passed navigation URI
	if (wcslen(szNavURI) < MAX_URL)
	{
		memset(m_tcNetworkEventURI, 0, MAX_URL * sizeof(TCHAR));
		wcscpy(m_tcNetworkEventURI, szNavURI);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::SetHost(LPCWSTR tcHost)
{
	//  Store the given host in m_tcHost
	bool bRetVal = false;
	if (m_tcHost)
	{
		//  There is already a hostname stored, remove it from memory
		delete[] m_tcHost;
		m_tcHost = NULL;
	}
	
	if (tcHost && wcslen(tcHost) < MAX_URL)
	{	
		m_tcHost = new TCHAR[wcslen(tcHost) + 1];
		if (m_tcHost)
		{
			memset(m_tcHost, 0, (wcslen(tcHost) + 1) * sizeof(TCHAR));
			if (wcslen(tcHost) > wcslen(L"http://") && wcsnicmp(tcHost, L"http://", wcslen(L"http://")) == 0)
				wcscpy(m_tcHost, tcHost + wcslen(L"http://"));
			else
				wcscpy(m_tcHost, tcHost);
		}
		bRetVal = true;
	}
	return bRetVal;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::SetPort(int iPort)
{
	m_iPort = iPort;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::SetConnectionTimeout (int iTimeout)
{
	//  This is the Socket Connection timeout, specified in milliseconds
	int iSeconds = (iTimeout - (iTimeout % 1000)) / 1000;
	int iMicroseconds = (iTimeout - (iSeconds * 1000)) * 1000;

	m_connectionTimeout.tv_sec = (long)iSeconds;
	m_connectionTimeout.tv_usec = (long)iMicroseconds;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::SetNetworkPollInterval(int iInterval)
{
	m_iNetworkPollInterval = iInterval;
	SetEvent(m_hCheckFrequencyChangedEvent);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::StartNetworkChecking()
{
	m_NetworkState = NETWORK_INITIALISING;
	if (m_hNetworkChecker == NULL || WaitForSingleObject(m_hNetworkChecker,0) == WAIT_OBJECT_0)
		m_hNetworkChecker = CreateThread(NULL, 0, 
			(LPTHREAD_START_ROUTINE)NetworkChecker, this, 0, NULL);
	return (m_hNetworkChecker != NULL);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetwork::StopNetworkChecking()
{
	//  Only stop the network checking if it is currently running
	if (m_hNetworkChecker)
	{
		SetEvent(m_hStopNetworkCheckerEvent);
		if (WaitForSingleObject(m_hNetworkChecker, 1000) == WAIT_TIMEOUT)
		{
			//  Terminate Thread if it didn't shut down within a second
			TerminateThread(m_hNetworkChecker, 0);
		}
		m_hNetworkChecker = NULL;
	}
	return TRUE;
}



//////////////////////////////////
//
//  Private Methods
//
//////////////////////////////////

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		August 2011 (Initial Creation)
*/
void CNetwork::CheckConnectivity()
{
	if (m_tcHost == NULL || m_tcHost[0] == NULL)
	{
		m_pModule->Log(PB_LOG_INFO, L"Attempted to retrieve network connectivity but no host provided", 
			_T(__FUNCTION__), __LINE__);
		return;
	}
	if (m_tcNetworkEventURI == NULL || m_tcNetworkEventURI[0] == NULL)
	{
		m_pModule->Log(PB_LOG_INFO, L"Attempted to retrieve network connectivity but no network event provided", 
			_T(__FUNCTION__), __LINE__);
		return;
	}
	bool bConnectSuccessful = false;
	struct addrinfo hints, *result = NULL, *ptr = NULL;
	SOCKET sockfd = INVALID_SOCKET;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	char szPortAsString[INTERNET_MAX_PORT_NUMBER_LENGTH + 1];
	_itoa(m_iPort, szPortAsString, 10);
	char* szHost = new char[wcslen(m_tcHost) + 1];
	memset(szHost, 0, wcslen(m_tcHost) + 1);
	wcstombs(szHost, m_tcHost, wcslen(m_tcHost));
	int iResult = getaddrinfo(szHost, szPortAsString, &hints, &result);
	if (iResult != 0)
	{
		//  Log the Fact that we can't get the addr info
		WCHAR wLogMessage[1024];
		int iErr = WSAGetLastError();
		wsprintf(wLogMessage, L"Attempted to resolve hostname to connect to but did not succeed, return value (%i), last error (%i)", 
			iResult, iErr);
		m_pModule->Log(PB_LOG_INFO, wLogMessage, 
			_T(__FUNCTION__), __LINE__);
	}
	else
	{
		ptr=result;
		sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (sockfd == INVALID_SOCKET)
		{
			WCHAR wLogMessage[1024];
			int iErr = WSAGetLastError();
			wsprintf(wLogMessage, L"Unable to create communications socket, last error was %i", iErr);
			m_pModule->Log(PB_LOG_INFO, wLogMessage, 
				_T(__FUNCTION__), __LINE__);
		}
		else
		{
			//  Make the Socket Non Blocking
			u_long iNonBlock = 1;
			int iNonBlockRes = ioctlsocket(sockfd, FIONBIO, &iNonBlock);
			if (iNonBlockRes != NO_ERROR)
			{
				m_pModule->Log(PB_LOG_INFO, L"Error Setting socket into Non Blocking mode", 
					_T(__FUNCTION__), __LINE__);
			}
			else
			{
				int iConnectSuccess = connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);
				//  Because Socket is non blocking we expect it to return SOCKET_ERROR
				//  and WSAGetLastError() would be WSAEWOULDBLOCK
				if (!(iConnectSuccess == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK))
				{
						//  For some reason the socket did not block
						m_pModule->Log(PB_LOG_WARNING, L"Socket Operation unexpectedly blocked, are you connected to a PC?", 
							_T(__FUNCTION__), __LINE__);
				}
				else
				{
					fd_set WriteFDs;
					FD_ZERO(&WriteFDs);
					FD_SET(sockfd, &WriteFDs);
					int iSelectReturnVal = select(0, 0, &WriteFDs, 0, &m_connectionTimeout);
					if (iSelectReturnVal > 0)
					{
						//  We have a socket to connect to
						bConnectSuccessful = true;
					}
					else if (iSelectReturnVal == 0)
					{
						//  The socket did not become writeable within the timeout period
						m_pModule->Log(PB_LOG_INFO, L"Unable to connect to specified host", 
							_T(__FUNCTION__), __LINE__);
					}
					else
					{
						//  Some form of error occured
						WCHAR wLogMessage[1024];
						int iErr = WSAGetLastError();
						wsprintf(wLogMessage, L"Unable to connect to specified host, last error was %i", iErr);
						m_pModule->Log(PB_LOG_INFO, wLogMessage, 
							_T(__FUNCTION__), __LINE__);
					}
				}
			}
			closesocket(sockfd);
			sockfd = INVALID_SOCKET;
		}
	}
	delete[] szHost;
	if (bConnectSuccessful)
	{
		if (m_NetworkState != NETWORK_CONNECTED)
		{
			m_pModule->SendPBNavigate(tcNetworkEventNames,
												m_iInstanceID,
												m_tcNetworkEventURI,
												L"Connected", NULL);
			m_pModule->Log(PB_LOG_INFO, L"Connected to Network", 
								_T(__FUNCTION__), __LINE__);
			m_NetworkState = NETWORK_CONNECTED;
		}
	}
	else
	{
		//  We are not connected
		if (m_NetworkState != NETWORK_DISCONNECTED)
		{
			m_pModule->SendPBNavigate(tcNetworkEventNames,
												m_iInstanceID,
												m_tcNetworkEventURI,
												L"Disconnected", NULL);
			m_pModule->Log(PB_LOG_INFO, L"Disconnected from Network", 
								_T(__FUNCTION__), __LINE__);
			m_NetworkState = NETWORK_DISCONNECTED;
		}
	}
}
//////////////////////////
//  Private Threads		//
//////////////////////////

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
DWORD CNetwork::NetworkChecker( LPVOID lpParameter )
{
	CNetwork* pNetwork = (CNetwork*)lpParameter;
	bool exitThread = false;
	DWORD dwEvent;
	HANDLE hWaitHandles[2];
	hWaitHandles[0] = pNetwork->m_hStopNetworkCheckerEvent;
	hWaitHandles[1] = pNetwork->m_hCheckFrequencyChangedEvent;
	pNetwork->CheckConnectivity();

	while (true)
	{
		//  Wait for an exit event (indicating stop the thread) or timeout
		dwEvent = WaitForMultipleObjects(
			2,
			hWaitHandles,
			FALSE,
			pNetwork->m_iNetworkPollInterval);
//		pNetwork->m_pModule->Log(PB_LOG_INFO, L"Stopped Waiting", _T(__FUNCTION__), __LINE__);
		switch (dwEvent)
		{
		case WAIT_OBJECT_0:
			{
				goto _exitThread;
			}
		case WAIT_OBJECT_0 + 1:
			{
				//  m_hQueryFrequencyChangedEvent, user has changed the query
				//  frequency, no action just start waiting again.
				break;
			}
		case WAIT_TIMEOUT:
			{
				//  Timed out 
				//  Check Network connectivity
				pNetwork->CheckConnectivity();
				break;
			}

		case WAIT_FAILED:
			{
				//  We never expect this to happen, something else has gone badly wrong.
				DWORD err = GetLastError();
				WCHAR szLogMsg[256];
				wsprintf(szLogMsg, L"Failed to wait for network event, last error: %i", err);
				CloseHandle(pNetwork->m_hStopNetworkCheckerEvent);
				CloseHandle(pNetwork->m_hCheckFrequencyChangedEvent);
				pNetwork->m_hStopNetworkCheckerEvent = NULL;
				pNetwork->m_hCheckFrequencyChangedEvent = NULL;
				pNetwork->m_pModule->Log(PB_LOG_ERROR, szLogMsg, 
					_T(__FUNCTION__), __LINE__);
				goto _exitThread;
			}
		}  //  End Switch
	}	//  End While !exitThread
_exitThread:
	DEBUGMSG(TRUE, (L"Network Checker Thread Exiting\n"));
	return 0;
}
