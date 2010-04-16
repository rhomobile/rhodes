package com.rho;

import com.rho.net.*;
import com.xruby.runtime.lang.RubyValue;
import java.util.Vector;

public class RhodesApp
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhodesApp");
	RhoConf RHOCONF(){ return RhoConf.getInstance(); }
	
	static RhodesApp m_pInstance;
	private String m_strRhoRootPath;
    Vector/*<unsigned long>*/ m_arCallbackObjects = new Vector();
    private SplashScreen m_oSplashScreen = new SplashScreen();

    NetRequest getNet() { return RhoClassFactory.createNetRequest();}
    
    public static RhodesApp Create(String strRootPath)
    {
        if ( m_pInstance != null ) 
            return m_pInstance;

        m_pInstance = new RhodesApp(strRootPath);
        return m_pInstance;
    }
    
    public static void Destroy()
    {
    	
    }
    
    public static RhodesApp getInstance(){ return m_pInstance; }
    public SplashScreen getSplashScreen(){return m_oSplashScreen;}
	
    RhodesApp(String strRootPath)
    {
        m_strRhoRootPath = strRootPath;
        
        getSplashScreen().init();
    }

    
    public String addCallbackObject(RubyValue valObject, String strName)
    {
        int nIndex = -1;
        for (int i = 0; i < (int)m_arCallbackObjects.size(); i++)
        {
            if ( m_arCallbackObjects.elementAt(i) == null )
                nIndex = i;
        }
        if ( nIndex  == -1 )
        {
            m_arCallbackObjects.addElement(valObject);
            nIndex = m_arCallbackObjects.size()-1;
        }else
            m_arCallbackObjects.setElementAt(valObject,nIndex);

        String strRes = "__rho_object[" + strName + "]=" + nIndex;

        return strRes;
    }

    public RubyValue getCallbackObject(int nIndex)
    {
        if ( nIndex < 0 || nIndex > m_arCallbackObjects.size() )
            return null;

        RubyValue res = (RubyValue)m_arCallbackObjects.elementAt(nIndex);
        m_arCallbackObjects.setElementAt(null,nIndex);        
        return res;
    }

    String canonicalizeRhoUrl(String strUrl)throws Exception 
    {
    	return getNet().resolveUrl(strUrl);    	
    }
 
    public void callPopupCallback(String strCallbackUrl, String id, String title)throws Exception
    {
        if ( strCallbackUrl == null || strCallbackUrl.length() == 0 )
            return;
    	
        strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
        String strBody = "button_id=" + id + "&button_title=" + title;
        strBody += "&rho_callback=1";
        getNet().pushData( strCallbackUrl, strBody, null );
    }    
}