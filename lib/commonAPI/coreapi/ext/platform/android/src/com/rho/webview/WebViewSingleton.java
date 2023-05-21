package com.rho.webview;

import android.graphics.Rect;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.webview.WebViewConfig;

import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.net.Proxy;
import android.net.Uri;
import android.webkit.ValueCallback;

import java.util.HashMap;
import java.util.Map;

public class WebViewSingleton implements IWebViewSingleton, IRhoExtension {

    private static final String TAG = WebViewSingleton.class.getSimpleName();

    private static final String DISABLE_PAGE_LOADING_INDICATION = "disable_loading_indication";
    private static final String ENABLE_ZOOM = "enable_screen_zoom";
    private static final String ENABLE_WEB_PLUGINS = "enable_web_plugins";
    private static final String ENABLE_CACHE = "WebView.enableCache";
    private static final String DISABLE_SCANNER_NAVIGATION = "disable_scanner_during_navigation";
    private static final String ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE = "enable_media_playback_without_gesture";
    private WebViewConfig mConfig = new WebViewConfig();

    //private boolean bypassProxy;
    private String originalProxyHost;
    private int originalProxyPort;

    private final int bypassPolicyNever = 0; //never override proxy settings, never restore;
    private final int bypassPolicyLocalPage = 1; //override proxy on local page load start, restore settings on page load finish;
    private final int bypassPolicyLocalResource = 2; //override proxy on local resource load start, restore settings on page load finish;
    private final int bypassPolicyAppActive =3; //override proxy on app activate, restore settings on app deactivate;

    private int bypassPolicy = bypassPolicyLocalPage; //default

    public WebViewSingleton() {
        IRhoExtManager extManager = RhoExtManager.getInstance();

        originalProxyHost = Proxy.getDefaultHost();
        originalProxyPort = Proxy.getDefaultPort();

        readRhoConfig(extManager.getConfig("rhoconfig"));
    }

    @Override
    public void getFramework(IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().getWebView(-1).getEngineId());
    }

    @Override
    public void getFullScreen(IMethodResult result) {
        result.set(BaseActivity.getFullScreenMode());
    }

    @Override
    public void setFullScreen(boolean value, IMethodResult result) {
        BaseActivity.setFullScreenMode(value);
    }

    @Override
    public void getKeyboardDisplayRequiresUserAction(IMethodResult result) {
        result.set(true);
    }

    @Override
    public void setKeyboardDisplayRequiresUserAction(boolean value, IMethodResult result) {
    }


    @Override
    public void getEnableDragAndDrop(IMethodResult result) {
        result.set(true);
    }

    @Override
    public void setEnableDragAndDrop(boolean value, IMethodResult result) {
    }

    @Override
    public void getEnableZoom(IMethodResult result) {
        result.set(mConfig.getBool(WebViewConfig.ENABLE_ZOOM));
    }

//    @Override
//    public void setEnableZoom(boolean value, IMethodResult result) {
//        RhoConf.setBoolean(ENABLE_ZOOM, value);
//        mEnableZoom = value;
//    }
//
    @Override
    public void getEnableMediaPlaybackWithoutGesture(IMethodResult result) {
        result.set(mConfig.getBool(WebViewConfig.ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE));
    }

    @Override
    public void getEnablePageLoadingIndication(IMethodResult result) {
        result.set(mConfig.getBool(WebViewConfig.ENABLE_PAGE_LOADING_INDICATION));
    }

//    @Override
//    public void setEnablePageLoadingIndication(boolean value, IMethodResult result) {
//        RhoConf.setBoolean(DISABLE_PAGE_LOADING_INDICATION, !value);
//        mEnablePageLoadingIndication = value;
//    }

    @Override
    public void getEnableWebPlugins(IMethodResult result) {
        result.set(mConfig.getBool(WebViewConfig.ENABLE_WEB_PLUGINS));
    }

