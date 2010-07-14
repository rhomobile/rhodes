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

    CAutoPtr<IRhoClassFactory> ptrFactory = rho_impl_createClassFactory();
    CAutoPtr<IRhoThreadImpl> ptrThread = ptrFactory->createThreadImpl();

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
