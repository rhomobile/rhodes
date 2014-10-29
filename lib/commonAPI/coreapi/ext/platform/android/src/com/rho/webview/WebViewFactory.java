package com.rho.webview;

import com.rhomobile.rhodes.extmanager.IRhoExtManager;

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

    public void onCreateApplication(IRhoExtManager extManager) {
        WebViewFactorySingleton.setInstance(this);
        extManager.registerExtension("WebView", mSingleton);
    }
    

}
