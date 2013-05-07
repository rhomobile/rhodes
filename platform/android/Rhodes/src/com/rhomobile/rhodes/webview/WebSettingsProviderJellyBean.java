package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;

public class WebSettingsProviderJellyBean extends WebSettingsProviderFroyo implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings) {
        super.fillSettings(settings);
        Logger.I(TAG, "Allow universal access from file URL");
        settings.setAllowUniversalAccessFromFileURLs(true);
    }
}