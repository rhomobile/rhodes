package com.rho;

import com.xruby.runtime.lang.RubyValue;
import java.util.Vector;

public class SplashScreen
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("SplashScreen");
	RhoConf RHOCONF(){ return RhoConf.getInstance(); }
	
	private long m_nDelay = 0;
	private TimeInterval m_startTime = new TimeInterval();
   	public final static int NONE = 0, VZOOM = 1, HZOOM = 2, VCENTER = 4, HCENTER = 8;
   	private int m_nFlags = NONE;
   	
   	public boolean isFlag(int nFlag)
   	{
   		return (m_nFlags&nFlag) != 0;
   	}
	
   	public void start()
   	{
   		if (m_nDelay>0 && m_startTime.toULong() == 0)
   		    m_startTime = TimeInterval.getCurrentTime();
   	}
   	
   	private long howLongWaitMs()
   	{
	   TimeInterval endTime = TimeInterval.getCurrentTime();
	   long nTimeElapsed = endTime.minus(m_startTime).toULong();
	   
	   return m_nDelay * 1000 - nTimeElapsed;
   	}
   	
   	public void hide()
   	{
   		if (m_nDelay<=0 || m_startTime.toULong() == 0)
   			return;
   		
   		long nWaitMs = howLongWaitMs();
   		m_startTime = new TimeInterval();
   		if ( nWaitMs <= 0 )
   			return;
   			
		synchronized (this) {
			try{
				this.wait(nWaitMs);
			}catch(Exception e)
			{
				LOG.ERROR("wait failed", e);
			}
		}
   	}
    
    public void init()
    {
    	String strSplash = RHOCONF().getString("splash_screen");
    	
		Tokenizer stringtokenizer = new Tokenizer(strSplash, ";");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}
			
			if (tok.indexOf("delay") == 0)
			{
				int nEq = tok.indexOf('=');
				if (nEq>=0)
				{
					String val = tok.substring(nEq+1);
					val.trim();
					if ( val.length() > 0 )
						m_nDelay = Integer.parseInt(val);
				}
			}else if (tok.indexOf("zoom") == 0)
			{
				m_nFlags |= SplashScreen.VZOOM | SplashScreen.HZOOM;
			}else if ( tok.indexOf("vzoom") == 0)
			{
				m_nFlags |= SplashScreen.VZOOM;
			}else if ( tok.indexOf("hzoom") == 0)
			{
				m_nFlags |= SplashScreen.HZOOM;
			}else if ( tok.indexOf("center") == 0)
			{
				m_nFlags |= SplashScreen.VCENTER | SplashScreen.HCENTER;
			}else if ( tok.indexOf("vcenter") == 0)
			{
				m_nFlags |= SplashScreen.VCENTER;
			}else if ( tok.indexOf("hcenter") == 0)
			{
				m_nFlags |= SplashScreen.HCENTER;
			}
			
		}
		
    }
    
}