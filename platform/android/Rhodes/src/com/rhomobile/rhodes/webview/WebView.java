package com.rhomobile.rhodes.webview;

import android.view.View;

public interface WebView {
    View getView();
    boolean canGoBack();
    void goBack();
    void goForward();
    void reload();
    void clear();
    String getUrl();
    void loadUrl(String url);
    void loadData(String data, String mime, String encoding);
    void loadDataWithBaseURL(String baseUrl, String data, String mimeType, String encoding, String historyUrl);
}
