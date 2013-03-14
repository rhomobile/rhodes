/**
 *  \file Network.h
 *  \brief Network Plug-in workhorse Class.  
 *  
 */

//#include "../../common/Public/PBPlugin.h"
#ifndef _NETWORKDETECT_H_
#define _NETWORKDETECT_H_
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <Wininet.h>
#include "common/RhoThread.h"
#include "api_generator/MethodResult.h"
#include "logging/RhoLog.h"

static TCHAR* tcNetworkEventNames[] = 
{
	L"connectionInformation",
	NULL
};

enum NetworkState {NETWORK_INITIALISING, NETWORK_CONNECTED, NETWORK_DISCONNECTED};


/**
*  Class to wrap the Network functionality, to poll on a specified address to see if
*  we are able to connect to it.  Proxy settings are read from the configuration 
*  file.
*/
class CNetworkDetection :  public rho::common::CRhoThread
{
public:
	virtual void run();
	CNetworkDetection();
	
	/**
	*  Stop all threads, deinitialise all member variables, close all 
	*  handles and free memory allocated by the CTelemetry object.
	*/ 
	~CNetworkDetection();

	/**
	*  Perform initialisation steps, memory allocation e.t.c.
	*  \return Whether or not the Network class successfully initialised.
	*/
	BOOL Initialise();

	/**
	*  Specify the Navigation string.
	*  \param szNavURI The Navigation String to be sent when invoking 
	*  SendPBNavigate
	*  \return Whether or not the Navigation URI was successfully set.
	*/
	BOOL SetNetworkEventURI(LPCWSTR szNavURI);

	BOOL SetHost(const rho::String& host);

	BOOL SetPort(int iPort);

	BOOL SetNetworkPollInterval(int iInterval);

	BOOL SetConnectionTimeout (int iTimeout);

	BOOL StartNetworkChecking();

	BOOL StopNetworkChecking();

	BOOL IsChecking();

	void SetCallback(rho::apiGenerator::CMethodResult* pCallback);
	rho::apiGenerator::CMethodResult* GetCallback() {return m_pDetectCallback;}

private:  //  Methods
	void CheckConnectivity();
	std::string itos(int i);

private:  //  Threads

	/**
	*  Thread which runs to notify the calling application of the value of 
	*  registered filters every x milliseconds, 
	*  where x is specified using setFilterQueryFrequency.
	*  \param lpParam Pointer to the Telemetry object associated with this 
	*  thread.
	*  \return 0 when the Thread Exits
	*/
	//static DWORD NetworkChecker (LPVOID lpParam);
	rho::String m_szLastError;

private:  //  Attributes
	/// The URI To Navigate the PB Browser to
	//TCHAR m_tcNetworkEventURI[MAXURL];
	/// Handle to Event signaled to indicate the class is being destroyed.
//	HANDLE	m_hStopNetworkCheckerEvent;	
//	HANDLE	m_hNetworkChecker;
//	HANDLE	m_hCheckFrequencyChangedEvent;
	/// Ensures Navigates are not sent once the class has been told to shut down
//	BOOL	m_bShuttingDown;			
	/// Pointer to PBModule associated with this Telemetry object, used to send navigates and logs to the core
	//PBModule*		m_pModule;			
	timeval			m_connectionTimeout;
	rho::apiGenerator::CMethodResult*	m_pDetectCallback;


	rho::String m_szHost;
	TCHAR* m_tcProxy;
	int m_iPort;
	int m_iNetworkPollInterval;
	NetworkState m_NetworkState;

};
#endif