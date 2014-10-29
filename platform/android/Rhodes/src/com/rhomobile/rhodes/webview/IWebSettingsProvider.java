package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.extmanager.IRhoConfig;

import android.webkit.WebSettings;

public interface IWebSettingsProvider {
    void fillSettings(WebSettings settings, IRhoConfig config);
}