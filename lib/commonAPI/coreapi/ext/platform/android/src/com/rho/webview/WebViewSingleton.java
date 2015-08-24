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

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.HashMap; 

import android.util.*;
import android.annotation.TargetApi;
import android.content.Context;
import android.content.Intent;
import android.net.Proxy;
import android.os.Build;
import android.os.Parcelable;
import java.util.*;
import org.apache.http.HttpHost;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;


public class WebViewSingleton implements IWebViewSingleton, IRhoExtension {

    private static final String TAG = WebViewSingleton.class.getSimpleName();
    
    private static final String DISABLE_PAGE_LOADING_INDICATION = "disable_loading_indication";
    private static final String ENABLE_ZOOM = "enable_screen_zoom";
    private static final String ENABLE_WEB_PLUGINS = "enable_web_plugins";
    private static final String ENABLE_CACHE = "WebView.enableCache";
    public final int PROXY_CHANGED = 193;
    private WebViewConfig mConfig = new WebViewConfig();

    public WebViewSingleton() {
        IRhoExtManager extManager = RhoExtManager.getInstance();
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
    public void getEnableZoom(IMethodResult result) {
        result.set(mConfig.getBool(WebViewConfig.ENABLE_ZOOM));
    }

//    @Override
//    public void setEnableZoom(boolean value, IMethodResult result) {
//        RhoConf.setBoolean(ENABLE_ZOOM, value);
//        mEnableZoom = value;
//    }

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

    @Override
    public boolean onBeforeNavigate(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "onBeforeNavigate ------------------------------------------------------------");
        return res;
    }

