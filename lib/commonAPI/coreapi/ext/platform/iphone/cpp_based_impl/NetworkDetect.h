#pragma once

#include "../../../shared/NetworkDetectBase.h"

class CNetworkDetection :  public CNetworkDetectionBase
{
public:
	CNetworkDetection() {}
	virtual ~CNetworkDetection() {}

private:  //  Methods
	virtual void CheckConnectivity();

    virtual void Startup();
    virtual void Cleanup();
};