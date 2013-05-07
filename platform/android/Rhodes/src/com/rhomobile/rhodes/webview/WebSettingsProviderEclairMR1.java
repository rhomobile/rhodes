package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;

public class WebSettingsProviderEclairMR1 extends WebSettingsProviderEclair implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings) {
        super.fillSettings(settings);
        settings.setAppCacheEnabled(true);
        settings.setDomStorageEnabled(true);
    }
}