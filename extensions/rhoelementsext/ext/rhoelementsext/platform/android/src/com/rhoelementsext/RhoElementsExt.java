package com.rhoelementsext;

import java.io.File;
import java.lang.Exception;
import java.lang.*;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.net.URI;

import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Environment;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.ElementsCore.BadLinkReason;
import com.rho.rhoelements.KeyCodeRemapper;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;


import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;



public class RhoElementsExt extends AbstractRhoExtension
{
    private static final String TAG = RhoElementsExt.class.getSimpleName();

    public final static String jsCollectMetas = 
            "var metas = document.getElementsByTagName('meta');" +
            "for (var i=0; i<metas.length; i++) metaReceiver.setMeta(metas[i].getAttribute('http-equiv'), metas[i].getAttribute('content'));" +
            "metaReceiver.setMeta('status', 'eom');";

    public final static String jsInjectWrappersTpl = "javascript:" +
            "var scriptElement = document.createElement('script');" + 
            "scriptElement.setAttribute('type', 'text/javascript');" + 
            "scriptElement.setAttribute('src', '%s');" +
            "document.getElementsByTagName('head')[0].appendChild(scriptElement);";
    
    public final static String wrappersLocalUrl = "file:///android_asset/apps/public/re1/elements.js";
    public final static String wrappersServerPath = "/public/re1/elements.js";

    private boolean mIsActivated = false;
    private boolean isFirstTime = true;
    private static boolean isPluginUiAdded = false;
    public static boolean isFromIntent = false; //This is set by RhoElementsProxy if it gets there because of a new intent (shortcuts)
    private static boolean isAppAlreadyLaunch = false;
    
    @Override
    public boolean onSetPropertiesData(IRhoExtManager extManager, String propId, String data, int pos, int total, IRhoWebView webView, boolean res)
    {
    	Logger.D(TAG, "onSetPropertiesData: propId: " + propId + ", data: " + data + ", pos: " + pos + ", total");
    	if (res) //Callback was already handled by some other extension
    	    return res;
    	res = false;
    	try
    	{
    	    //RhoElementsProxy.getElementsCoreInstance().onMeta(webView, propId, data, pos, total);
    	    Common.metaReceiver.setMeta(propId, data);
    	    res = true;
    	}
    	catch (Exception e)
	{
    	    Logger.E(TAG, e);
	}
    	return res;
    }
    
