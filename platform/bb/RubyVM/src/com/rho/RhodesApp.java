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
	
    private String m_strRhoRootPath, m_strBlobsDirPath, m_strDBDirPath;
	private String m_strStartUrl, m_strHomeUrl;
	private String m_strAppBackUrl = "", m_strAppBackUrlOrig = "";
	
    Vector/*<unsigned long>*/ m_arCallbackObjects = new Vector();
    private SplashScreen m_oSplashScreen = new SplashScreen();
    
    Mutex m_mxPushCallback = new Mutex();
    String m_strPushCallback = "", m_strPushCallbackParams = "";

	Mutex m_mxScreenRotationCallback = new Mutex();
    String m_strScreenRotationCallback = "", m_strScreenRotationCallbackParams = "";
    
    int m_currentTabIndex = 0;
    String[] m_currentUrls = new String[5];
    
    String getAppBackUrl(){return m_strAppBackUrl;}

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
	
    public String getBlobsDirPath(){return m_strBlobsDirPath; }
    public String getDBDirPath(){return m_strDBDirPath; }
    public String getRhoRootPath(){return m_strRhoRootPath;}
    
    NetRequest getNet() { return RhoClassFactory.createNetRequest();}
    public String getHomeUrl(){ return m_strHomeUrl; }
    
    RhodesApp(String strRootPath)
    {
        m_strRhoRootPath = strRootPath;
        
        getSplashScreen().init();
        
        initAppUrls();        
    }

    void initAppUrls() 
    {
    	m_strHomeUrl = "http://localhost:2375";
        m_strBlobsDirPath = getRhoRootPath() + "db/db-files";
    	m_strDBDirPath = getRhoRootPath() + "db";
    }
    
    public String resolveDBFilesPath(String strFilePath)
    {
        if ( strFilePath.startsWith(getRhoRootPath()) )
            return strFilePath;

        return FilePath.join(getRhoRootPath(), strFilePath);
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
    
    boolean isExternalUrl(String strUrl)
    {
    	return strUrl.startsWith("http://") || strUrl.startsWith("https://") ||
    		strUrl.startsWith("javascript:") || strUrl.startsWith("mailto:")
    		 || strUrl.startsWith("tel:")|| strUrl.startsWith("wtai:");    
    }
    
    public String canonicalizeRhoUrl(String url) 
    {
		if ( url == null || url.length() == 0 )
			return getHomeUrl();

		String strUrl = new String(url);
		strUrl.replace('\\', '/');
		if ( !strUrl.startsWith(getHomeUrl()) && !isExternalUrl(strUrl) )
			strUrl = FilePath.join(getHomeUrl(), strUrl);
		
		return strUrl;
    }
    
    public boolean isRhodesAppUrl(String url)
    {
    	return url.startsWith(getHomeUrl()); 
    }
    
    public void setPushNotification(String strUrl, String strParams )throws Exception
    {
        synchronized(m_mxPushCallback)
        {
            m_strPushCallback = canonicalizeRhoUrl(strUrl);
            m_strPushCallbackParams = strParams;
        }
    }

    public boolean callPushCallback(String strData)throws Exception
    {
        synchronized(m_mxPushCallback)
        {
            if ( m_strPushCallback.length() == 0 )
                return false;
        	
            String strBody = "status=ok&message=";
            strBody += URI.urlEncode(strData);
            strBody += "&rho_callback=1";
            if ( m_strPushCallbackParams.length() > 0 )
                strBody += "&" + m_strPushCallbackParams;

            NetResponse resp = getNet().pushData( m_strPushCallback, strBody, null );
            if (!resp.isOK())
                LOG.ERROR("Push notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
            else
            {
                String szData = resp.getCharData();
                return !(szData != null && szData.equals("rho_push"));
            }
        }

        return true;
    }
 
    public void setScreenRotationNotification(String strUrl, String strParams)throws Exception
    {
        synchronized(m_mxScreenRotationCallback)
        {
            m_strScreenRotationCallback = canonicalizeRhoUrl(strUrl);
            m_strScreenRotationCallbackParams = strParams;
        }
    }

    public void callScreenRotationCallback(int width, int height, int degrees)throws Exception
    {
    	synchronized(m_mxScreenRotationCallback) 
    	{
    		if (m_strScreenRotationCallback.length() == 0)
    			return;
    		
    		String strBody = "rho_callback=1";
    		
            strBody += "&width=";   strBody += width;
    		strBody += "&height=";  strBody += height;
    		strBody += "&degrees="; strBody += degrees;
    		
            if ( m_strScreenRotationCallbackParams.length() > 0 )
                strBody += "&" + m_strPushCallbackParams;
    			
            NetResponse resp = getNet().pushData( m_strScreenRotationCallback, strBody, null );
            if (!resp.isOK()) {
                LOG.ERROR("Screen rotation notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData());
            }
        }
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
 
    public String getCurrentUrl(int index)
    { 
        return m_currentUrls[m_currentTabIndex]; 
    }
    
    public void keepLastVisitedUrl(String strUrl)
    {
        //LOG(INFO) + "Current URL: " + strUrl;

    	try{
	        m_currentUrls[m_currentTabIndex] = canonicalizeRhoUrl(strUrl);
	
	        if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") )
	        {
	            if ( strUrl.startsWith(m_strHomeUrl) )
	                strUrl = strUrl.substring(m_strHomeUrl.length());
	
	            int nFragment = strUrl.indexOf('#');
	            if ( nFragment >= 0 )
	                strUrl = strUrl.substring(0, nFragment);
	
	            RHOCONF().setString("LastVisitedPage",strUrl);		
	            RHOCONF().saveToFile();
	        }
    	}catch(Exception exc)
    	{
    		LOG.ERROR("Save current location failed.", exc);
    	}
	}
    
    String getStartUrl()
    {
        m_strStartUrl = canonicalizeRhoUrl( RHOCONF().getString("start_path") );
        return m_strStartUrl;
    }
    
    void navigateToUrl(String url)throws Exception
    {
    	IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
    	helper.navigateUrl(url);
    }
    
    public void loadUrl(String url)throws Exception
    {
        boolean callback = false;
        if (url.startsWith("callback:") )
        {
            callback = true;
            url = url.substring(9);
        }
        url = canonicalizeRhoUrl(url);
        if (callback)
        {
            getNet().pushData( url,  "rho_callback=1", null );
        }
        else
            navigateToUrl(url);
    }
    
    public void setAppBackUrl(String url)
    {
        if ( url != null && url.length() > 0 )
        {
            m_strAppBackUrlOrig = url;
            m_strAppBackUrl = canonicalizeRhoUrl(url);
        }
        else
        {
            m_strAppBackUrlOrig = "";
            m_strAppBackUrl = "";
        }
    }
    
    public boolean isCloseBack()
    {
    	return m_strAppBackUrlOrig.equalsIgnoreCase("close");
    }
    
    public void navigateBack()
    {
    	try{
	        if ( m_strAppBackUrlOrig.length() > 0 )
	            loadUrl(m_strAppBackUrlOrig);
	        else if ( !getCurrentUrl(0).equalsIgnoreCase(getStartUrl()) )
	        {
	        	IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
	        	helper.navigateBack();
	        }
    	}catch(Exception exc)
    	{
    		LOG.ERROR("Navigate back failed.", exc);
    	}
    }
    
}