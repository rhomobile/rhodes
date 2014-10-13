package com.rhomobile.rhodes.webview;

import android.webkit.CookieManager;
import android.webkit.WebSettings;

import com.rhomobile.rhodes.extmanager.IRhoConfig;


public class WebSettingsProviderHoneycombMR1 extends WebSettingsProviderFroyo {
    public void fillSettings(WebSettings settings, IRhoConfig config) {
        super.fillSettings(settings, config);
        CookieManager.setAcceptFileSchemeCookies(true);
    }

}
