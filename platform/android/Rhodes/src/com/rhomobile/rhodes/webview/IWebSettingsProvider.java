package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

import android.webkit.WebSettings;

public interface IWebSettingsProvider {
    void fillSettings(WebSettings settings, IRhoWebViewConfig config);
}