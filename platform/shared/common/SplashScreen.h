#pragma once

#include "logging/RhoLog.h"
#include "common/RhoTime.h"

namespace rho {
namespace common {

class CSplashScreen
{
    DEFINE_LOGCLASS;
	
    long m_nDelay;
    CTimeInterval m_startTime;
   	int m_nFlags;

public: 
    static const int NONE = 0, VZOOM = 1, HZOOM = 2, VCENTER = 4, HCENTER = 8;
   	
    CSplashScreen()
    {
  	    m_nDelay = 0;
   	    m_nFlags = NONE;
    }

   	boolean isFlag(int nFlag)
   	{
   		return (m_nFlags&nFlag) != 0;
   	}
	
   	void start();
  	void hide();
    void init();

    long howLongWaitMs();
   
};

}
}
