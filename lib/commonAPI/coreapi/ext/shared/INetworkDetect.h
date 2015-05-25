#pragma once

#include "api_generator/MethodResult.h"

enum NetworkState {NETWORK_INITIALISING, NETWORK_CONNECTED, NETWORK_DISCONNECTED};

/**
*  Interface to wrap the Network functionality, to poll on a specified address to see if
*  we are able to connect to it.
*/
class INetworkDetection
{
public:
	virtual ~INetworkDetection() {}

	/**
	*  Perform initialisation steps, memory allocation e.t.c.
	*  \return Whether or not the Network class successfully initialised.
	*/
	virtual bool Initialise() = 0;
    virtual void Cleanup() = 0;
        virtual void CleanupAndDeleteSelf() = 0;
	virtual bool SetHost(const rho::String& host) = 0;
	virtual void SetPort(int iPort) = 0;
	virtual bool SetNetworkPollInterval(int iInterval) = 0;
	virtual bool SetConnectionTimeout (int iTimeout) = 0;
	virtual bool StartNetworkChecking() = 0;
	virtual bool StopNetworkChecking() = 0;
	virtual bool IsChecking() = 0;
	virtual void SetCallback(rho::apiGenerator::CMethodResult pCallback) = 0;
	virtual rho::apiGenerator::CMethodResult GetCallback() = 0;
};

class NetworkDetectionFactory {
public:
    //implement for concrete platforms
    static INetworkDetection* createNetworkDetection();
};
