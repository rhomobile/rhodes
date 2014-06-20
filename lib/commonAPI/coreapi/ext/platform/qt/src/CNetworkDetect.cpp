#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>

#include "NetworkDetect.h"

INetworkDetection* NetworkDetectionFactory::createNetworkDetection() {
    return new CNetworkDetection();
}

void CNetworkDetection::Cleanup()
{
	stop(1000);
    
}

void CNetworkDetection::Startup()
{
}

bool CNetworkDetection::CheckConnectivity()
{
    bool bConnectSuccessful = false;
    
    struct addrinfo hints, *result = NULL, *ptr = NULL;
	int sockfd = -1;
    memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	char szPortAsString[5 + 1];
    snprintf(szPortAsString,5,"%d",m_iPort);
	char* szHost = new char[m_szHost.length() + 1];
	memset(szHost, 0, m_szHost.length() + 1);
	strcpy(szHost, m_szHost.c_str());
	int iResult = getaddrinfo(szHost, szPortAsString, &hints, &result);
	if (iResult != 0)
    {
		//  Log the Fact that we can't get the addr info
        int iErr = errno;
		m_szLastError = "Attempted to resolve hostname to connect to but did not succeed, return value (" + itos(iResult) +
        "), last error (" + itos(iErr) + ")";
		LOG(INFO) + m_szLastError;
	}
	else
	{
		ptr=result;
		sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        
		if (sockfd == -1)
		{
            int iErr = errno;
			m_szLastError = "Unable to create communications socket, last error was " + itos(iErr);
			LOG(INFO) + m_szLastError;
		}
		else
		{
			int iNonBlockRes = fcntl(sockfd, F_SETFL, O_NONBLOCK);
			if (iNonBlockRes == -1)
			{
				m_szLastError = "Error setting socket into Non Blocking mode";
				LOG(INFO) + m_szLastError;
			}
			else
			{
				int iConnectSuccess = connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);
				if (!(iConnectSuccess == -1 && errno == EINPROGRESS))
				{
					m_szLastError = "Socket Operation unexpectedly blocked, are you connected to a PC?";
					LOG(WARNING) + m_szLastError;
				}
				else
				{
					fd_set WriteFDs;
					FD_ZERO(&WriteFDs);
					FD_SET(sockfd, &WriteFDs);
                    timeval to = m_connectionTimeout;
					int iSelectReturnVal = select(sockfd+1, 0, &WriteFDs, 0, &to);
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
                        int iErr = errno;
						m_szLastError = "Unable to connect to specified host, last error was " + itos(iErr);
						LOG(INFO) + m_szLastError;
					}
				}
			}
			close(sockfd);
			sockfd = -1;
		}
	}
	delete[] szHost;
    if (result != 0) {
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

