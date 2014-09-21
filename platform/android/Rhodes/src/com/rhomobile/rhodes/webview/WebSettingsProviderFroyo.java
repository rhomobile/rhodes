package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoConfig;

public class WebSettingsProviderFroyo extends WebSettingsProviderEclairMR1 implements IWebSettingsProvider {
    @Override
    protected void setWebPlugins(WebSettings settings, IRhoConfig config) {
        if(config != null && config.getBool(WebViewConfig.ENABLE_WEB_PLUGINS, WebViewConfig.ENABLE_WEB_PLUGINS_DEF)) {
            settings.setPluginState(WebSettings.PluginState.ON);
            Logger.I(TAG, "Set web plugins state 'ON'");
        }
        else {
            settings.setPluginState(WebSettings.PluginState.OFF);
            Logger.I(TAG, "Set web plugins state 'OFF'");
        }
    }
}