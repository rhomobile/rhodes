package com.rhomobile.rhodes.webview;

import android.annotation.SuppressLint;
import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

public class WebSettingsProviderBase implements IWebSettingsProvider {
    protected static final String TAG = "WebSettingsProvider";
    
    @SuppressLint("SetJavaScriptEnabled")
    public void fillSettings(WebSettings settings, IRhoWebViewConfig config) {
        settings.setSavePassword(false);
        settings.setSaveFormData(false);
        settings.setJavaScriptEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(false);
        settings.setSupportZoom(config != null && config.getBool("enableZoom"));
        settings.setBuiltInZoomControls(config != null && config.getBool("enableZoom"));
        
        Logger.T(TAG, "Enable Zoom: " + (config != null && config.getBool("enableZoom")));
        
        if (config != null && config.getBool("enableCache")) {
            settings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
        } else {
            settings.setCacheMode(WebSettings.LOAD_NO_CACHE);
        }
        settings.setSupportMultipleWindows(false);

        setWebPlugins(settings, config);
    }
    
    @SuppressWarnings("deprecation")
    protected void setWebPlugins(WebSettings settings, IRhoWebViewConfig config) {
        settings.setPluginsEnabled(config != null && config.getBool("enableWebPlugins"));
        
        Logger.T(TAG, "Enable Web Plugins: " + settings.getPluginsEnabled());
    }
}