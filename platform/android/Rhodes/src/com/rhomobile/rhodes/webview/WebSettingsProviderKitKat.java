package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;
import android.webkit.WebView;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

public class WebSettingsProviderKitKat extends
        WebSettingsProviderJellyBean implements IWebSettingsProvider {

    @Override
    public void fillSettings(WebSettings settings, IRhoWebViewConfig config) {
        super.fillSettings(settings, config);
        if (Capabilities.DEBUG_ENABLED) {
            WebView.setWebContentsDebuggingEnabled(true);
        }
    }

}
