package com.rhomobile.rhodes.webview;

import java.lang.reflect.Method;
import java.util.Locale;

import android.annotation.SuppressLint;
import android.os.Build;
import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.RhoConf;

public class WebSettingsProviderBase implements IWebSettingsProvider {
    protected static final String TAG = "WebSettingsProvider";

    @SuppressLint("SetJavaScriptEnabled")
    public void fillSettings(WebSettings settings, IRhoConfig config) {

        boolean enableZoom = config == null || config.getBool(WebViewConfig.ENABLE_ZOOM);
        boolean enableCache = config == null || config.getBool(WebViewConfig.ENABLE_CACHE);
        boolean enableMediaPlaybackWithoutGesture = config != null && config.getBool(WebViewConfig.ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE);
        String customUA = RhoConf.getString("useragent");

        //settings.setMediaPlaybackRequiresUserGesture(!enableMediaPlaybackWithoutGesture);
        settings.setMediaPlaybackRequiresUserGesture(false);
        settings.setSavePassword(false);
        settings.setSaveFormData(false);
        settings.setJavaScriptEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(false);
        settings.setSupportZoom(enableZoom);
        settings.setBuiltInZoomControls(enableZoom);
        //settings.setAppCacheEnabled(true);
        if(!RhoConf.getString("fontFamily").isEmpty())
        settings.setStandardFontFamily(RhoConf.getString("fontFamily"));
        //customUA = updateRevesionOfCustomUA(settings.getUserAgentString(), customUA);
        //settings.setUserAgentString(customUA);
        //if(RhoConf.isExist("ApplicationCachePath")){
        //	if(!RhoConf.getString("ApplicationCachePath").isEmpty())
        //		settings.setAppCachePath(RhoConf.getString("ApplicationCachePath"));
        //}
        //if(RhoConf.isExist("ApplicationCacheQuota")){
       // 	if(!RhoConf.getString("ApplicationCacheQuota").isEmpty())
       // 		try{
       // 		settings.setAppCacheMaxSize(Long.valueOf(RhoConf.getString("ApplicationCacheQuota")).longValue()); 
       // 		}
       // 		catch(NumberFormatException exp){
       // 		}
        //}	
        Logger.T(TAG, "Enable Zoom: " + enableZoom);
        
        if (enableCache) {
            settings.setCacheMode(WebSettings.LOAD_DEFAULT);
        } else {
            settings.setCacheMode(WebSettings.LOAD_NO_CACHE);
        }
        Logger.T(TAG, "Enable Cache: " + enableCache);

        settings.setSupportMultipleWindows(false);

        setWebPlugins(settings, config);
    }
    
    protected void setWebPlugins(WebSettings settings, IRhoConfig config) {
        
        try {
            Class<WebSettings> clazz = WebSettings.class;
            Method methodSetPlugins = clazz.getMethod("setPluginsEnabled", boolean.class);
            methodSetPlugins.invoke(settings, config != null && config.getBool(WebViewConfig.ENABLE_WEB_PLUGINS));
        } catch (Throwable e) {
            Logger.W(TAG, "WebSettings.setPluginsEnabled(bool) is removed from API by Google.");
        }
    }
    
    private String updateRevesionOfCustomUA(String defaultUA, String customUA) {
    	String deviceInfo = android.os.Build.BRAND + ","
				+ android.os.Build.VERSION.RELEASE + "; "
				+ Locale.getDefault().toString() + "; " + android.os.Build.MODEL;
		int index = Build.FINGERPRINT.indexOf(android.os.Build.VERSION.RELEASE);
		String[] buildInfo = Build.FINGERPRINT.substring(index).split("/");
		deviceInfo = deviceInfo + " Build/" + buildInfo[1];
		customUA = customUA.replace("U; /%p", "U; " + deviceInfo);
		
		
		int start = defaultUA.indexOf("AppleWebKit/") + 12;
		String temp = defaultUA.substring(start, defaultUA.length());
		customUA = customUA.replace(
				"AppleWebKit/%w",
				"AppleWebKit/"
						+ defaultUA.substring(start, start + temp.indexOf("(") - 1));

		int start1 = defaultUA.indexOf("Version/") + 8;
		String temp1 = defaultUA.substring(start1, defaultUA.length());
		customUA = customUA
				.replace(
						"Version/%e",
						"Version/"
								+ defaultUA.substring(start1,
										start1 + temp1.indexOf(" ")));

		int start2 = defaultUA.indexOf("Safari/") + 7;
		defaultUA.substring(start2, defaultUA.length());
		customUA = customUA.replace("Safari/%w",
				"Safari/" + defaultUA.substring(start2, defaultUA.length()));
		return customUA;
    }
}
