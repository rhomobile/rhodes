package com.rho.webview;

public class WebViewFactory implements IWebViewFactory {

    WebViewSingleton mSingleton = new WebViewSingleton();

    @Override
    public IWebViewSingleton getApiSingleton() {
        return mSingleton;
    }

    @Override
    public IWebView getApiObject(String id) {
        return null;
    }
}
