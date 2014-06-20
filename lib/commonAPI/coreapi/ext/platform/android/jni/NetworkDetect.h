#pragma once

#include "NetworkDetectBase.h"

class CNetworkDetection :  public CNetworkDetectionBase
{
public:
	CNetworkDetection() {}
	~CNetworkDetection() {}

private:  //  Methods
	virtual bool CheckConnectivity();

    virtual void Startup();
    virtual void Cleanup();
};