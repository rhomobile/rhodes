package com.rhomobile.rhodes.extmanager;


import android.view.View;
import android.view.ViewGroup;

public interface IRhoWebView {
    enum CaptureFormat { CAPTURE_FORMAT_HTML, CAPTURE_FORMAT_JPEG }
    enum TextZoom {SMALLEST, SMALLER, NORMAL, LARGER, LARGEST}
    
    View getView();
    void setContainerView(ViewGroup view);
    ViewGroup getContainerView();
    void setWebClient();
    void setConfig(IRhoWebViewConfig config);
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
    void setZoom(double scale);
    void setTextZoom(TextZoom zoom);
    TextZoom getTextZoom();
    String getEngineId();
    void onPause();
    void onResume();
    void destroy();
    void addJSInterface(Object obj, String name);
    
    void capture(CaptureFormat format, String path);
    void disableScroll(boolean fingerToScroll);
}
