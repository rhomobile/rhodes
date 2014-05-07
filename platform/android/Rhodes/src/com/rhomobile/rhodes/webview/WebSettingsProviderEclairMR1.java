package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

import android.content.Context;
import android.webkit.WebSettings;
import com.rhomobile.rhodes.util.ContextFactory;

public class WebSettingsProviderEclairMR1 extends WebSettingsProviderEclair implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings, IRhoWebViewConfig config) {
        super.fillSettings(settings, config);
        settings.setAppCacheMaxSize(5 * 1048576);
        String pathToCache = ContextFactory.getContext().getDir("database", Context.MODE_PRIVATE).getPath();
        settings.setAppCachePath(pathToCache);
        settings.setAppCacheEnabled(true);

        settings.setDomStorageEnabled(true);
    }
}