/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.webview;

import java.io.File;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.MalformedURLException;
import java.io.IOException;
import java.util.Map;
import java.util.HashMap;

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.mainview.TabbedMainView;
import com.rhomobile.rhodes.webview.WebViewConfig;
import com.rhomobile.rhodes.socket.SSLImpl;

import android.graphics.Bitmap;
import android.net.Uri;
import android.net.http.SslError;
import android.view.Window;
import android.webkit.HttpAuthHandler;
import android.webkit.SslErrorHandler;
import android.webkit.URLUtil;
import android.webkit.WebResourceResponse;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.CookieSyncManager;
import android.webkit.ClientCertRequest;
import java.security.PrivateKey;
import java.security.cert.Certificate;
import java.security.cert.X509Certificate;


public class RhoWebViewClient extends WebViewClient
{
    private class HttpAuthResult implements IRhoExtension.IAuthRequest {
        private HttpAuthHandler authHandler;
        private String host;
        private String realm;
//        private boolean pending;
        
        public HttpAuthResult(HttpAuthHandler handler, String host, String realm) {
            authHandler = handler;
            this.host = host;
            this.realm = realm;
        }

        @Override
        public void proceed(String user, String pass) {
            authHandler.proceed(user, pass);
        }

        @Override
        public void cancel() {
            authHandler.cancel();
        }

        @Override
        public String host() {
            return this.host;
        }

        @Override
        public String realm() {
            return this.realm;
        }

//        @Override
//        public void setPending() {
//            pending = true;
//        }
//
//        boolean isPending() {
//            return pending;
//        }

        @Override
        public String type() {
            return "http";
        }

    }

    private final String TAG = RhoWebViewClient.class.getSimpleName();
    private GoogleWebView mWebView;
    private static boolean ourIsDoNotTrack = false;


    public static setDoNotTrack(boolean dnt) {
        ourIsDoNotTrack = dnt;
    }


    public RhoWebViewClient(GoogleWebView webView) {
        mWebView = webView;
    }

//    @Override
//    public WebResourceResponse shouldInterceptRequest(WebView view, String url) {
//        Logger.I(TAG, "Resource request: " + url);
//        return null;
//    }
//
    public void onLoadResource(WebView view, String url) {
        Logger.I(TAG, "Load resource" + url);

        RhoExtManager.getImplementationInstance().onLoadResource(view, url);
    }


    @Override
    public boolean shouldOverrideUrlLoading(WebView view, String url) {
        Logger.I(TAG, "Loading URL: " + url);

        //RhoElements implementation of "history:back"
        if(url.equalsIgnoreCase("history:back")) {
        	Logger.I(TAG, "history:back");
        	view.goBack();
        	return true;
        }
        else if(url.equalsIgnoreCase("history:twiceback"))
        {
            Logger.I(TAG, "history:twiceback");
        	view.goBack();
		view.goBack();
        	return true;
        }

        if (url.contains(".HTM"))
        {
    	     url=url.replace(".HTML", ".html");
    	     url=url.replace(".HTM", ".htm");
    	     Logger.I(TAG, "Changed to lower case html, url="+ url);
    	}




        boolean res = RhodesService.getInstance().handleUrlLoading(url);
        if (!res) {
            Logger.profStart("BROWSER_PAGE");

            RhoExtManager.getImplementationInstance().onBeforeNavigate(view, url);

            Uri localUri = LocalFileProvider.overrideUri(Uri.parse(url));
            if (localUri != null) {
                url = Uri.decode(localUri.toString());
                Logger.T(TAG, "Override URL: " + url);
                view.loadUrl(url);
                return true;
            }
            else {
                if (ourIsDoNotTrack) {
                    HashMap<String, String> extraHeaders = new HashMap<String, String>();
                    extraHeaders.put("DNT", "1");
                    view.loadUrl(url, extraHeaders);
                    return true;
                }
            }
        }
        return res;
    }

