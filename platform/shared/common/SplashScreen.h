/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
