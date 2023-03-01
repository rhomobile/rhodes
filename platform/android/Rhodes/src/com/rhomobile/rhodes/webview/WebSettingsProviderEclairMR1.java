package com.rhomobile.rhodes.webview;

import android.content.Context;
import android.webkit.WebSettings;

import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.util.ContextFactory;

public class WebSettingsProviderEclairMR1 extends WebSettingsProviderEclair implements IWebSettingsProvider {
    public void fillSettings(WebSettings settings, IRhoConfig config) {
        super.fillSettings(settings, config);
        //settings.setAppCacheMaxSize(5 * 1048576);
        //String pathToCache = ContextFactory.getAppContext().getDir("database", Context.MODE_PRIVATE).getPath();
        //settings.setAppCachePath(pathToCache);
        //settings.setAppCacheEnabled(true);

        settings.setDomStorageEnabled(true);
    }
}