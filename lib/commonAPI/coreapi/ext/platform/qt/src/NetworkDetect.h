#pragma once

#include "NetworkDetectBase.h"

class CNetworkDetection :  public CNetworkDetectionBase
{
public:
	CNetworkDetection() {}
	~CNetworkDetection() {}

private:  //  Methods
	virtual void CheckConnectivity();

    virtual void Startup();
    virtual void Cleanup();
};