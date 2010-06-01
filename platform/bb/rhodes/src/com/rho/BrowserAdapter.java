package com.rho;

import javax.microedition.io.HttpConnection;

import com.rho.net.URI;

import rhomobile.*;
import rhomobile.RhodesApplication.PrimaryResourceFetchThread;

import net.rim.device.api.browser.field.BrowserContent;
import net.rim.device.api.browser.field.Event;
import net.rim.device.api.browser.field.RedirectEvent;
import net.rim.device.api.browser.field.RenderingApplication;
import net.rim.device.api.browser.field.RenderingException;
import net.rim.device.api.browser.field.RenderingOptions;
import net.rim.device.api.browser.field.RenderingSession;
import net.rim.device.api.browser.field.RequestedResource;
import net.rim.device.api.browser.field.UrlRequestedEvent;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.system.Application;
import net.rim.device.api.system.Display;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.component.Status;

public class BrowserAdapter implements RenderingApplication, IBrowserAdapter  
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("BrowserAdapter");
    private RenderingSession _renderingSession;
    private RhoMainScreen m_oMainScreen;
    
    private RhodesApplication m_app;
	private HttpConnection m_connResource = null;
    private boolean m_bLoadImageAsync = false;
    
	public BrowserAdapter(RhoMainScreen oMainScreen, RhodesApplication app, boolean bLoadImageAsync) 
	{
		m_oMainScreen = oMainScreen;
		m_app = app;
		m_bLoadImageAsync = bLoadImageAsync;
		
        _renderingSession = RenderingSession.getNewInstance();
        
        // enable javascript
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.JAVASCRIPT_ENABLED, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.JAVASCRIPT_LOCATION_ENABLED, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ENABLE_CSS, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.DEFAULT_CHARSET_VALUE, "utf-8");
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.OVERWRITE_CHARSET_MODE, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ALLOW_POPUPS, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.USE_BACKGROUND_IMAGES, true);
        
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.VALUE_THRESHOLD, 100000);
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.USE_BACKGROUND_IMAGES, true);
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.SHOW_IMAGE_PLACEHOLDERS, false);
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ENABLE_WML, false);
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ENABLE_EMBEDDED_RICH_CONTENT, false);
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ENABLE_IMAGE_EDITING, false);
//        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.NO_SEARCH_MENU_MODE, true);
        
	}

	public void setFullBrowser()
    {
        //this is the undocumented option to tell the browser to use the 4.6 Rendering Engine
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, 17000, true);
    }
	
    public Object eventOccurred(Event event) {

        int eventId = event.getUID();

        switch (eventId) {

            case Event.EVENT_URL_REQUESTED : {

                UrlRequestedEvent urlRequestedEvent = (UrlRequestedEvent) event;
                String absoluteUrl = urlRequestedEvent.getURL();

                if ( urlRequestedEvent.getPostData() == null ||
                	 urlRequestedEvent.getPostData().length == 0 )
                	m_app.addToHistory(absoluteUrl, null );

                PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(absoluteUrl,
                                                                                   urlRequestedEvent.getHeaders(),
                                                                                   urlRequestedEvent.getPostData(),
                                                                                   event);
                thread.start();

                break;

            } case Event.EVENT_BROWSER_CONTENT_CHANGED: {

                // browser field title might have changed update title
                /*BrowserContentChangedEvent browserContentChangedEvent = (BrowserContentChangedEvent) event;

                if (browserContentChangedEvent.getSource() instanceof BrowserContent) {
                    BrowserContent browserField = (BrowserContent) browserContentChangedEvent.getSource();
                    String newTitle = browserField.getTitle();
                    if (newTitle != null) {
                        synchronized (getAppEventLock())
                    	//synchronized (Application.getEventLock())
                        {
                        	_mainScreen.setTitle(newTitle);
                        }
                    }
                }*/

                break;

            } case Event.EVENT_REDIRECT : {

                    RedirectEvent e = (RedirectEvent) event;
                    String referrer = e.getSourceURL();
                    String absoluteUrl = e.getLocation();

                    switch (e.getType()) {

                        case RedirectEvent.TYPE_SINGLE_FRAME_REDIRECT :
                            // show redirect message
                            Application.getApplication().invokeAndWait(new Runnable() {
                                public void run() {
                                    Status.show("You are being redirected to a different page...");
                                }
                            });

                            break;

                        case RedirectEvent.TYPE_JAVASCRIPT :
                            break;
                        case RedirectEvent.TYPE_META :
                            // MSIE and Mozilla don't send a Referer for META Refresh.
                            referrer = null;
                            break;
                        case RedirectEvent.TYPE_300_REDIRECT :
                            // MSIE, Mozilla, and Opera all send the original
                            // request's Referer as the Referer for the new
                            // request.

                        	m_app.addToHistory(absoluteUrl,referrer);
                            Object eventSource = e.getSource();
                            if (eventSource instanceof HttpConnection) {
                                referrer = ((HttpConnection)eventSource).getRequestProperty("referer");
                            }
                            break;

                    }

                    HttpHeaders requestHeaders = new HttpHeaders();
                    requestHeaders.setProperty("referer", referrer);
                    PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(absoluteUrl, requestHeaders,null, event);
                    thread.start();
                    break;

            } case Event.EVENT_CLOSE :
                // TODO: close the application
                break;

            case Event.EVENT_SET_HEADER :
            case Event.EVENT_SET_HTTP_COOKIE : {
            	/*String cookie = ((SetHttpCookieEvent)event).getCookie();
        		String response = processAjaxCall(cookie);
        		if (response != null)
        			synchronized (pendingResponses) {
        				pendingResponses.addElement(response);
        			}
        		response = null;
            	cookie = null;*/
            	break;
            }
            case Event.EVENT_HISTORY :           // no history support
            case Event.EVENT_EXECUTING_SCRIPT :  // no progress bar is supported
            case Event.EVENT_FULL_WINDOW :       // no full window support
            case Event.EVENT_STOP :              // no stop loading support
            default :
        }

        return null;
    }

    public void processConnection(HttpConnection connection, Object e) 
    {
        BrowserContent browserContent = null;

        try {
            browserContent = _renderingSession.getBrowserContent(connection, this, (Event)e);

            if (browserContent != null) {
       			browserContent.finishLoading();
                
                Field field = browserContent.getDisplayableContent();
                if (field != null) {
                	
                    synchronized (Application.getEventLock()) 
                    {
                    	m_oMainScreen.deleteAll();
                    	m_oMainScreen.add(field);
                    	
                    	/*if ( m_oMainScreen.getFieldCount() > 0 )
                    	{
	                    	Field old = m_oMainScreen.getField(0);
	                    	m_oMainScreen.add(field);
	                    	m_oMainScreen.delete(old);
                    	}else
                    		m_oMainScreen.add(field);*/
/*                        
                        _mainScreen.doPaint();
                        if ( e == null )
                        {//This should awake screen in case of long network response
	                        KeyCodeEvent inject1 = 	new KeyCodeEvent( KeyCodeEvent.KEY_DOWN, (char)Keypad.KEY_ESCAPE, 0);
	                        KeyCodeEvent inject2 =  new KeyCodeEvent( KeyCodeEvent.KEY_UP, (char)Keypad.KEY_ESCAPE, 0);
	                        inject1.post();
	                        inject2.post();
	                        m_bSkipKeyPress = true;
                        }*/
                    }
                }
                
            }
        } catch (RenderingException re) {
        	LOG.ERROR("RenderingException", re);
	    } finally {
	        SecondaryResourceFetchThread.doneAddingImages();
	    }
    }
    
	public int getAvailableHeight(BrowserContent browserContent) {
		return Display.getHeight();
	}

	public int getAvailableWidth(BrowserContent browserContent) {
		return Display.getWidth();
	}

	public String getHTTPCookie(String url) {
		return null;
	}

	public int getHistoryPosition(BrowserContent browserContent) {
		return 0;
	}

    public HttpConnection getResource( RequestedResource resource, BrowserContent referrer) {

        if (resource == null)
            return null;
        
        String url = resource.getUrl();
        if (url == null || url.endsWith("/favicon.ico"))
        	return null;
        
        try
        {
        	if (referrer == null || URI.isLocalData(url) || !m_bLoadImageAsync) 
        	{
	        	boolean bLocalHost = RhodesApp.getInstance().isRhodesAppUrl(url);
	        	if ( bLocalHost && m_connResource!= null)
	        	{
	        		com.rho.net.RhoConnection rhoConn = (com.rho.net.RhoConnection)((com.rho.net.bb.NativeBBHttpConnection)m_connResource).getNativeConnection(); 
	        		rhoConn.resetUrl(url);
	        		
	        		Utilities.makeConnection(url, resource.getRequestHeaders(), null, m_connResource);
	        		return m_connResource;
	        	}else
	        	{
		            HttpConnection connection = Utilities.makeConnection(url, resource.getRequestHeaders(), null, null);
		            if (bLocalHost)
		            	m_connResource = connection;
		            return connection;
	        	}
        	}else
        	{
        		SecondaryResourceFetchThread.enqueue(resource, referrer);        		
        	}
        }catch(Exception exc)
        {
        	LOG.ERROR("getResource failed.", exc);
        }
        
        return null;
        
/*
        // check if this is cache-only request
        if (resource.isCacheOnly()) {
            // no cache support
            return null;
        }

        String url = resource.getUrl();

        if (url == null) {
            return null;
        }

        try{
	        // if referrer is null we must return the connection
	        if (referrer == null) {
	            HttpConnection connection = Utilities.makeConnection(url, resource.getRequestHeaders(), null);
	            return connection;
	
	        } else 
	        {
	    		if ( URI.isLocalHost(url) || URI.isLocalData(url))
	    		{
	                HttpConnection connection = Utilities.makeConnection(url, resource.getRequestHeaders(), null);
	                return connection;
	   			}else
	   			{
		            // if referrer is provided we can set up the connection on a separate thread
		            SecondaryResourceFetchThread.enqueue(resource, referrer);
	   			}
	        }
        }catch(Exception exc)
        {
        	LOG.ERROR("getResource failed.", exc);
        }
        
        return null;*/
    }

    public void invokeRunnable(Runnable runnable) {
        (new Thread(runnable)).start();
    }

    public boolean navigationMovement(int dx, int dy, int status, int time)
    {
    	return false;
    }
    
    public void executeJavascript(String strJavascript)
    {
    	
    }
}
