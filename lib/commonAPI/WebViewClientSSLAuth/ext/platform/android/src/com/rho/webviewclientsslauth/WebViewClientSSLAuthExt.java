package com.rho.webviewclientsslauth;

import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.RhoWebChromeClient;
import com.rhomobile.rhodes.webview.RhoWebViewClient;

import com.rhomobile.rhodes.RhodesActivity;

import android.webkit.ClientCertRequestHandler;
import android.webkit.WebView;
import android.webkit.WebChromeClient;
import android.webkit.WebViewClient;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;

import com.rhomobile.rhodes.api.MethodResult;

import android.app.Activity;

import com.rho.webviewclientsslauth.IWebViewClientSSLAuthSingleton;

import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.security.cert.Certificate;

import java.io.FileInputStream;
import java.io.InputStream;

import java.io.File;
import java.io.IOException;

public final class WebViewClientSSLAuthExt extends AbstractRhoExtension {

    private static final String TAG = "WebViewClientSSLAuthExt";

    private final class ClientAuthWebViewClient extends RhoWebViewClient
    {

      private static final String TAG = "ClientAuthWebViewClient";

      public ClientAuthWebViewClient(GoogleWebView webview) {
        super(webview);
      }

		  public void onReceivedClientCertRequest(WebView view,	ClientCertRequestHandler handler, String host_and_port) {

        Logger.I(TAG, "onReceivedClientCertRequest");

        IWebViewClientSSLAuthSingleton s = WebViewClientSSLAuthFactorySingleton.getInstance().getApiSingleton();

        String clientCertPath = null;
        if ( s != null ) {
          MethodResult r = new MethodResult(false);
          s.getClientKeyPath(r);
          clientCertPath = r.getString();
        }

        if ( (clientCertPath == null) && (clientCertPath.length() == 0) ) {
          handler.cancel();
          return;
        }

       	Logger.I(TAG, "clientSSLCertificate is " + clientCertPath );

        MethodResult r = new MethodResult(false);
        s.getClientKeyPassword(r);
        String password = r.getString();
        if ( null == password ) {
          password = "";
        }

   			KeyStore truststore;
   			InputStream in = null;

   			try {
   				truststore = KeyStore.getInstance("pkcs12");
   				in = new FileInputStream( new File(clientCertPath) );
		      
   				truststore.load(in, password.toCharArray());				
   				String alias = truststore.aliases().nextElement();
				
   				PrivateKey key = (PrivateKey)truststore.getKey(alias, password.toCharArray());
   				Certificate[] cert = truststore.getCertificateChain(alias);
				
   				X509Certificate[] chain = new X509Certificate[cert.length];
   				for ( int i = 0; i < cert.length; ++i ) {
   					chain[i] = (X509Certificate)cert[i];
   				}
				
         	Logger.I(TAG, "Proceeding page load with client certificate. Key alias: " + alias );
   				handler.proceed(key, chain);
		          
		    } catch (Exception e) {
  				e.printStackTrace();	
          handler.cancel();
  			}
	   		finally {
          try {
  					in.close();
	   			} catch (IOException e) {
  					e.printStackTrace();
  				}
		    }
  		}
   }

    private final class ClientAuthWebView extends GoogleWebView {

      private static final String TAG = "ClientAuthWebView";

      public ClientAuthWebView(Activity activity) {
        super(activity);
        mWebViewClient = new ClientAuthWebViewClient(this);
      }
    }

    @Override
    public IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex) {
      Logger.I(TAG, "onCreateWebView");
      return new ClientAuthWebView(RhodesActivity.getInstance());
    }
  
}
