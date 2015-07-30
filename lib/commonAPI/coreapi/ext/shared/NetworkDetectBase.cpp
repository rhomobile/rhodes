#include "NetworkDetectBase.h"
#include <stdlib.h>

#define DEFAULT_NETWORK_POLL_INTERVAL 30000
#define DEFAULT_CONNECTION_TIMEOUT_SECONDS 1
#define DEFAULT_NETWORK_URL "www.motorolasolutions.com"
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
#if defined(RHODES_QT_PLATFORM) && defined(OS_WINDOWS_DESKTOP)
HANDLE g_hThread =NULL;
HANDLE g_hStopEvent = NULL;

DWORD WINAPI runProc(LPVOID detectionObjPtr)
{

	CNetworkDetectionBase* ptr = (CNetworkDetectionBase*)detectionObjPtr;

	while( 1 )    
	{
		if(ptr->CheckConnectivity())
		{
			break;
		}
		DWORD dwRes = ::WaitForSingleObject( g_hStopEvent, ptr->getPollInterval() );
		if(dwRes == WAIT_OBJECT_0)
		{
			break;
		}		

	}
	return 0;
}
#endif

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
bool CNetworkDetectionBase::StartNetworkChecking()
{
	m_NetworkState = NETWORK_INITIALISING;
#if defined(RHODES_QT_PLATFORM) && defined(OS_WINDOWS_DESKTOP)
	
	g_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hThread = ::CreateThread(NULL, 0, runProc, this, 0, NULL);

#else
	start(epNormal);
#endif	

	return true;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		May 2011 (Initial Creation)
*/
bool CNetworkDetectionBase::StopNetworkChecking()
{
	//  Only stop the network checking if it is currently running

#if defined(RHODES_QT_PLATFORM) && defined(OS_WINDOWS_DESKTOP)
	if(g_hStopEvent && g_hThread)
	{
		::SetEvent(g_hStopEvent);
		::WaitForSingleObject( g_hThread, INFINITE );
		CloseHandle(g_hThread);
		CloseHandle(g_hStopEvent);
	}
	g_hThread = NULL;
	g_hStopEvent = NULL;

#else
	stop(60000);
#endif


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
	LOG(INFO) + "Stopping Network Detection Thread";
  
  if ( m_deleteSelf )
  {
    delete this;
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

