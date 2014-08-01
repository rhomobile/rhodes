package com.rho.webviewclientsslauth;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

import com.rhomobile.rhodes.RhoConf;

class WebViewClientSSLAuthSingleton extends WebViewClientSSLAuthSingletonBase implements IWebViewClientSSLAuthSingleton {
    public WebViewClientSSLAuthSingleton(WebViewClientSSLAuthFactory factory) {
        super();
    }

    public void getClientKeyPath(IMethodResult result) {
      result.set( RhoConf.getString("clientSSLCertificate") );
    }

    public void setClientKeyPath(String clientKeyPath, IMethodResult result) {
      RhoConf.setString("clientSSLCertificate", clientKeyPath);
    }

    public void getClientKeyPassword(IMethodResult result) {
      result.set( RhoConf.getString("clientSSLCertificatePassword") );
    }

    public void setClientKeyPassword(String clientKeyPassword, IMethodResult result) {
      RhoConf.setString("clientSSLCertificatePassword", clientKeyPassword);
    }
}