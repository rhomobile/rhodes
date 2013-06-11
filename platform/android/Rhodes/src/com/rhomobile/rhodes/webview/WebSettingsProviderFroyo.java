package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

public class WebSettingsProviderFroyo extends WebSettingsProviderEclairMR1 implements IWebSettingsProvider {
    @Override
    protected void setWebPlugins(WebSettings settings, IRhoWebViewConfig config) {
        if(config != null && config.getBool("enableWebPlugins")) {
            settings.setPluginState(WebSettings.PluginState.ON);
            Logger.I(TAG, "Set web plugins state 'ON'");
        }
        else {
            settings.setPluginState(WebSettings.PluginState.OFF);
            Logger.I(TAG, "Set web plugins state 'OFF'");
        }
    }
}