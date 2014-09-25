package com.rhomobile.rhodes.webview;

import java.lang.reflect.Method;

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
            settings.setCacheMode(WebSettings.LOAD_DEFAULT);
        } else {
            settings.setCacheMode(WebSettings.LOAD_NO_CACHE);
        }
        settings.setSupportMultipleWindows(false);

        setWebPlugins(settings, config);
    }
    
    protected void setWebPlugins(WebSettings settings, IRhoWebViewConfig config) {
        
        try {
            Class<WebSettings> clazz = WebSettings.class;
            Method methodSetPlugins = clazz.getMethod("setPluginsEnabled", boolean.class);
            methodSetPlugins.invoke(settings, config != null && config.getBool("enableWebPlugins"));
        } catch (Throwable e) {
            Logger.W(TAG, "WebSettings.setPluginsEnabled(bool) is removed from API by Google.");
        }
    }
}
