package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

import android.webkit.WebSettings;

public class WebSettingsProviderEclairMR1 extends WebSettingsProviderEclair implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings, IRhoWebViewConfig config) {
        super.fillSettings(settings, config);
        settings.setAppCacheEnabled(true);
        settings.setDomStorageEnabled(true);
    }
}