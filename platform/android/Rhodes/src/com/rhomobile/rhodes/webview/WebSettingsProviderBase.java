package com.rhomobile.rhodes.webview;

import java.lang.reflect.Method;

import android.annotation.SuppressLint;
import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoConfig;

public class WebSettingsProviderBase implements IWebSettingsProvider {
    protected static final String TAG = "WebSettingsProvider";
    
    @SuppressLint("SetJavaScriptEnabled")
    public void fillSettings(WebSettings settings, IRhoConfig config) {
        
        boolean enableZoom = config == null || config.getBool(WebViewConfig.ENABLE_ZOOM, WebViewConfig.ENABLE_ZOOM_DEF);
        boolean enableCache = config == null || config.getBool(WebViewConfig.ENABLE_CACHE, WebViewConfig.ENABLE_CACHE_DEF);
        
        settings.setSavePassword(false);
        settings.setSaveFormData(false);
        settings.setJavaScriptEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(false);
        settings.setSupportZoom(enableZoom);
        settings.setBuiltInZoomControls(enableZoom);
        
        Logger.T(TAG, "Enable Zoom: " + enableZoom);
        
        if (enableCache) {
            settings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
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
            methodSetPlugins.invoke(settings, config != null && config.getBool(WebViewConfig.ENABLE_WEB_PLUGINS, WebViewConfig.ENABLE_WEB_PLUGINS_DEF));
        } catch (Throwable e) {
            Logger.W(TAG, "WebSettings.setPluginsEnabled(bool) is removed from API by Google.");
        }
    }
}