//    @Override
//    public void setEnableWebPlugins(boolean value, IMethodResult result) {
//        RhoConf.setBoolean(ENABLE_WEB_PLUGINS, value);
//        mEnableWebPlugins = value;
//    }

    @Override
    public void getNavigationTimeout(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setNavigationTimeout(int value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getScrollTechnique(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getFontFamily(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getUserAgent(IMethodResult result) {
        //RhodesActivity.safeGetInstance().getMainView().getWebView(-1).
    }

    @Override
    public void getViewportEnabled(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getViewportWidth(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getCacheSize(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getEnableCache(IMethodResult result) {
        result.set(mConfig.getBool(WebViewConfig.ENABLE_CACHE));
    }

//    @Override
//    public void setEnableCache(boolean value, IMethodResult result) {
//        RhoConf.setBoolean(ENABLE_CACHE, value);
//        mEnableCache = value;
//    }

    @Override
    public void getAcceptLanguage(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setAcceptLanguage(String value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getZoomPage(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setZoomPage(double value, IMethodResult result) {
        RhodesActivity.safeGetInstance().getMainView().getWebView(-1).setZoom(value);

    }

    @Override
    public void getTextZoomLevel(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setTextZoomLevel(int value, IMethodResult result) {
		IRhoWebView.TextZoom textZoom;
		switch (value)
		{
		case 0:
			textZoom = IRhoWebView.TextZoom.SMALLEST;
			break;
		case 1:
			textZoom = IRhoWebView.TextZoom.SMALLER;
			break;
		case 2:
			textZoom = IRhoWebView.TextZoom.NORMAL;
			break;
		case 3:
			textZoom = IRhoWebView.TextZoom.LARGER;
			break;
		case 4:
			textZoom = IRhoWebView.TextZoom.LARGEST;
			break;
		default:
			textZoom = IRhoWebView.TextZoom.NORMAL;
			break;
		}
        RhodesActivity.safeGetInstance().getMainView().getWebView(-1).setTextZoom(textZoom);
    }

    @Override
    public void refresh(int tabIndex, IMethodResult result) {
        WebView.refresh(tabIndex);
    }

    @Override
    public void navigate(String url, int tabIndex, IMethodResult result) {
        WebView.navigate(RhodesService.getInstance().normalizeUrl(url), tabIndex);
    }

    @Override
    public void navigateBack(int tabIndex, IMethodResult result) {
        WebView.navigateBack(tabIndex);
    }

    @Override
    public void currentLocation(int tabIndex, IMethodResult result) {
        result.set(RhodesService.currentLocation(tabIndex));
    }

    @Override
    public void currentURL(int tabIndex, IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().get_current_url(tabIndex));
    }

    @Override
    public void executeJavascript(String javascriptText, int tabIndex, IMethodResult result) {
        WebView.executeJs(javascriptText, tabIndex);
    }

    @Override
    public void active_tab(IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().activeTab());
    }

    @Override
    public void full_screen_mode(boolean enable, IMethodResult result) {
        setFullScreen(enable, result);
    }

    @Override
    public void setCookie(String url, String cookie, IMethodResult result) {
        WebView.setCookie(url, cookie);
    }

    @Override
    public void getCookies(final String url, final IMethodResult result) {
      try {
          PerformOnUiThread.exec(new Runnable() {
              @Override
              public void run() {
                  result.set( WebView.getCookies(url) );
              }
          });
      }
      catch (Exception e) {
          Logger.E(TAG, e);
          result.setError( e.toString() );
      }
    }

    @Override
    public void removeCookie(final String url, final String cookie, final IMethodResult result) {

      try {
          PerformOnUiThread.exec(new Runnable() {
              @Override
              public void run() {
                  boolean removed = WebView.removeCookie(url, cookie);
                  Map<String,Object> res = new HashMap();
                  if ( removed ) {
                      res.put( "removed_cookie", cookie );
                  }
                  result.set(res);
              }
          });
      }
      catch (Exception e) {
          Logger.E(TAG, e);
          result.setError(e.toString());
      }
    }

    @Override
    public void removeAllCookies( final IMethodResult result ) {
        try {
            PerformOnUiThread.exec(new Runnable() {
                @Override
                public void run() {
                    WebView.removeAllCookies( new ValueCallback<Boolean>() {
                        @Override
                        public void onReceiveValue( Boolean v ) {
                            result.set( v.booleanValue() );
                        }

                    });
                }
            });
        } catch (Exception e) {
            Logger.E(TAG,e);
            result.setError(e.toString());
        }
    }

    @Override
    public void save(String format, String path, int tabIndex, IMethodResult result) {
        WebView.saveCurrentPage(format, path, tabIndex);
    }

    @Override
    public void getActiveTab(IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().activeTab());
    }

    public WebViewConfig getWebViewConfig() {
        return mConfig;
    }

    @Override
    public IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex) {
        return null;
    }

    @Override
    public boolean onWebViewCreated(IRhoExtManager extManager, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "Set config initially: " + (mConfig != null));
        ext.setConfig(mConfig);

        return res;
    }

    @Override
    public boolean onSetPropertiesData(IRhoExtManager extManager, String propId, String data, int pos, int total, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onSetProperty(IRhoExtManager extManager, String name, String value, IRhoWebView ext, boolean res) {
        return res;
    }

    private void adjustProxySettingsForUrl( String url ) {
        Uri uri = Uri.parse(url);
        String host = uri.getHost();

        boolean haveOriginalProxy = ( ( originalProxyHost!=null ) && ( originalProxyHost.length() > 0 ) );
        boolean isLocalUrl = ( (host!=null) && ( host.equals("localhost") || uri.getHost().equals("127.0.0.1") ) );

        Logger.D( TAG, "adjustProxySettingsForUrl: " + url + " originalProxyHost: " + originalProxyHost );


        if ( haveOriginalProxy ) {
            //we have system/rhoelements proxy settings so decide what to do with URL
            if ( isLocalUrl ) {
                //local URL, we don't need proxy
                dropProxySettings();
            } else {
                //set original proxy setting
                restoreProxySettings();
            }
        }
    }

    private void dropProxySettings() {
        Logger.I( TAG, "Bypassing system proxy: " + originalProxyHost + ":" + String.valueOf(originalProxyPort) );
        ProxySettings.setProxy(ContextFactory.getAppContext(),"",0);
    }

    private void restoreProxySettings() {

        boolean haveOriginalProxy = ( ( originalProxyHost!=null ) && ( originalProxyHost.length() > 0 ) );

        if ( haveOriginalProxy ) {
            Logger.I( TAG, "Restoring proxy settings: " + originalProxyHost + ":" + String.valueOf(originalProxyPort) );
            ProxySettings.setProxy(ContextFactory.getAppContext(),originalProxyHost,originalProxyPort);
        }
    }

    @Override
    public boolean onBeforeNavigate(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "onBeforeNavigate >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + url );

        if ( bypassPolicy == bypassPolicyLocalPage ) {
            adjustProxySettingsForUrl( url );
        }

        return res;
    }

    @Override
    public boolean onNavigateStarted(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "onBeforeNavigate >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + url );

        if ( bypassPolicy == bypassPolicyLocalPage ) {
            adjustProxySettingsForUrl( url );
        }

        return res;
    }

    @Override
    public boolean onLoadResource(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "onLoadResource >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + url );

        if ( bypassPolicy == bypassPolicyLocalResource ) {
            adjustProxySettingsForUrl( url );
        }

        return res;
    }


    @Override
    public boolean onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateComplete(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "onNavigateComplete <<<<<<<<<<<<<<<<<<<<<<<<<<<<<" + url );

        if ( (bypassPolicy == bypassPolicyLocalResource ) || (bypassPolicy == bypassPolicyLocalPage ) ) {
            restoreProxySettings();
        }

        return res;
    }

    @Override
    public boolean onDocumentComplete(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onGoBack(IRhoExtManager extManager, String current_url, String back_url, IRhoWebView ext, boolean res) {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean onGoForward(IRhoExtManager extManager, String current_url, String forward_url, IRhoWebView ext, boolean res) {
        // TODO Auto-generated method stub
        return false;
    }


    @Override
    public boolean onAlert(IRhoExtManager extManager, String message, IRhoWebView ext, IAlertResult alertResult, boolean res) {
        return res;
    }

    @Override
    public boolean onConfirm(IRhoExtManager extManager, String message, IRhoWebView ext, IAlertResult confirmResult, boolean res) {
        return res;
    }

    @Override
    public boolean onPrompt(IRhoExtManager extManager, String message, String defaultResponse, IRhoWebView ext, IPromptResult promptResult, boolean res) {
        return res;
    }

    @Override
    public boolean onSelect(IRhoExtManager extManager, String[] items, int selected, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onStatus(IRhoExtManager extManager, String status, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onTitle(IRhoExtManager extManager, String title, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onConsole(IRhoExtManager extManager, String message, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect area, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateError(IRhoExtManager extManager, String url, LoadErrorReason reason, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onAuthRequest(IRhoExtManager extManager, IAuthRequest request, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public void onAppActivate(IRhoExtManager extManager, boolean bActivate) {

        if ( bypassPolicy == bypassPolicyAppActive ) {
            if ( bActivate ) {
                dropProxySettings();
            } else {
                restoreProxySettings();
            }
        }

    }

    @Override
    public boolean startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean stopLocationUpdates(IRhoExtManager extManager, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config, String name, boolean res) {

        /*if(name.equalsIgnoreCase("rhoelementsext")) {
            readRhoelementsConfig(config);
            RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandlerInUIThread(true) {
                @Override public void runInUI() {
                    MainView mainView = RhodesActivity.safeGetInstance().getMainView();
                    for (int i = 0; i < mainView.getTabsCount(); ++i) {
                        Logger.T(TAG, "Set WebView config: tab " + i);
                        mainView.getWebView(i).setConfig(mConfig);
                    }
                }});
        } else*/
        if (name.equalsIgnoreCase("rhoconfig")) {

            readRhoConfig(config);

            RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandlerInUIThread(true) {
                @Override public void runInUI() {
                    MainView mainView = RhodesActivity.safeGetInstance().getMainView();
                    for (int i = 0; i < mainView.getTabsCount(); ++i) {
                        Logger.T(TAG, "Set WebView config: tab " + i);
                        mainView.getWebView(i).setConfig(mConfig);
                    }
                }});
        }

        return res;
    }

    @Override
    public String onGetProperty(IRhoExtManager extManager, String name) {
        return null;
    }

    private void readRhoelementsConfig(IRhoConfig config) {
        if (config.isExist("enablezoom")) {
           // mConfig.set(WebViewConfig.ENABLE_ZOOM, config.getBool("enable_screen_zoom", WebViewConfig.ENABLE_ZOOM_DEF));
            mConfig.set(WebViewConfig.ENABLE_ZOOM, config.getBool("enablezoom"));
        }else{
        	 mConfig.set(WebViewConfig.ENABLE_ZOOM, true);
        }
        if (config.isExist("pagezoom")) {
            double zoomValue = config.getDouble("pagezoom");
            mConfig.set(WebViewConfig.PAGE_ZOOM, zoomValue);
        }

        if (config.isExist("cache")) {
            String cache = config.getString("cache");
            cache=cache.toLowerCase();
            int cacheTemp=0;
            int endString=cache.indexOf("mb");
            if(endString==-1)
            	endString=cache.indexOf("kb");
            int cacheInt =Integer.parseInt(cache.substring(0,endString ).toString());

            mConfig.set(WebViewConfig.ENABLE_CACHE, cacheInt!=0);
        }

        if(config.isExist("splashscreenpath")){
        	String splashscreenpathValue = config.getString("splashscreenpath");
        	if(splashscreenpathValue != null){
        		mConfig.set(WebViewConfig.SETTING_SPLASHSCREEN_PATH, splashscreenpathValue);
        	}
        }

        if(config.isExist("splashscreenduration")){
        	String splashscreendurationValue = config.getString("splashscreenduration");
        	if(splashscreendurationValue != null){
        		mConfig.set(WebViewConfig.SETTING_SPLASHSCREEN_DURATION, splashscreendurationValue);
        	}
        }
        if(config.isExist("http_proxy")){
    		String httpProxy = config.getString("http_proxy");
    		if(httpProxy.length()!=0){
	    		int index = httpProxy.lastIndexOf(":");
	    		if(index > -1)
	    		{
		    		String portNumber = httpProxy.substring(index + 1, httpProxy.length());
		    		String hostURL = httpProxy.substring(0, index);
		    		int port = Integer.parseInt(portNumber);

                    originalProxyHost = hostURL;
                    originalProxyPort = port;

		    		ProxySettings.setProxy(RhodesActivity.getContext(),hostURL,port);
		  		}
	    	}
    	}
    	if(config.isExist("https_proxy")){
    		String httpsProxy = config.getString("https_proxy");
    		if(httpsProxy.length()!=0){
	    		int index = httpsProxy.lastIndexOf(":");
	    		if(index > -1)
	    		{
	    			String portNumber = httpsProxy.substring(index + 1, httpsProxy.length());
	    			String hostURL = httpsProxy.substring(0, index);
	    			int port = Integer.parseInt(portNumber);

                    originalProxyHost = hostURL;
                    originalProxyPort = port;

	    			ProxySettings.setProxy(RhodesActivity.getContext(),hostURL,port);
	    		}
	    	}
    	}
    	 if(config.isExist("username") && config.isExist("password") ){
	        	String username = config.getString("username");
				String password = config.getString("password");
	        	if(username != null && password !=null){
					if(username.length() > 0 && password.length()> 0){
						mConfig.set(WebViewConfig.AUTH_USERNAME,username);
						mConfig.set(WebViewConfig.AUTH_PASSWORD,password);
					}
				}
			}

    	if(config.isExist("disablescannerduringnavigation")){
    	        String disablescanner = config.getString("disablescannerduringnavigation");
    	        if(disablescanner != null){
    				if(disablescanner.length() > 0){
    						mConfig.set(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION,disablescanner);
    					}
    			}
    	}
    	if(config.isExist("iswindowskey")){
    	        String iswinkey = config.getString("iswindowskey");
    	        if(iswinkey != null){
    				if(iswinkey.length() > 0){
    						mConfig.set("iswindowskey",iswinkey);
    					}
    			}
    	}

    	if(config.isExist("usedwforscanning")){
    	        String _usedwforscanning = config.getString("usedwforscanning");
    	        if(_usedwforscanning != null){
    				if(_usedwforscanning.length() > 0){
    						mConfig.set("usedwforscanning",_usedwforscanning);
    					}
    			}
        }

        if (config.isExist(ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE)) {
            mConfig.set(WebViewConfig.ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE, config.getBool(ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE));
        } else {
            mConfig.set(WebViewConfig.ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE, false);
        }
    }

    private void readRhoConfig(IRhoConfig config) {
        if (RhoConf.isExist(DISABLE_PAGE_LOADING_INDICATION))
            mConfig.set(WebViewConfig.ENABLE_PAGE_LOADING_INDICATION, !RhoConf.getBool(DISABLE_PAGE_LOADING_INDICATION));


        if (RhoConf.isExist(ENABLE_ZOOM))
            mConfig.set(WebViewConfig.ENABLE_ZOOM, RhoConf.getBool(ENABLE_ZOOM));

        if (RhoConf.isExist(ENABLE_WEB_PLUGINS))
            mConfig.set(WebViewConfig.ENABLE_WEB_PLUGINS, RhoConf.getBool(ENABLE_WEB_PLUGINS));

        if (RhoConf.isExist(ENABLE_CACHE))
            mConfig.set(WebViewConfig.ENABLE_CACHE, RhoConf.getBool(ENABLE_CACHE));

        if (RhoConf.isExist(DISABLE_SCANNER_NAVIGATION))
            mConfig.set(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION, RhoConf.getString(DISABLE_SCANNER_NAVIGATION));
        if (RhoConf.isExist("iswindowskey"))
            mConfig.set("iswindowskey", RhoConf.getString("iswindowskey"));

        if (RhoConf.isExist("android_proxy_bypass_policy")) {
            String val = RhoConf.getString("android_proxy_bypass_policy");
            if ( val.equals("never")) {
                bypassPolicy = bypassPolicyNever;
            } else if ( val.equals("local_page") ) {
                bypassPolicy = bypassPolicyLocalPage;
            } else if ( val.equals("local_resource") ) {
                bypassPolicy = bypassPolicyLocalResource;
            } else if ( val.equals("app_active") ) {
                bypassPolicy = bypassPolicyAppActive;
            }
        }

        if (RhoConf.isExist(ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE))
            mConfig.set(WebViewConfig.ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE, RhoConf.getBool(ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE));

        Logger.I( TAG, "Proxy bypass policy: " + String.valueOf( bypassPolicy ) );

        //bypassProxy = RhoConf.isExist("android_webview_bypass_system_proxy") && RhoConf.getBool("android_webview_bypass_system_proxy");
    }

}
