#pragma once
#include "common/RhoThread.h"
#include "logging/RhoLog.h"
#include "INetworkDetect.h"
#if defined( OS_WP8 ) || defined( OS_WINCE ) || defined(OS_UWP) || defined(WINDOWS_PLATFORM)
#ifndef _HOST_TRACKER
#include "winsock2.h"
#endif
#endif


class CNetworkDetectionBase :  public rho::common::CRhoThread, public INetworkDetection
{
public:
	virtual void run();
	CNetworkDetectionBase();
	~CNetworkDetectionBase();

	/**
	*  Perform initialisation steps, memory allocation e.t.c.
	*  \return Whether or not the Network class successfully initialised.
	*/
	virtual bool Initialise();
	virtual bool SetHost(const rho::String& host);
	virtual void SetPort(int iPort) {m_iPort = iPort;}
	virtual bool SetNetworkPollInterval(int iInterval);
	virtual bool SetConnectionTimeout (int iTimeout);
	virtual bool StartNetworkChecking();
	virtual bool StopNetworkChecking();
	virtual bool IsChecking();
	virtual void SetCallback(rho::apiGenerator::CMethodResult pCallback);
	virtual rho::apiGenerator::CMethodResult GetCallback() {return m_pDetectCallback;}

protected:  //  Methods
	virtual bool CheckConnectivity() = 0;
    
    virtual void Startup() = 0;
    virtual void Cleanup() = 0;
    void CleanupAndDeleteSelf();
    
	std::string itos(int i);

protected:  //  Threads
	rho::String m_szLastError;

protected:  //  Attributes
	timeval			m_connectionTimeout;
	rho::apiGenerator::CMethodResult	m_pDetectCallback;
	rho::String m_szHost;
	int m_iPort;
	int m_iNetworkPollInterval;
	NetworkState m_NetworkState;
  bool m_deleteSelf;
};