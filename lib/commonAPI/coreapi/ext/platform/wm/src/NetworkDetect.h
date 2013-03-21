#ifndef _NETWORKDETECT_H_
#define _NETWORKDETECT_H_
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "common/RhoThread.h"
#include "api_generator/MethodResult.h"
#include "logging/RhoLog.h"

enum NetworkState {NETWORK_INITIALISING, NETWORK_CONNECTED, NETWORK_DISCONNECTED};

/**
*  Class to wrap the Network functionality, to poll on a specified address to see if
*  we are able to connect to it.
*/
class CNetworkDetection :  public rho::common::CRhoThread
{
public:
	virtual void run();
	CNetworkDetection();
	~CNetworkDetection();

	/**
	*  Perform initialisation steps, memory allocation e.t.c.
	*  \return Whether or not the Network class successfully initialised.
	*/
	BOOL Initialise();
	BOOL SetHost(const rho::String& host);
	void SetPort(int iPort) {m_iPort = iPort;}
	BOOL SetNetworkPollInterval(int iInterval);
	BOOL SetConnectionTimeout (int iTimeout);
	BOOL StartNetworkChecking();
	BOOL StopNetworkChecking();
	BOOL IsChecking();
	void SetCallback(rho::apiGenerator::CMethodResult pCallback);
	rho::apiGenerator::CMethodResult GetCallback() {return m_pDetectCallback;}

private:  //  Methods
	void CheckConnectivity();
	std::string itos(int i);

private:  //  Threads
	rho::String m_szLastError;

private:  //  Attributes
	timeval			m_connectionTimeout;
	rho::apiGenerator::CMethodResult	m_pDetectCallback;
	rho::String m_szHost;
	int m_iPort;
	int m_iNetworkPollInterval;
	NetworkState m_NetworkState;
};
#endif