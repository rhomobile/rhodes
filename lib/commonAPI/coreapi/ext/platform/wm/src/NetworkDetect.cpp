#include "NetworkDetect.h"

INetworkDetection* NetworkDetectionFactory::createNetworkDetection() {
    return new CNetworkDetection();
}

void CNetworkDetection::Cleanup()
{
    stop(1000);

	WSACleanup();
}

void CNetworkDetection::Startup()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		August 2011 (Initial Creation)
*/
bool CNetworkDetection::CheckConnectivity()
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

//#if defined(OS_WINCE)
//                if (iConnectSuccess == SOCKET_ERROR)
//#else
				//  Because Socket is non blocking we expect it to return SOCKET_ERROR
				//  and WSAGetLastError() would be WSAEWOULDBLOCK
                if (!(iConnectSuccess == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK))
//#endif
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
    if (result != 0) 
    {
	    freeaddrinfo(result);
    }
	if (bConnectSuccessful)
	{
		if (m_NetworkState != NETWORK_CONNECTED)
		{
			m_NetworkState = NETWORK_CONNECTED;
			rho::Hashtable<rho::String, rho::String> detectedCallbackData;
			detectedCallbackData.put("connectionInformation", "Connected");
			detectedCallbackData.put("failureMessage", "Connected");
			m_pDetectCallback.set(detectedCallbackData);
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
			m_pDetectCallback.set(detectedCallbackData);
		}
	}
	return bConnectSuccessful;
}

