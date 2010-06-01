package com.rho;

import net.rim.device.api.browser.field.RenderingOptions;
import net.rim.device.api.browser.field2.*;
import net.rim.device.api.system.Application;

import javax.microedition.io.HttpConnection;
import javax.microedition.io.InputConnection;

import com.rho.RhodesApp;
import com.rho.net.URI;

import rhomobile.Utilities;
import rhomobile.RhodesApplication.PrimaryResourceFetchThread;
import rhomobile.RhodesApplication;

//http://rim.lithium.com/t5/Java-Development/Browser-events-on-JDK-5-field2-package/m-p/430991
//http://208.74.204.192/t5/Web-Development/Embedded-Browser-Ajax-support/m-p/410260;jsessionid=CDE85D9F4D88217EA118DE3F5DF9FEFD
//http://supportforums.blackberry.com/t5/Web-Development/JavaScript-not-working-on-Black-Berry-Simulator-8330/m-p/412687#M3397

public class BrowserAdapter5 implements IBrowserAdapter 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("BrowserAdapter");
    private RhoMainScreen m_oMainScreen;
    private BrowserField m_oBrowserField;
    private BrowserFieldConfig m_oConfig;
    
    private RhodesApplication m_app;
 
    private class RhoProtocolController extends ProtocolController
    {
		public RhoProtocolController(BrowserField browserField) {
			super(browserField);
		}

		public void handleNavigationRequest(BrowserFieldRequest request)
				throws Exception 
		{
			String url = request.getURL();

            if ( request.getPostData() == null ||
            		request.getPostData().length == 0 )
            {
            	//String referrer = request.getHeaders().getPropertyValue("referer");
               	m_app.addToHistory(url, null );
            }
            
			PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(
				url, request.getHeaders(),request.getPostData(),url);
			thread.start();
		}

		public InputConnection handleResourceRequest(BrowserFieldRequest request)
				throws Exception 
		{
			String url = request.getURL();
    		if ( RhodesApp.getInstance().isRhodesAppUrl(url) || URI.isLocalData(url))
    		{
                HttpConnection connection = Utilities.makeConnection(url, request.getHeaders(), null, null);
                
                return connection;
    		}else
    			return super.handleResourceRequest(request);
		}

		public void setNavigationRequestHandler(String protocol,
				BrowserFieldNavigationRequestHandler handler) {
			super.setNavigationRequestHandler(protocol, handler);
		}

		public void setResourceRequestHandler(String protocol,
				BrowserFieldResourceRequestHandler handler) {
			super.setResourceRequestHandler(protocol, handler);
		}
    	
    };
    private RhoProtocolController m_oController;
    
	public BrowserAdapter5(RhoMainScreen oMainScreen, RhodesApplication app) 
	{
		m_oMainScreen = oMainScreen;
		m_app = app;
		
		m_oConfig = new BrowserFieldConfig();
		m_oConfig.setProperty( BrowserFieldConfig.NAVIGATION_MODE, BrowserFieldConfig.NAVIGATION_MODE_POINTER );
		m_oConfig.setProperty( BrowserFieldConfig.JAVASCRIPT_ENABLED, Boolean.TRUE );
//		m_oConfig.setProperty( BrowserFieldConfig.NAVIGATION_MODE, BrowserFieldConfig.NAVIGATION_MODE_CARET );
		m_oConfig.setProperty( BrowserFieldConfig.ENABLE_COOKIES, Boolean.TRUE );
	}

	public void setFullBrowser()
	{
	}
	
	private void createBrowserField()
	{
		m_oBrowserField = new BrowserField(m_oConfig);
		m_oBrowserField.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ALLOW_POPUPS, true);
		m_oController = new RhoProtocolController(m_oBrowserField);
		m_oConfig.setProperty(BrowserFieldConfig.CONTROLLER, m_oController);
		
		BrowserFieldListener _listener = new BrowserFieldListener() 
		{ 
		     public void documentLoaded(BrowserField browserField, 
		    		 org.w3c.dom.Document document) throws Exception 
		     {
		    	synchronized (Application.getEventLock())
		    	{
		    		m_oMainScreen.deleteAll();
					m_oMainScreen.add(m_oBrowserField);
		     	}
		     }
		     /*
		     public void downloadProgress(BrowserField browserField, 
		    		 net.rim.device.api.browser.field.ContentReadEvent event)throws Exception
		     {
		        //Add your code here.
		     }*/
		};
		m_oBrowserField.addListener( _listener );
	}
	
    public void processConnection(HttpConnection connection, Object e) 
    {
        synchronized (Application.getEventLock()) 
        {
        	createBrowserField();        	
        	m_oBrowserField.displayContent(connection,  (e != null ? (String)e : "") );
        }
    }

    public void executeJavascript(String strJavascript)
    {
        //synchronized (Application.getEventLock()) 
        {
        	BrowserField field = (BrowserField)m_oMainScreen.getField(0);
        	field.executeScript(strJavascript);
        }
    }
    
    //http://supportforums.blackberry.com/t5/Java-Development/Tab-Navigation/td-p/381156
    public boolean navigationMovement(int dx, int dy, int status, int time)
    {
    	//TODO: try to change navigation
    	/*
    	Field focField = m_oBrowserField.getLeafFieldWithFocus();
    	Manager focManager = focField != null ? focField.getManager() : null;
    	boolean bHorizontal = false;
    	if ( focManager != null && focManager instanceof HorizontalFieldManager)
    		bHorizontal = true;
    	*/	
    	return false;
    }
}
