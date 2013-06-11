package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

public class WebSettingsProviderJellyBean extends WebSettingsProviderFroyo implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings, IRhoWebViewConfig config) {
        super.fillSettings(settings, config);
        Logger.I(TAG, "Allow universal access from file URL");
        settings.setAllowUniversalAccessFromFileURLs(true);
    }
}