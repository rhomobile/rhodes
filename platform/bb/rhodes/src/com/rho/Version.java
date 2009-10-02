package com.rho;

import net.rim.device.api.system.ApplicationDescriptor;
import net.rim.device.api.system.ApplicationManager;

public class Version {
	
	private static String getSoftwareVersion() {
		ApplicationManager appMan = ApplicationManager.getApplicationManager();
		//grab the running applications
		ApplicationDescriptor[] appDes = appMan.getVisibleApplications();
	
		//check for the version of a standard
		//RIM app. I like to use the ribbon
		//app but you can check the version
		//of any RIM module as they will all
		//be the same.
		int size = appDes.length;
		String strVer = "0.0";
		for (int i = size-1; i>=0; --i){
			if ((appDes[i].getModuleName()).equals("net_rim_bb_ribbon_app")){
				strVer = appDes[i].getVersion();
				break;
			}
		}
	  
		return strVer; 
	}
    
    public static class SoftVersion{
		public int nMajor = 0;
		public int nMinor = 0;
		public int nMinor2 = 0;
    };
    
    private static SoftVersion m_softVer;
    
    public static SoftVersion getSoftVersion()
    {
    	if ( m_softVer != null )
    		return m_softVer;
    	
    	m_softVer = new SoftVersion();
		String strVer = getSoftwareVersion();//DeviceInfo.getPlatformVersion();
    	
		int nDot = strVer.indexOf('.');
		
		if ( nDot >= 0 )
		{
			m_softVer.nMajor = Integer.parseInt( strVer.substring(0, nDot) );
			
			int nDot2 = strVer.indexOf('.',nDot+1);
			if ( nDot2 >= 0 )
			{
				m_softVer.nMinor = Integer.parseInt( strVer.substring(nDot+1,nDot2) );
				
				int nDot3 = strVer.indexOf('.',nDot2+1);
				if ( nDot3 >= 0 )
					m_softVer.nMinor2 = Integer.parseInt( strVer.substring(nDot2+1,nDot3) );
			}
			else
				m_softVer.nMinor = Integer.parseInt( strVer.substring(nDot+1) );
		}else
			m_softVer.nMajor = Integer.parseInt( strVer );
		
	
		return m_softVer;
    }
}
