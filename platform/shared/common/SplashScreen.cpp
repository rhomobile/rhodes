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

#include "SplashScreen.h"
#include "RhoThread.h"
#include "IRhoClassFactory.h"
#include "RhoConf.h"
#include "Tokenizer.h"

namespace rho {
namespace common {
IMPLEMENT_LOGCLASS(CSplashScreen,"SplashScreen");

void CSplashScreen::start()
{
	if (m_nDelay>0 && m_startTime.toULong() == 0)
        m_startTime = CTimeInterval::getCurrentTime();
}

long CSplashScreen::howLongWaitMs()
{
   if (m_nDelay <= 0) {
       return 0;
   }
   CTimeInterval endTime = CTimeInterval::getCurrentTime();
   long nTimeElapsed = endTime.minus(m_startTime).toULong();
   
   return m_nDelay * 1000 - nTimeElapsed;
}

void CSplashScreen::hide()
{
	if (m_nDelay<=0 || m_startTime.toULong() == 0)
		return;
	long nWaitMs = howLongWaitMs();
    m_startTime = CTimeInterval();
	if ( nWaitMs <= 0 )
		return;

    CAutoPtr<IRhoThreadImpl> ptrThread = rho_get_RhoClassFactory()->createThreadImpl();

    ptrThread->sleep(nWaitMs);
}

void CSplashScreen::init()
{
	String strSplash = RHOCONF().getString("splash_screen");
	
	CTokenizer stringtokenizer(strSplash, ";");
	while (stringtokenizer.hasMoreTokens()) {
		String tok = stringtokenizer.nextToken();
		tok = String_trim(tok);
		if (tok.length() == 0) {
			continue;
		}
		
		if (tok.find("delay") == 0)
		{
			int nEq = tok.find('=');
			if (nEq>=0)
			{
				String val = tok.substr(nEq+1);
				val = String_trim(val);
				if ( val.length() > 0 )
                    convertFromStringA( val.c_str(), m_nDelay );
			}
		}else if (tok.find("zoom") == 0)
		{
			m_nFlags |= VZOOM | HZOOM;
		}else if ( tok.find("vzoom") == 0)
		{
			m_nFlags |= VZOOM;
		}else if ( tok.find("hzoom") == 0)
		{
			m_nFlags |= HZOOM;
		}else if ( tok.find("center") == 0)
		{
			m_nFlags |= VCENTER | HCENTER;
		}else if ( tok.find("vcenter") == 0)
		{
			m_nFlags |= VCENTER;
		}else if ( tok.find("hcenter") == 0)
		{
			m_nFlags |= HCENTER;
		}
		
	}
}
}
}
