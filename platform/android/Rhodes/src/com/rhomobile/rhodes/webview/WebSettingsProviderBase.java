package com.rhomobile.rhodes.webview;

import android.annotation.SuppressLint;
import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;

public class WebSettingsProviderBase implements IWebSettingsProvider {
    protected static final String TAG = "WebSettingsProvider";
    
    @SuppressLint("SetJavaScriptEnabled")
    public void fillSettings(WebSettings settings) {
        settings.setSavePassword(false);
        settings.setSaveFormData(false);
        settings.setJavaScriptEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(false);
        if (RhoConf.getBool("enable_screen_zoom")) {
            settings.setSupportZoom(true);
            settings.setBuiltInZoomControls(true);
            Logger.I(TAG, "Enable WebView zoom");
        } else {
            settings.setSupportZoom(false);
            settings.setBuiltInZoomControls(false);
            Logger.I(TAG, "Disable WebView zoom");
        }
        settings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
        settings.setSupportMultipleWindows(false);
        setWebPlugins(settings);
    }
    
    @SuppressWarnings("deprecation")
    protected void setWebPlugins(WebSettings settings) {
        if(RhoConf.getBool("enable_web_plugins")) {
            settings.setPluginsEnabled(true);
            Logger.I(TAG, "Set web plugins enabled");
        }
        else {
            settings.setPluginsEnabled(false);
            Logger.I(TAG, "Set web plugins disabled");
        }
    }
        
}