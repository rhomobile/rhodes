package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoConfig;

public class WebSettingsProviderJellyBean extends WebSettingsProviderHoneycombMR1 implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings, IRhoConfig config) {
        super.fillSettings(settings, config);
        Logger.I(TAG, "Allow universal access from file URL");
        settings.setAllowUniversalAccessFromFileURLs(true);
    }
}