package rhomobile;

import java.io.IOException;

import javax.microedition.io.HttpConnection;

//import org.garret.perst.Storage;
//import org.garret.perst.StorageFactory;

import net.rim.device.api.browser.field.*;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.system.Application;
import net.rim.device.api.ui.*;
import net.rim.device.api.ui.component.Status;
import net.rim.device.api.ui.container.MainScreen;
import com.rho.NetworkAccess;

/**
 * 
 */
final public class RhodesApplication extends UiApplication implements RenderingApplication {
    
    private static final String REFERER = "referer";   
    
    private RenderingSession _renderingSession;   

    private MainScreen _mainScreen;
    
    private HttpConnection  _currentConnection;
    
    /***************************************************************************
     * Main.
     **************************************************************************/
    public static void main(String[] args) {
    	NetworkAccess.autoConfigure();

    	//Storage storage = StorageFactory.getInstance().createStorage();
       	//storage.open("MyTest.dbs");
       	//storage.close();
       	
        RhoRuby.RhoRubyStart("");
    	//String[] args = new String[0];
		//com.xruby.runtime.lang.RubyRuntime.init(args);
		
        RhodesApplication app = new RhodesApplication();
        app.enterEventDispatcher();
        
        //com.xruby.runtime.lang.RubyRuntime.fini();
        
        RhoRuby.RhoRubyStop();
    }
    
    private RhodesApplication() {               
        _mainScreen = new MainScreen();        
        pushScreen(_mainScreen);
        _renderingSession = RenderingSession.getNewInstance();
        
        // enable javascript
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.JAVASCRIPT_ENABLED, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.JAVASCRIPT_LOCATION_ENABLED, true);                        
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ENABLE_CSS, true);                        
        
        PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread("http://localhost:8080", null, null, null, this);
        thread.start();                       
    }
          
    public void processConnection(HttpConnection connection, Event e) {
         
        // cancel previous request
        if (_currentConnection != null) {
            try {
                _currentConnection.close();
            } catch (IOException e1) {
            }
        }
        
        _currentConnection = connection;
        
        BrowserContent browserContent = null;
        
        try {
            browserContent = _renderingSession.getBrowserContent(connection, this, e);
            
            if (browserContent != null) {
                
                Field field = browserContent.getDisplayableContent();
                
                if (field != null) {
                    synchronized (Application.getEventLock()) {
                        _mainScreen.deleteAll();
                        _mainScreen.add(field);
                    }
                }
                
                browserContent.finishLoading();
            }
                                                         
        } catch (RenderingException re) {

        } finally {
            SecondaryResourceFetchThread.doneAddingImages();
        }
        
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#eventOccurred(net.rim.device.api.browser.Event)
     */
    public Object eventOccurred(Event event) {

        int eventId = event.getUID();

        switch (eventId) {

            case Event.EVENT_URL_REQUESTED : {

                UrlRequestedEvent urlRequestedEvent = (UrlRequestedEvent) event;    
                String absoluteUrl = urlRequestedEvent.getURL();
    
                HttpConnection conn = null;
                PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(urlRequestedEvent.getURL(),
                                                                                         urlRequestedEvent.getHeaders(), 
                                                                                         urlRequestedEvent.getPostData(),
                                                                                         event, this);
                thread.start();
    
                break;

            } case Event.EVENT_BROWSER_CONTENT_CHANGED: {                
                    
                // browser field title might have changed update title
                BrowserContentChangedEvent browserContentChangedEvent = (BrowserContentChangedEvent) event; 
            
                if (browserContentChangedEvent.getSource() instanceof BrowserContent) { 
                    BrowserContent browserField = (BrowserContent) browserContentChangedEvent.getSource(); 
                    String newTitle = browserField.getTitle();
                    if (newTitle != null) {
                        _mainScreen.setTitle(newTitle);
                    }
                }                   

                break;                

            } case Event.EVENT_REDIRECT : {

                    RedirectEvent e = (RedirectEvent) event;
                    String referrer = e.getSourceURL();
                                        
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
                            Object eventSource = e.getSource();
                            if (eventSource instanceof HttpConnection) {
                                referrer = ((HttpConnection)eventSource).getRequestProperty(REFERER);
                            }
                            break;

                    }
                    
                    HttpHeaders requestHeaders = new HttpHeaders();
                    requestHeaders.setProperty(REFERER, referrer);
                    PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(e.getLocation(), requestHeaders,null, event, this);
                    thread.start();
                    break;

            } case Event.EVENT_CLOSE :
                // TODO: close the appication
                break;
            
            case Event.EVENT_SET_HEADER :        // no cache support
            case Event.EVENT_SET_HTTP_COOKIE :   // no cookie support
            case Event.EVENT_HISTORY :           // no history support            
            case Event.EVENT_EXECUTING_SCRIPT :  // no progress bar is supported
            case Event.EVENT_FULL_WINDOW :       // no full window support
            case Event.EVENT_STOP :              // no stop loading support
            default :
        }

        return null;
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#getAvailableHeight(net.rim.device.api.browser.BrowserContent)
     */
    public int getAvailableHeight(BrowserContent browserField) {
        // field has full screen
        return Graphics.getScreenHeight();
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#getAvailableWidth(net.rim.device.api.browser.BrowserContent)
     */
    public int getAvailableWidth(BrowserContent browserField) {
        // field has full screen
        return Graphics.getScreenWidth();
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#getHistoryPosition(net.rim.device.api.browser.BrowserContent)
     */
    public int getHistoryPosition(BrowserContent browserField) {
        // no history support
        return 0;
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#getHTTPCookie(java.lang.String)
     */
    public String getHTTPCookie(String url) {
        // no cookie support
        return null;
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#getResource(net.rim.device.api.browser.RequestedResource,
     *      net.rim.device.api.browser.BrowserContent)
     */
    public HttpConnection getResource( RequestedResource resource, BrowserContent referrer) {

        if (resource == null) {
            return null;
        }

        // check if this is cache-only request
        if (resource.isCacheOnly()) {
            // no cache support
            return null;
        }

        String url = resource.getUrl();

        if (url == null) {
            return null;
        }

        // if referrer is null we must return the connection
        if (referrer == null) {
            HttpConnection connection = Utilities.makeConnection(resource.getUrl(), resource.getRequestHeaders(), null);
            return connection;
            
        } else {
            
            // if referrer is provided we can set up the connection on a separate thread
            SecondaryResourceFetchThread.enqueue(resource, referrer);
            
        }

        return null;
    }

    /**
     * @see net.rim.device.api.browser.RenderingApplication#invokeRunnable(java.lang.Runnable)
     */
    public void invokeRunnable(Runnable runnable) {
        (new Thread(runnable)).run();
    }
    
}

class PrimaryResourceFetchThread extends Thread {
    
    private RhodesApplication _application;
    
    private Event _event;

    private byte[] _postData;

    private HttpHeaders _requestHeaders;

    private String _url;
    
    PrimaryResourceFetchThread(String url, HttpHeaders requestHeaders, byte[] postData, 
                                  Event event, RhodesApplication application) {
        
        _url = url;
        _requestHeaders = requestHeaders;
        _postData = postData;
        _application = application;
        _event = event;
    }

    public void run() {
        HttpConnection connection = Utilities.makeConnection(_url, _requestHeaders, _postData);
        _application.processConnection(connection, _event);        
    }
}

