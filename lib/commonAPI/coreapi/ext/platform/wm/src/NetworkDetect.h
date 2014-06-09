#ifndef _NETWORKDETECT_H_
#define _NETWORKDETECT_H_
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "../../../shared/NetworkDetectBase.h"

/**
*  Class to wrap the Network functionality, to poll on a specified address to see if
*  we are able to connect to it.
*/
class CNetworkDetection :  public CNetworkDetectionBase
{
public:
	CNetworkDetection() {}
	~CNetworkDetection() {}
protected:
    virtual bool CheckConnectivity();

private:  //  Methods
	

    virtual void Startup();
    virtual void Cleanup();

};
#endif