    @Override
    public boolean onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoWebView webView, boolean res)
    {
    	Logger.D(TAG, "onSetPropertiesDataEnd -- START");
    	if (res) //Callback was already handled by some other extension
    	    return res;
    	res = false;
    	try
	{
    	    //RhoElementsProxy.getElementsCoreInstance().onMetaEnd(webView);
    	    Common.metaReceiver.setMeta("status", "eom");
    	    res = true;
	}
	catch (Exception e)
	{
	    Logger.E(TAG, e);
	}
    	return res;
    }
    
    @Override
    public boolean onNavigateStarted(IRhoExtManager extManager, final String url, final IRhoWebView webView, boolean res)
    {
        Logger.D(TAG, "onNavigateStarted: url:" + url);

        if (isPluginUiAdded == false)
        {
            RhoElementsProxy.getElementsCoreInstance().addPluginUi(RhoElementsProxy.getActivity());
            isPluginUiAdded = true;
        }
        try
        {
            // Hides the SIP if shown and in automatic mode
            RhoElementsProxy.getElementsCoreInstance().onInputMethod(webView, false, "", new Rect());
            RhoElementsProxy.getElementsCoreInstance().onLoadStart(webView, url, 0L, 0L);
        }
        catch (Exception e)
        {
            Logger.E(TAG, e);
        }
        return res;
    }
    
    @Override
    public boolean onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoWebView webView, boolean res)
    {
        Logger.D(TAG, "onNavigateProgress: url:" + url);

        try
        {
            RhoElementsProxy.getElementsCoreInstance().onLoadProgress(webView, pos, total);
        }
        catch (Exception e)
        {
            Logger.E(TAG, e);
        }
        return res;
    }
    
    @Override
    public boolean onNavigateComplete(IRhoExtManager extManager, final String url, final IRhoWebView webView, boolean res)
    {
        Logger.D(TAG, "onNavigateComplete: url:" + url);
        if(!(url.contains("Android/data/com.symbol.rhoelements/settings.html")))
		RhoElementsProxy.getElementsCoreInstance().startSettingsButton();
		
	if((url.contains("Android/data/com.symbol.rhoelements/badlink.html")))
		RhoElementsProxy.getElementsCoreInstance().enableSettingsButton(true);
		
        if (isFromIntent)
            isFromIntent = false; //Reset flag

        try
        {
            RhoElementsProxy.getElementsCoreInstance().onLoadEnd(webView, url, 0, 0);
            webView.loadUrl("javascript:" + jsCollectMetas);
            
//            if (url.startsWith("file://") || url.startsWith("content://" + ContextFactory.getAppContext().getPackageName())) {
//                webView.loadUrl(String.format(jsInjectWrappersTpl, wrappersLocalUrl));
//            }
//            else {
//                Uri uri = Uri.parse(url);
//                StringBuilder wrappersUrl = new StringBuilder("http://");
//                wrappersUrl.append(uri.getAuthority());
//                wrappersUrl.append(wrappersServerPath);
//                webView.loadUrl(String.format(jsInjectWrappersTpl, wrappersUrl.toString()));
//            }
        }
        catch (Exception e)
        {
            Logger.E(TAG, e);
        }
        return res;
    }

    @Override
    public boolean onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect area, IRhoWebView webView, boolean res) {
        Logger.D(TAG, "onInputMethod: enabled:" + enabled);

        if (res) // Callback was already handled by some other extension
            return res;
        res = false;

        try {
            RhoElementsProxy.getElementsCoreInstance().onInputMethod(webView, enabled, type, area);
            res = true;
        }
        catch (Exception e) {
            Logger.E(TAG, e);
        }
        return res;
    }
    
    @Override
    public boolean onNavigateError(IRhoExtManager extManager, String url, IRhoExtension.LoadErrorReason reason, IRhoWebView webView, boolean res)
    {
    	Logger.D(TAG, "onNavigateError: url:" + url);
    	if (res) //Callback was already handled by some other extension
    	    return res;
    	res = false;
    	try
	{
    	    if (reason == IRhoExtension.LoadErrorReason.BAD_LICENSE)
    	    {
    		RhoElementsProxy.getElementsCoreInstance().showBadLink(BadLinkReason.UNLICENSED);
    	    }
    	    else
    		RhoElementsProxy.getElementsCoreInstance().onLoadError(webView);
    	    res = true;
	}
	catch (Exception e)
	{
	    Logger.E(TAG, e);
	}
    	return res;
    }
    
    @Override
    public boolean startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoWebView webView, boolean res)
    {
    	Logger.D(TAG, "startLocationUpdates: highAccuracy:" + highAccuracy);
    	if (res) //Callback was already handled by some other extension
    	    return res;
    	res = false;
    	try
	{
    	    RhoElementsProxy.getElementsCoreInstance().startLocationUpdates(webView.getView(), highAccuracy);
    	    res = true;
	}
	catch (Exception e)
	{
	    Logger.E(TAG, e);
	}
    	return res;
    }
    
    @Override
    public boolean stopLocationUpdates(IRhoExtManager extManager, IRhoWebView webView, boolean res)
    {
    	Logger.D(TAG, "stopLocationUpdates");
    	if (res) //Callback was already handled by some other extension
    	    return res;
    	res = false;
    	try
	{
    	    RhoElementsProxy.getElementsCoreInstance().stopLocationUpdates(webView.getView());
    	    res = true;
	}
	catch (Exception e)
	{
	    Logger.E(TAG, e);
	}
    	return res;
    }

    @Override
    /**
     * Called everytime the activity starts and either close or put in the background.
     */
    public void onAppActivate(IRhoExtManager extManager, boolean bActivate)
    {
        Logger.D(TAG, "onAppActivate: " + bActivate);
        Logger.D(TAG, "shared_runtime: " + extManager.onStartNewConfig());
        Logger.D(TAG, "isFirstTime: " + isFirstTime);
        Logger.D(TAG, "isFromIntent: " + isFromIntent);
        
        // Do Ruby interface plugins registration when application (server, Ruby VM) started completely
        
        if (bActivate && !mIsActivated)
        { 
            //extManager.registerExtension("rhoelements", this);
            String plugins = extManager.getBuildConfigItem("moto-plugins");
            Logger.D(TAG, "Symbol plugins processing: " + plugins);
            mIsActivated = true;
        }
        
        if (bActivate)
        {
	        //Check whether the application is Native or not (RhoElements)
	        if ( (isFirstTime) && (extManager.onStartNewConfig()) )
	        {
		        if (isFromIntent == false) //If this is true, itmeans that the start page is the one the user has set in the config.xml
		        {
		            String url = Common.config.getSetting(Config.SETTING_START_PAGE);
		            Logger.D(TAG, "url: " + Common.config.getSetting(Config.SETTING_START_PAGE));
		            String parsedURL = null;
		            try
		            {
		        	parsedURL = Common.parseAndroidURI(url).toString();
		        	Logger.D(TAG, "parsedURL: " + parsedURL);
		            } 
		            catch (InvalidParameterException e)
		            {
		        	// Will never happen
		        	parsedURL = url;
		            } 
		            catch (URISyntaxException e)
		            {
		        	parsedURL = url; // can happen if not a normal URI (i.e a Javascript call)
		            }
		            extManager.navigate(parsedURL);
		        }
		        else
		        {
		            try
		            {
		        	Intent rhodesIntent = RhodesActivity.safeGetInstance().getIntent();
		        	Logger.D(TAG, "intent id: " + rhodesIntent.getExtras().getInt("id"));
		        	Logger.D(TAG, "intent data: " + rhodesIntent.getDataString());
		        	RhoElementsProxy.getInstance().onNewIntent(RhodesActivity.safeGetInstance(), RhodesActivity.safeGetInstance().getIntent());
		            }
		            catch (NullPointerException e)
		            {
		        	Logger.D(TAG, "Initialization issue:");
		        	Logger.D(TAG, "RhodesActivity: " + RhodesActivity.safeGetInstance());
		        	Logger.D(TAG, "RhoElementsProxy instance: " + RhoElementsProxy.getInstance());
		            }
		        }
		        isFirstTime=false;
	        }
        }
    }

    static public Object metaProc(String module, String func, String params)
    {
    	Logger.D(TAG, "metaProc -- " + "module: " + module + " func: " + func + " params: "+ params);
    	try
		{
			Common.metaReceiver.setMeta(module, func);
		}
		catch (Exception e)
		{
			Logger.E(TAG, e);
		}
        return null;
    }
    
    @Override
    public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config, String name, boolean res)
    {
        if(name.contentEquals("rhoelementsext")) {
        RhoElementsProxy.getElementsCoreInstance().initKeyHashMap();
            Logger.T(TAG, "Processing rhoelements config");
            IRhoConfig rhoelementsGetConfig= null;
		IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
		GoogleWebView web=(GoogleWebView)vi;
		rhoelementsGetConfig=web.getConfig();
		WebViewConfig cfg=(WebViewConfig)rhoelementsGetConfig;
            String logProtocol = config.getString("logprotocol");
            String logURI = config.getString("loguri");
            String logPort = config.getString("logport");
            //String startPage = config.getString("startpage");
            String useragent = config.getString("useragent");
            
            Logger.D(TAG, "LogProtocol: " + logProtocol + ", URI: " + logURI + ", Port: " + logPort);
            String ScanDecodeWav_val = config.getString("scandecodewav");
		if(ScanDecodeWav_val != "" || ScanDecodeWav_val != null){
			RhodesActivity.setDecodeWav(ScanDecodeWav_val);
		}
			
            String fontFamily = config.getString("fontfamily");
            if(fontFamily != null){
              RhoConf.setString("fontFamily", fontFamily);
            }
            
            if(config.isExist("applicationcachepath")){
		String appCachePath = config.getString("applicationcachepath");
		if(appCachePath != null)	
                 RhoConf.setString("ApplicationCachePath", appCachePath);
            }
			
	     if(config.isExist("applicationcachequota")){
		String appCacheSize = config.getString("applicationcachequota");
		if(appCacheSize != null)
	          RhoConf.setString("ApplicationCacheQuota", appCacheSize);
	    }
	    String isFuncKeyConf =  null;
         try{
         	isFuncKeyConf = config.getString("functionkeyscapturable");
         }
         catch(NumberFormatException e){
         	e.printStackTrace();
         }
         
         if (isFuncKeyConf != null && isFuncKeyConf.length() > 0){
         	if(Integer.parseInt(isFuncKeyConf) == 0){
         		WebViewConfig.IS_FUNCTIONS_KEY_DEF = false;
         	}
         } 
         try{ 
        	 if(rhoelementsGetConfig != null){
	     if(config.isExist("enablefunctionkey_f1")){
	        	String enablefuncKeyF1Value = config.getString("enablefunctionkey_f1");
	        	if(enablefuncKeyF1Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F1, enablefuncKeyF1Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f2")){
	        	String enablefuncKeyF2Value = config.getString("enablefunctionkey_f2");
	        	if(enablefuncKeyF2Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F2, enablefuncKeyF2Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f3")){
	        	String enablefuncKeyF3Value = config.getString("enablefunctionkey_f3");
	        	if(enablefuncKeyF3Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F3, enablefuncKeyF3Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f4")){
	        	String enablefuncKeyF4Value = config.getString("enablefunctionkey_f4");
	        	if(enablefuncKeyF4Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F4, enablefuncKeyF4Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f5")){
	        	String enablefuncKeyF5Value = config.getString("enablefunctionkey_f5");
	        	if(enablefuncKeyF5Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F5, enablefuncKeyF5Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f6")){
	        	String enablefuncKeyF6Value = config.getString("enablefunctionkey_f6");
	        	if(enablefuncKeyF6Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F6, enablefuncKeyF6Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f7")){
	        	String enablefuncKeyF7Value = config.getString("enablefunctionkey_f7");
	        	if(enablefuncKeyF7Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F7, enablefuncKeyF7Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f8")){
	        	String enablefuncKeyF8Value = config.getString("enablefunctionkey_f8");
	        	if(enablefuncKeyF8Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F8, enablefuncKeyF8Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f9")){
	        	String enablefuncKeyF9Value = config.getString("enablefunctionkey_f9");
	        	if(enablefuncKeyF9Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F9, enablefuncKeyF9Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f10")){
	        	String enablefuncKeyF10Value = config.getString("enablefunctionkey_f10");
	        	if(enablefuncKeyF10Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F10, enablefuncKeyF10Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f11")){
	        	String enablefuncKeyF11Value = config.getString("enablefunctionkey_f11");
	        	if(enablefuncKeyF11Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F11, enablefuncKeyF11Value);
	        	}
	        }
	        
	        if(config.isExist("enablefunctionkey_f12")){
	        	String enablefuncKeyF12Value = config.getString("enablefunctionkey_f12");
	        	if(enablefuncKeyF12Value != null){
	        		cfg.set(WebViewConfig.ENABLE_FUNCTION_KEY_F12, enablefuncKeyF12Value);
	        	}
	        }
	        
	        if(config.isExist("exitpasswordenabled")){
	        	String ExitPasswordEnabledValue = config.getString("exitpasswordenabled");
	        	if(ExitPasswordEnabledValue != null){
	        	RhodesService.setExitPasswordEnabled(ExitPasswordEnabledValue);
	        	}
	        }
	        
		if(config.isExist("exitpasswordvalue")){
	        	String ExitPasswordValueValue = config.getString("exitpasswordvalue");
	        	if(ExitPasswordValueValue != null){
	        	RhodesService.setExitPasswordValue(ExitPasswordValueValue);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a1")){
	        	String enableAppKeyA1Value = config.getString("enableapplicationkey_a1");
	        	if(enableAppKeyA1Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A1, enableAppKeyA1Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a2")){
	        	String enableAppKeyA2Value = config.getString("enableapplicationkey_a2");
	        	if(enableAppKeyA2Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A2, enableAppKeyA2Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a3")){
	        	String enableAppKeyA3Value = config.getString("enableapplicationkey_a3");
	        	if(enableAppKeyA3Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A3, enableAppKeyA3Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a4")){
	        	String enableAppKeyA4Value = config.getString("enableapplicationkey_a4");
	        	if(enableAppKeyA4Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A4, enableAppKeyA4Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a5")){
	        	String enableAppKeyA5Value = config.getString("enableapplicationkey_a5");
	        	if(enableAppKeyA5Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A5, enableAppKeyA5Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a6")){
	        	String enableAppKeyA6Value = config.getString("enableapplicationkey_a6");
	        	if(enableAppKeyA6Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A6, enableAppKeyA6Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a7")){
	        	String enableAppKeyA7Value = config.getString("enableapplicationkey_a7");
	        	if(enableAppKeyA7Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A7, enableAppKeyA7Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a8")){
	        	String enableAppKeyA8Value = config.getString("enableapplicationkey_a8");
	        	if(enableAppKeyA8Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A8, enableAppKeyA8Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a9")){
	        	String enableAppKeyA9Value = config.getString("enableapplicationkey_a9");
	        	if(enableAppKeyA9Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A9, enableAppKeyA9Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a10")){
	        	String enableAppKeyA10Value = config.getString("enableapplicationkey_a10");
	        	if(enableAppKeyA10Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A10, enableAppKeyA10Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a11")){
	        	String enableAppKeyA11Value = config.getString("enableapplicationkey_a11");
	        	if(enableAppKeyA11Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A11, enableAppKeyA11Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a12")){
	        	String enableAppKeyA12Value = config.getString("enableapplicationkey_a12");
	        	if(enableAppKeyA12Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A12, enableAppKeyA12Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a13")){
	        	String enableAppKeyA13Value = config.getString("enableapplicationkey_a13");
	        	if(enableAppKeyA13Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A13, enableAppKeyA13Value);
	        	}
	        }
         
	        if(config.isExist("enableapplicationkey_a14")){
	        	String enableAppKeyA14Value = config.getString("enableapplicationkey_a14");
	        	if(enableAppKeyA14Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A14, enableAppKeyA14Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a15")){
	        	String enableAppKeyA15Value = config.getString("enableapplicationkey_a15");
	        	if(enableAppKeyA15Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A15, enableAppKeyA15Value);
	        	}
	        }
	        
	        if(config.isExist("enableapplicationkey_a16")){
	        	String enableAppKeyA16Value = config.getString("enableapplicationkey_a16");
	        	if(enableAppKeyA16Value != null){
	        		cfg.set(WebViewConfig.ENABLE_APPLICATION_KEY_A16, enableAppKeyA16Value);
	        	}
	        }
	        
	        if(config.isExist("resizeonsip")){
	        	String resizeOnSIP = config.getString("resizeonsip");
	        	if(resizeOnSIP != null && resizeOnSIP.contains("1") && !RhodesActivity.IS_RESIZE_SIP){
	        		RhodesActivity.safeGetInstance().resizeSIP();
	        	}
	        }
	        if(config.isExist("zoominkey")){
			String zoomInKey = config.getString("zoominkey"); 
			if(zoomInKey.length() > 0){
				Common.ZOOM_KEY_ENABLE = true;
				//zoomInKey = zoomInKey.replace("0x","");
				//Common.ZOOM_IN_KEY  = hex2decimal(zoomInKey);
				try{
					Common.ZOOM_IN_KEY=KeyCodeRemapper.getProperParameter(zoomInKey);
				}catch(Exception e){
					e.printStackTrace();
				}
				}
			}
	     if(config.isExist("zoomoutkey")){
	     	String zoomOutKey = config.getString("zoomoutkey"); 
			if(zoomOutKey.length() > 0){
				Common.ZOOM_KEY_ENABLE = true;
				//zoomOutKey = zoomOutKey.replace("0x","");
				//Common.ZOOM_OUT_KEY = hex2decimal(zoomOutKey); 
				try{
					Common.ZOOM_OUT_KEY = KeyCodeRemapper.getProperParameter(zoomOutKey);
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		 }
		 
	      if(config.isExist("lowbatteryscan")){
	        	String enableScan = config.getString("lowbatteryscan");
	        	if(enableScan != null){
	        		cfg.set(WebViewConfig.ENABLE_SCAN, enableScan);
	        	}
	        }
	        
         }
        	 }
         catch(NumberFormatException e){
 			e.printStackTrace();
 		}
 		
            if(useragent != null){
                RhoConf.setString("useragent", useragent);
            }
			try {
	            if (logProtocol != null && logURI != null) {
	            	Uri logUri = Uri.parse(Common.parseAndroidURI(logURI).toString());
	
	                if (logProtocol.equalsIgnoreCase("file")) {
	                    String logPath = logUri.getPath();
	                    Logger.I(TAG, "New log path: " + logPath);
	                    RhodesService.resetFileLogging(logPath);
	                }
	                else if(logProtocol.equalsIgnoreCase("http")) {        
	                    if ( logPort != null ) {
	                      try {
	                        URI uri = new URI("http",logUri.getUserInfo(),logUri.getHost(),Integer.parseInt(logPort),logUri.getPath(),logUri.getQuery(),logUri.getFragment());
	                        logUri = Uri.parse(uri.toString());
	                      } catch (NumberFormatException e) {
	                          Logger.E(TAG, e);
	                      } catch (URISyntaxException e) {
	                          Logger.E(TAG, e);
	                      }
	                    }
	                    Logger.D(TAG, "New log URI: " + logUri.toString());
	                    RhodesService.resetHttpLogging(logUri.toString());
	                }
	            }
			} catch (InvalidParameterException e1) {
				Logger.E(TAG, "LogURI could not be parsed, setting to default");
			} catch (URISyntaxException e1) {
				Logger.E(TAG, "LogURI could not be parsed, setting to default");
			}

            int minSeverity = -1;
            
            if (config.isExist("logdebug") && config.getBool("logdebug")) {
                minSeverity = 0;
            }
             else if (config.isExist("logtrace") && config.getBool("logtrace")) {
                minSeverity = 0;
            } 
            else if ((config.isExist("loginfo") && config.getBool("loginfo"))
                    || (config.isExist("loguser") && config.getBool("loguser"))) {
                minSeverity = 1;
            } else if (config.isExist("logwarning") && config.getBool("logwarning")) {
                minSeverity = 2;
            } else if (config.isExist("logerror") && config.getBool("logerror")) {
                minSeverity = 3;
            }
            
            if (minSeverity >= 0) {
                Logger.T(TAG, "Setting logging min severity to " + minSeverity);
                RhoLogConf.setMinSeverity(minSeverity);
            }
          
            if (config.isExist("autorotate") && isAppAlreadyLaunch == false) {
                RhodesService.setScreenAutoRotate(config.getBool("autorotate"));
				isAppAlreadyLaunch = true;				
            }
            
            /*if(startPage !=null)
			{ 
			   Logger.E(TAG, "The value of start tag is "+ startPage);
			   RhodesService.setStartPath(startPage);
			}*/
	    		
        }
		
		RhoElementsProxy.getElementsCoreInstance().RegisterCustomReciever( extManager,  config,  name,  res);

        return res;
    }
	    @Override
    public boolean onWebViewCreated(IRhoExtManager extManager, IRhoWebView ext, boolean res)
    {
        Logger.T(TAG, "Adding 'generic' to JS");
        ext.addJSInterface(Generic.getInstance(), "genericImpl");

        Logger.T(TAG, "Adding 'apd' to JS");
        ext.addJSInterface(APD.getInstance(), "apdImpl");
        
        Logger.T(TAG, "Adding 'nosip' to JS");
        ext.addJSInterface(NoSIP.getInstance(), "nosipImpl");

        Logger.T(TAG, "Adding 'metaReceiver' to JS");
        ext.addJSInterface(Common.metaReceiver, "metaReceiver");

        return res;
    }
    public  int hex2decimal(String s) {
        String digits = "0123456789ABCDEF";
        s = s.toUpperCase();
        int val = 0;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            int d = digits.indexOf(c);
            val = 16*val + d;
        }
        return val;
    }

}


