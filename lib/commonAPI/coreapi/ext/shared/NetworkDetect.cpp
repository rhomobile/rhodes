#include "NetworkDetect.h"
#include <sstream>

#define DEFAULT_NETWORK_POLL_INTERVAL 30000
#define DEFAULT_CONNECTION_TIMEOUT_SECONDS 1
#define DEFAULT_NETWORK_URL "www.motorolasolutions.com"
#define	DEFAULT_PORT 80
#define NETWORK_DETECT_MINIMUM_POLL_INTERVAL 5000

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
CNetworkDetection::CNetworkDetection()
{
	//  Initialise Variables
	m_szHost = DEFAULT_NETWORK_URL;
	m_iPort = DEFAULT_PORT;
	m_iNetworkPollInterval = DEFAULT_NETWORK_POLL_INTERVAL;
	m_connectionTimeout.tv_sec = DEFAULT_CONNECTION_TIMEOUT_SECONDS;
	m_connectionTimeout.tv_usec = 0;
	m_szLastError = "";
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
CNetworkDetection::~CNetworkDetection()
{
	//  Terminate separate thread
	stop(1000);

	WSACleanup();
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetworkDetection::Initialise() 
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	return TRUE;
}


BOOL CNetworkDetection::SetHost(const rho::String& szHost)
{
	if (szHost.length() > 7 && szHost.compare(0, 7, "http://") == 0)
		m_szHost = szHost.substr(7);
	else if (szHost.length() > 8 && szHost.compare(0, 8, "https://") == 0)
		m_szHost = szHost.substr(8);
	else
		m_szHost = szHost;
	return TRUE;
}

BOOL CNetworkDetection::SetConnectionTimeout (int iTimeout)
{
	//  This is the Socket Connection timeout, specified in milliseconds
	int iSeconds = (iTimeout - (iTimeout % 1000)) / 1000;
	int iMicroseconds = (iTimeout - (iSeconds * 1000)) * 1000;

	m_connectionTimeout.tv_sec = (long)iSeconds;
	m_connectionTimeout.tv_usec = (long)iMicroseconds;

	return TRUE;
}

BOOL CNetworkDetection::SetNetworkPollInterval(int iInterval)
{
	if (iInterval >= NETWORK_DETECT_MINIMUM_POLL_INTERVAL)
	{
		m_iNetworkPollInterval = iInterval;
		if (isWaiting())
			stopWait();
		return TRUE;
	}
	else
	{
		LOG(INFO) + "Network Poll Interval property ignored, value is too small, it must be more than " + NETWORK_DETECT_MINIMUM_POLL_INTERVAL;
		return FALSE;
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetworkDetection::StartNetworkChecking()
{
	m_NetworkState = NETWORK_INITIALISING;
	start(epNormal);
	return true;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
BOOL CNetworkDetection::StopNetworkChecking()
{
	//  Only stop the network checking if it is currently running
	stop(10000);
	return TRUE;
}

BOOL CNetworkDetection::IsChecking()
{
	return isAlive();
}

void CNetworkDetection::SetCallback(rho::apiGenerator::CMethodResult* pCallback)
{
	m_pDetectCallback = pCallback;
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
void CNetworkDetection::CheckConnectivity()
{
	bool bConnectSuccessful = false;
	struct addrinfo hints, *result = NULL, *ptr = NULL;
	SOCKET sockfd = INVALID_SOCKET;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	char szPortAsString[5 + 1];
	_itoa(m_iPort, szPortAsString, 10);
	char* szHost = new char[m_szHost.length() + 1];
	memset(szHost, 0, m_szHost.length() + 1);
	strcpy(szHost, m_szHost.c_str());
	int iResult = getaddrinfo(szHost, szPortAsString, &hints, &result);
	if (iResult != 0)
	{
		//  Log the Fact that we can't get the addr info
		int iErr = WSAGetLastError();
		m_szLastError = "Attempted to resolve hostname to connect to but did not succeed, return value (" + itos(iResult) + 
			"), last error (" + itos(iErr) + ")";
		LOG(INFO) + m_szLastError;
	}
	else
	{
		ptr=result;
		sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (sockfd == INVALID_SOCKET)
		{
			int iErr = WSAGetLastError();
			m_szLastError = "Unable to create communications socket, last error was " + itos(iErr);
			LOG(INFO) + m_szLastError;
		}
		else
		{
			//  Make the Socket Non Blocking
			u_long iNonBlock = 1;
			int iNonBlockRes = ioctlsocket(sockfd, FIONBIO, &iNonBlock);
			if (iNonBlockRes != NO_ERROR)
			{
				m_szLastError = "Error setting socket into Non Blocking mode";
				LOG(INFO) + m_szLastError;
			}
			else
			{
				int iConnectSuccess = connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);
				//  Because Socket is non blocking we expect it to return SOCKET_ERROR
				//  and WSAGetLastError() would be WSAEWOULDBLOCK
				if (!(iConnectSuccess == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK))
				{
					m_szLastError = "Socket Operation unexpectedly blocked, are you connected to a PC?";
					LOG(WARNING) + m_szLastError;
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
						m_szLastError = "Unable to connect to specified host " + m_szHost + " on port " + itos(m_iPort);
						LOG(INFO) + m_szLastError;
					}
					else
					{
						//  Some form of error occured
						int iErr = WSAGetLastError();
						m_szLastError = "Unable to connect to specified host, last error was " + itos(iErr);
						LOG(INFO) + m_szLastError;
					}
				}
			}
			closesocket(sockfd);
			sockfd = INVALID_SOCKET;
		}
	}
	delete[] szHost;
	freeaddrinfo(result);
	if (bConnectSuccessful)
	{
		if (m_NetworkState != NETWORK_CONNECTED)
		{
			m_NetworkState = NETWORK_CONNECTED;
			rho::Hashtable<rho::String, rho::String> detectedCallbackData;
			detectedCallbackData.put("connectionInformation", "Connected");
			detectedCallbackData.put("failureMessage", "Connected");
			m_pDetectCallback->set(detectedCallbackData);
		}
	}
	else
	{
		//  We are not connected
		if (m_NetworkState != NETWORK_DISCONNECTED)
		{
			m_NetworkState = NETWORK_DISCONNECTED;
			rho::Hashtable<rho::String, rho::String> detectedCallbackData;
			detectedCallbackData.put("connectionInformation", "Disconnected");
			detectedCallbackData.put("failureMessage", m_szLastError);
			m_pDetectCallback->set(detectedCallbackData);
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
void CNetworkDetection::run()
{
	CheckConnectivity();
	while( !isStopping() )    
    {
		wait(m_iNetworkPollInterval / 1000);
		if (!isStopping())
		{
			CheckConnectivity();
		}     
    }
	OutputDebugString(L"Stopping Network Detection Thread");
}

std::string CNetworkDetection::itos(int i)
{
	std::stringstream s;
	s << i;
	return s.str();
}

