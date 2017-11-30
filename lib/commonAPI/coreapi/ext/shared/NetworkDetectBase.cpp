#include "NetworkDetectBase.h"
#include <stdlib.h>

#define DEFAULT_NETWORK_POLL_INTERVAL 30000
#define DEFAULT_CONNECTION_TIMEOUT_SECONDS 1
#define DEFAULT_NETWORK_URL "www.symbol.com"
#define	DEFAULT_PORT 80
#define NETWORK_DETECT_MINIMUM_POLL_INTERVAL 5000

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
CNetworkDetectionBase::CNetworkDetectionBase()
{
	//  Initialise Variables
	m_szHost = DEFAULT_NETWORK_URL;
	m_iPort = DEFAULT_PORT;
	m_iNetworkPollInterval = DEFAULT_NETWORK_POLL_INTERVAL;
	m_connectionTimeout.tv_sec = DEFAULT_CONNECTION_TIMEOUT_SECONDS;
	m_connectionTimeout.tv_usec = 0;
	m_szLastError = "";
  m_deleteSelf = false;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
CNetworkDetectionBase::~CNetworkDetectionBase()
{
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
bool CNetworkDetectionBase::Initialise()
{
	Startup();

	return true;
}


bool CNetworkDetectionBase::SetHost(const rho::String& szHost)
{
	if (szHost.length() > 7 && szHost.compare(0, 7, "http://") == 0)
		m_szHost = szHost.substr(7);
	else if (szHost.length() > 8 && szHost.compare(0, 8, "https://") == 0)
		m_szHost = szHost.substr(8);
	else
		m_szHost = szHost;
	return true;
}

bool CNetworkDetectionBase::SetConnectionTimeout (int iTimeout)
{
	//  This is the Socket Connection timeout, specified in milliseconds
	int iSeconds = (iTimeout - (iTimeout % 1000)) / 1000;
	int iMicroseconds = (iTimeout - (iSeconds * 1000)) * 1000;

	m_connectionTimeout.tv_sec = (long)iSeconds;
	m_connectionTimeout.tv_usec = (long)iMicroseconds;

	return true;
}

bool CNetworkDetectionBase::SetNetworkPollInterval(int iInterval)
{
	if (iInterval >= NETWORK_DETECT_MINIMUM_POLL_INTERVAL)
	{
		m_iNetworkPollInterval = iInterval;
		if (isWaiting())
			stopWait();
		return true;
	}
	else
	{
		LOG(INFO) + "Network Poll Interval property ignored, value is too small, it must be more than " + NETWORK_DETECT_MINIMUM_POLL_INTERVAL;
		return false;
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
bool CNetworkDetectionBase::StartNetworkChecking()
{
	m_NetworkState = NETWORK_INITIALISING;
	start(epNormal);
	return true;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
bool CNetworkDetectionBase::StopNetworkChecking()
{
	//  Only stop the network checking if it is currently running
	stop(60000);
	return true;
}

bool CNetworkDetectionBase::IsChecking()
{
	return isAlive();
}

void CNetworkDetectionBase::SetCallback(rho::apiGenerator::CMethodResult pCallback)
{
	m_pDetectCallback = pCallback;
}

//////////////////////////
//  Private Threads		//
//////////////////////////

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
void CNetworkDetectionBase::run()
{
	CheckConnectivity();
	while( !isStopping() )    
    {
		wait(m_iNetworkPollInterval);
		if (!isStopping())
		{
			CheckConnectivity();
		}     
    }
	LOG(INFO) + "Stopping Network Detection Thread..";
  
  if ( m_deleteSelf )
  {
#if defined(OS_WINDOWS_DESKTOP)// || defined(OS_UWP)
    LOG(INFO) + "Stopping Network Detection Thread - do not delete ";
 #else
     LOG(INFO) + "Stopping Network Detection Thread - delete this";
    delete this;
 #endif
  }
}

std::string CNetworkDetectionBase::itos(int i)
{
    char buf[16];
    snprintf(buf,16,"%d",i);    
	return std::string(buf);
}

void CNetworkDetectionBase::CleanupAndDeleteSelf()
{
  m_deleteSelf = true;
  Cleanup();
}