    @Override
    public void onPageStarted(WebView view, String url, Bitmap favicon) {
        super.onPageStarted(view, url, favicon);

        RhoExtManager.getImplementationInstance().onNavigateStarted(view, url);

        if (mWebView.getConfig() != null && mWebView.getConfig().getBool(WebViewConfig.ENABLE_PAGE_LOADING_INDICATION))
            RhodesActivity.safeGetInstance().getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0);
    }

    @Override
    public void onPageFinished(WebView view, String url) {

        Logger.profStop("BROWSER_PAGE");

        // Set title
       // String title = view.getTitle();
        try {
            //RhodesActivity.safeGetInstance().setTitle(title);
	checkTitleDataContain(view);
            if (mWebView.getConfig() != null && mWebView.getConfig().getBool(WebViewConfig.ENABLE_PAGE_LOADING_INDICATION))
                RhodesActivity.safeGetInstance().getWindow().setFeatureInt(Window.FEATURE_PROGRESS, RhodesActivity.MAX_PROGRESS);
        } catch (Throwable ex) {
            //Do nothing. Just for case if activity has been destroyed in between.
        }
        if ((url.trim().equalsIgnoreCase("data:text/html,")
				&& view.getTitle() != null && view.getTitle().equalsIgnoreCase(
				"data:text/html,"))
				|| (url.trim().equalsIgnoreCase("data:,") && view.getTitle() == null)) {
			String temp = RhodesActivity.safeGetInstance().getMainView().getTabDefaultUrl();
			if(temp != null && !temp.isEmpty()) {
				url = temp;
				view.loadUrl(url);
			}
        }
        RhoExtManager.getImplementationInstance().onNavigateComplete(view, url);
        //CookieSyncManager.getInstance().sync();

        super.onPageFinished(view, url);
    }

    @Override
    public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
        super.onReceivedError(view, errorCode, description, failingUrl);
        
        Logger.profStop("BROWSER_PAGE");
        
        StringBuilder msg = new StringBuilder(failingUrl != null ? failingUrl : "null");
        msg.append(" failed: ");
        msg.append(errorCode);
        msg.append(" - " + description);
        Logger.E(TAG, msg.toString());

        RhoExtManager.getImplementationInstance().onLoadError(view, IRhoExtension.LoadErrorReason.INTERNAL_ERROR);
    }

    @Override 
    public void onReceivedClientCertRequest(WebView view, ClientCertRequest request)
    {
        try {
            //SSLImpl.loadLocalCientP12Bundle();
            PrivateKey pkey = SSLImpl.getClientPrivateKey();
            Certificate[] cert_chain = SSLImpl.getClientCertChain();
            if(pkey != null && cert_chain != null)
            {
                request.proceed(pkey, (X509Certificate[])cert_chain);
            }
            else
                request.cancel();

        } catch (Throwable  e) {
            request.cancel();
        }
        
    }

    @Override
    public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {

        if (RhodesService.isLocalHttpsServerEnable()) {
            String url = error.getUrl();
            Uri uri = Uri.parse(url);
            String protocol = uri.getScheme();
            String server = uri.getAuthority();

            String baseUrl = RhodesService.getBaseUrl();
            Uri baseUri = Uri.parse(baseUrl);
            String base_server = baseUri.getAuthority();
            String base_protocol = baseUri.getScheme();
            if(base_server.equalsIgnoreCase(server) && protocol.equalsIgnoreCase(base_protocol))
            {
                if(error.getPrimaryError() == SslError.SSL_UNTRUSTED)
                {
                    handler.proceed();
                    return;
                }
            }
        }

        if(RhoConf.getBool("no_ssl_verify_peer")) {
            Logger.D(TAG, "Skip SSL error.");
            handler.proceed();
        } else {
            StringBuilder msg = new StringBuilder();
            msg.append("SSL error - ");
            switch(error.getPrimaryError()) {
            case SslError.SSL_NOTYETVALID:
                msg.append("The certificate is not yet valid: ");
                break;
            case SslError.SSL_EXPIRED:
                msg.append("The certificate has expired: ");
                break;
            case SslError.SSL_IDMISMATCH:
                msg.append("Hostname mismatch: ");
                break;
            case SslError.SSL_UNTRUSTED:
                msg.append("The certificate authority is not trusted: ");
                break;
            }
            msg.append(error.getCertificate().toString());
            Logger.W(TAG, msg.toString());
            handler.cancel();
        }
    }

    //@Override
    //public WebResourceResponse shouldInterceptRequest(WebView view, WebResourceRequest request) {
    //    return null;
    //}
    
    @Override
    public void onReceivedHttpAuthRequest (WebView view, HttpAuthHandler handler, String host, String realm) {
    	IRhoConfig rhoelementsGetConfig =  RhoExtManager.getInstance().getConfig("rhoelementsext");
		if(rhoelementsGetConfig != null){
			HttpAuthResult authResult = new HttpAuthResult(handler, host, realm);
    		String password = rhoelementsGetConfig.getString(WebViewConfig.AUTH_PASSWORD);
    		String username = rhoelementsGetConfig.getString(WebViewConfig.AUTH_USERNAME);
    		if(username != null && password !=null){
				if(password.length() > 0 &&  username.length() > 0){
		    		 username = rhoelementsGetConfig.getString(WebViewConfig.AUTH_USERNAME);
		    		 password = rhoelementsGetConfig.getString(WebViewConfig.AUTH_PASSWORD);
		    		  authResult.proceed(username,password);
				}else{
					  RhoExtManager.getImplementationInstance().onAuthRequest(view, authResult);
				}
	    	}else{
	    		  RhoExtManager.getImplementationInstance().onAuthRequest(view, authResult);
	    	}
		}else{
			HttpAuthResult authResult = new HttpAuthResult(handler, host, realm);
			RhoExtManager.getImplementationInstance().onAuthRequest(view, authResult);
		}
      }
     private void checkTitleDataContain(WebView view){
    	// Set title
        String title = view.getTitle();
    	if(! (title.equalsIgnoreCase("data:text/html,") || title.equalsIgnoreCase("data:,") 
    			|| title.contains("about:blank"))){
    		RhodesActivity.safeGetInstance().setTitle(title);
    	}
    }
    
}
