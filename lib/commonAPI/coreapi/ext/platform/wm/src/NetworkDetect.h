#ifndef _NETWORKDETECT_H_
#define _NETWORKDETECT_H_
#pragma once
#ifndef _HOST_TRACKER
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
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
	virtual void Cleanup();

private:  //  Methods
	

    virtual void Startup();
    

};
#endif