    @Override
    public boolean onNavigateStarted(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateComplete(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        Logger.I(TAG, "onNavigateComplete <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        return res;
    }

    @Override
    public boolean onDocumentComplete(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
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

        if(name.equalsIgnoreCase("rhoelementsext")) {
            readRhoelementsConfig(config);
            RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandler(true) {
                @Override public void run() {
                    MainView mainView = RhodesActivity.safeGetInstance().getMainView();
                    for (int i = 0; i < mainView.getTabsCount(); ++i) {
                        Logger.T(TAG, "Set WebView config: tab " + i);
                        mainView.getWebView(i).setConfig(mConfig);
                    }
                }});
        } else
        if (name.equalsIgnoreCase("rhoconfig")) {
            
            readRhoConfig(config);

            RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandler(true) {
                @Override public void run() {
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
		    		setProxy(RhodesActivity.getContext(),hostURL,port);
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
	    		
	    			setProxy(RhodesActivity.getContext(),hostURL,port);
	    		}
	    	}
    	}
        
    }
    
    /**
     * Override WebKit Proxy settings
     *
     * @param ctx  Android ApplicationContext
     * @param host
     * @param port
     * @return true if Proxy was successfully set
     */
    public boolean setProxy(Context ctx, String host, int port) {
        boolean ret = false;
        setSystemProperties(host, port);

        try {
            if (Build.VERSION.SDK_INT > 18) {
                ret = setKitKatProxy(ctx, host, port);
            } else if (Build.VERSION.SDK_INT > 13) {
                ret = setICSProxy(host, port);
            } else {
                Object requestQueueObject = getRequestQueue(ctx);
                if (requestQueueObject != null) {
                    // Create Proxy config object and set it into request Q
                    HttpHost httpHost = new HttpHost(host, port, "http");

                    setDeclaredField(requestQueueObject, "mProxyHost", httpHost);
                    ret = true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }
    
    private void setDeclaredField(Object obj, String name, Object value)
            throws SecurityException, NoSuchFieldException, IllegalArgumentException,
            IllegalAccessException {
        Field f = obj.getClass().getDeclaredField(name);
        f.setAccessible(true);
        f.set(obj, value);
    }

    public  Object getRequestQueue(Context ctx) throws Exception {
        Object ret = null;
        Class networkClass = Class.forName("android.webkit.Network");
        if (networkClass != null) {
            Object networkObj = invokeMethod(networkClass, "getInstance", new Object[]{ctx},
                    Context.class);
            if (networkObj != null) {
                ret = getDeclaredField(networkObj, "mRequestQueue");
            }
        }	
        return ret;
    }
    
    private  Object getDeclaredField(Object obj, String name) throws SecurityException,
    NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
    	Field f = obj.getClass().getDeclaredField(name);
    	f.setAccessible(true);
    	// System.out.println(obj.getClass().getName() + "." + name + " = "+
    	// out);
    	return f.get(obj);
    }


    private Object invokeMethod(Object object, String methodName, Object[] params,
    		Class... types) throws Exception {
    	Object out = null;
    	Class c = object instanceof Class ? (Class) object : object.getClass();
    	if (types != null) {
    		Method method = c.getMethod(methodName, types);
    		out = method.invoke(object, params);
    	} else {
    		Method method = c.getMethod(methodName);
    		out = method.invoke(object);
    	}
    	// System.out.println(object.getClass().getName() + "." + methodName +
    	// "() = "+ out);
    	return out;
    }
    private  boolean setICSProxy(String host, int port) throws ClassNotFoundException,
            NoSuchMethodException, IllegalArgumentException, InstantiationException,
            IllegalAccessException, InvocationTargetException {
        Class webViewCoreClass = Class.forName("android.webkit.WebViewCore");
        Class proxyPropertiesClass = Class.forName("android.net.ProxyProperties");
        if (webViewCoreClass != null && proxyPropertiesClass != null) {
            Method m = webViewCoreClass.getDeclaredMethod("sendStaticMessage", Integer.TYPE,
                    Object.class);
            Constructor c = proxyPropertiesClass.getConstructor(String.class, Integer.TYPE,
                    String.class);
            m.setAccessible(true);
            c.setAccessible(true);
            Object properties = c.newInstance(host, port, null);
            m.invoke(null, PROXY_CHANGED, properties);
            return true;
        }
        return false;

    }

 
    private  boolean setKitKatProxy(Context context, String host, int port) {
        Context appContext = context.getApplicationContext();
        try {
            Class applictionCls = appContext.getClass();
            Field loadedApkField = applictionCls.getField("mLoadedApk");
            loadedApkField.setAccessible(true);
            Object loadedApk = loadedApkField.get(appContext);
            Class loadedApkCls = Class.forName("android.app.LoadedApk");
            Field receiversField = loadedApkCls.getDeclaredField("mReceivers");
            receiversField.setAccessible(true);
            ArrayMap receivers = (ArrayMap) receiversField.get(loadedApk);
            for (Object receiverMap : receivers.values()) {
                for (Object rec : ((ArrayMap) receiverMap).keySet()) {
                    Class clazz = rec.getClass();
                    if (clazz.getName().contains("ProxyChangeListener")) {
                        Method onReceiveMethod = clazz.getDeclaredMethod("onReceive", Context.class, Intent.class);
                        Intent intent = new Intent(Proxy.PROXY_CHANGE_ACTION);

                        /*********** optional, may be need in future *************/
                        final String CLASS_NAME = "android.net.ProxyProperties";
                        Class cls = Class.forName(CLASS_NAME);
                        Constructor constructor = cls.getConstructor(String.class, Integer.TYPE, String.class);
                        constructor.setAccessible(true);
                        Object proxyProperties = constructor.newInstance(host, port, null);
                        intent.putExtra("proxy", (Parcelable) proxyProperties);
                        /*********** optional, may be need in future *************/

                        onReceiveMethod.invoke(rec, appContext, intent);
                    }
                }
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    private  void setSystemProperties(String host, int port) {

        System.setProperty("http.proxyHost", host);
        System.setProperty("http.proxyPort", port + "");
        
        System.setProperty("https.proxyHost", host);
        System.setProperty("https.proxyPort", port + "");


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
    }

}
