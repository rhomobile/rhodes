package rhomobile;

import java.io.IOException;

import javax.microedition.io.HttpConnection;

//import org.garret.perst.Storage;
//import org.garret.perst.StorageFactory;

import net.rim.device.api.browser.field.*;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.system.Application;
import net.rim.device.api.system.KeyListener;
import net.rim.device.api.ui.*;
import net.rim.device.api.ui.component.Status;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.ui.component.Menu;
import net.rim.device.api.system.Characters;
import net.rim.device.api.system.TrackwheelListener; 
import net.rim.device.api.system.SystemListener;

import rhomobile.NetworkAccess;
import rhomobile.location.GeoLocation;
import rhomobile.sync.SyncEngine;

import java.util.Vector;


/**
 * 
 */
final public class RhodesApplication extends UiApplication implements RenderingApplication{
    
	class CKeyListener  implements KeyListener{

		public boolean keyChar(char key, int status, int time) {
	        if( key == Characters.ENTER ) {
	        	gotoUrl();
	        	return true;
	        }
			return false;
		}

		public boolean keyDown(int keycode, int time) {
			int nKey = Keypad.key(keycode);
			if ( nKey == Keypad.KEY_ESCAPE )
			{
				back();
				return true;
			}
			
			return false;
		}

		public boolean keyRepeat(int keycode, int time) {return false;}
		public boolean keyStatus(int keycode, int time) {return false;}
		public boolean keyUp(int keycode, int time) {return false;}
    };
    
    class CTrackwheelListener implements TrackwheelListener{

		public boolean trackwheelClick(int status, int time) {
			gotoUrl();
			return true;
		}

		public boolean trackwheelRoll(int amount, int status, int time) {return false;}
		public boolean trackwheelUnclick(int status, int time) {return false;}
    }
    
    void back(){
    	if ( _history.size() <= 1 )
    		return;
    	
    	int nPos = _history.size()-2;
    	String url = (String)_history.elementAt(nPos);
    	_history.removeElementAt(nPos+1);
    	
        PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(url, null, null, null, this);
        thread.start();                       
    }
    
    void addToHistory(String strUrl ){
    	int nPos = -1;
    	for( int i = _history.size()-1; i >= 0; i-- ){
    		if ( strUrl.equalsIgnoreCase((String)_history.elementAt(i)) ){
    			nPos = i;
    			break;
    		}
    		String strUrl1 = strUrl + "/index";
    		if ( strUrl1.equalsIgnoreCase((String)_history.elementAt(i)) ){
    			nPos = i;
    			break;
    		}
    		
    	}
    	if ( nPos == -1 ){
    		boolean bReplace = RhoConnection.findIndex(strUrl) != -1;

    		if ( bReplace )
    			_history.setElementAt(strUrl, _history.size()-1 );
    		else	
    			_history.addElement(strUrl);
    	}
    	else
    		_history.setSize(nPos+1);
    }
    
    void gotoUrl(){
        Menu menu = _mainScreen.getMenu(0);
    	int size = menu.getSize();
    	for(int i=0; i<size; i++) 
    	{
    	    MenuItem item = menu.getItem(i);
    	    String label = item.toString();
    	    if(label.equalsIgnoreCase("Get Link")) //TODO: catch by ID?
    	    {
    	    	item.run();
    	    }			
    	}
    }
    
	private static final String REFERER = "referer";   
    
    private RenderingSession _renderingSession;   

    private MainScreen _mainScreen;
    
    private HttpConnection  _currentConnection;
   
    private Vector _history;
    
    private final String _httpRoot = "http://localhost:8080/apps/";
    
    /***************************************************************************
     * Main.
     **************************************************************************/
    public static void main(String[] args) {
    	NetworkAccess.autoConfigure();
    	
        RhoRuby.RhoRubyStart("");
		
        RhodesApplication app = new RhodesApplication();
        app.enterEventDispatcher();
    }
    
    void doClose(){
		SyncEngine.stop(null);
		GeoLocation.stop();
        RhoRuby.RhoRubyStop();
    }

	public void activate() {
		SyncEngine.start(null);
		GeoLocation.start();
		
		super.activate();
	}
    
	public void deactivate() {
		SyncEngine.stop(null);
		GeoLocation.stop();
		
		super.deactivate();
	}
    
    class CMainScreen extends MainScreen{

		private MenuItem syncItem = new MenuItem("Sync", 200000, 10) {
			public void run() {
					SyncEngine.wakeUp();
				}
			};			
    	
		protected void makeMenu(Menu menu, int instance) {
			// TODO Auto-generated method stub
			super.makeMenu(menu, instance);
			menu.add(syncItem);
		}

		public boolean onClose() {
			doClose();
			return super.onClose();
		}

		public boolean onMenu(int instance) {
			// TODO Auto-generated method stub
			return super.onMenu(instance);
		}
    	
    }
    
    private RhodesApplication() {
    	CKeyListener list = new CKeyListener();
    	CTrackwheelListener wheel = new CTrackwheelListener();
    	this._history = new Vector();
    	
        _mainScreen = new CMainScreen();
        _mainScreen.addKeyListener(list);
        _mainScreen.addTrackwheelListener(wheel);
        
        pushScreen(_mainScreen);
        _renderingSession = RenderingSession.getNewInstance();
        // enable javascript
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.JAVASCRIPT_ENABLED, true);
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.JAVASCRIPT_LOCATION_ENABLED, true);                        
        _renderingSession.getRenderingOptions().setProperty(RenderingOptions.CORE_OPTIONS_GUID, RenderingOptions.ENABLE_CSS, true);                        
        
        String strStartPage = _httpRoot.substring(0, _httpRoot.length()-1) + 
        	RhoRuby.getStartPage();
        _history.addElement(strStartPage);
        PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(strStartPage, null, null, null, this);
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
                
                synchronized (getAppEventLock()) 
                { 
                	browserContent.finishLoading();
                }
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
                if ( !absoluteUrl.startsWith(_httpRoot) )
                	absoluteUrl = _httpRoot + absoluteUrl.substring(_httpRoot.length()-5);
    
                if ( urlRequestedEvent.getPostData() == null ||
                	 urlRequestedEvent.getPostData().length == 0 )
                	addToHistory(absoluteUrl);
                
                PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(absoluteUrl,
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
                        synchronized (getAppEventLock()) 
                        { 
                        	_mainScreen.setTitle(newTitle);
                        }
                    }
                }                   

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
                            if ( !absoluteUrl.startsWith(_httpRoot) )
                            	absoluteUrl = _httpRoot + absoluteUrl.substring(_httpRoot.length()-5);
                        	
                        	addToHistory(absoluteUrl);                        	
                            Object eventSource = e.getSource();
                            if (eventSource instanceof HttpConnection) {
                                referrer = ((HttpConnection)eventSource).getRequestProperty(REFERER);
                            }
                            break;

                    }
                    
                    HttpHeaders requestHeaders = new HttpHeaders();
                    requestHeaders.setProperty(REFERER, referrer);
                    PrimaryResourceFetchThread thread = new PrimaryResourceFetchThread(absoluteUrl, requestHeaders,null, event, this);
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
    
    public PrimaryResourceFetchThread(String url, HttpHeaders requestHeaders, byte[] postData, 
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

