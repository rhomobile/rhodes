package com.rho.webviewclientsslauth;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class WebViewClientSSLAuthFactory
        extends RhoApiFactory< WebViewClientSSLAuth, WebViewClientSSLAuthSingleton>
        implements IWebViewClientSSLAuthFactory {

    @Override
    protected WebViewClientSSLAuthSingleton createSingleton() {
        return new WebViewClientSSLAuthSingleton(this);
    }

    @Override
    protected WebViewClientSSLAuth createApiObject(String id) {
        return new WebViewClientSSLAuth(id);
    }
}
