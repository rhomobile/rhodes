package com.rhomobile.rhodes.extmanager;


import android.app.Activity;
import android.view.View;
import android.view.ViewGroup;

public interface IRhoWebView {
    View getView();
    void setContainerView(ViewGroup view);
    ViewGroup getContainerView();
    void setWebClient(Activity activity);
    boolean canGoBack();
    void goBack();
    void goForward();
    void reload();
    void clear();
    String getUrl();
    void loadUrl(String url);
    void loadData(String data, String mime, String encoding);
    void loadDataWithBaseURL(String baseUrl, String data, String mimeType, String encoding, String historyUrl);
    void stopLoad();
    void setZoom(int scale);
    void setTextZoom(int scale);
    String getEngineId();
    void onPause();
    void onResume();
}
