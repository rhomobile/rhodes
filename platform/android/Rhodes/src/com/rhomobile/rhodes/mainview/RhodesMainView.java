package com.rhomobile.rhodes.mainview;

import java.lang.reflect.Constructor;

import android.content.Context;
import android.os.Build;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.webview.ChromeClientOld;
import com.rhomobile.rhodes.webview.RhoWebSettings;
import com.rhomobile.rhodes.webview.RhoWebViewClient;

public abstract class RhodesMainView implements MainView {

    protected static WebChromeClient mChromeClient;
    protected static WebViewClient mWebViewClient;
    protected static RhoWebSettings mWebSettings;
    private static Boolean mInitialized = false;

    public RhodesMainView(RhodesActivity activity) {
        synchronized(mInitialized) {
            if (!mInitialized) {
                initWebStuff(activity);
            }
        }
    }

    private static void initWebStuff(RhodesActivity activity) {
        String ccName;
        String wsName;
        int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
        if (sdkVersion < Build.VERSION_CODES.ECLAIR_MR1) {
            ccName = "ChromeClientOld";
            wsName = "RhoWebSettingsOld";
        }
        else {
            ccName = "ChromeClientNew";
            wsName = "RhoWebSettingsNew";
        }
        
        try {
            String pkgname = ChromeClientOld.class.getPackage().getName();
            String fullName = pkgname + "." + ccName;
            Class<? extends WebChromeClient> ccClass =
                Class.forName(fullName).asSubclass(WebChromeClient.class);
            
            Constructor<? extends WebChromeClient> ctor = ccClass.getConstructor(RhodesActivity.class);
            mChromeClient = ctor.newInstance(activity);
            
            pkgname = RhoWebSettings.class.getPackage().getName();
            fullName = pkgname + "." + wsName;
            Class<? extends RhoWebSettings> wsClass =
                Class.forName(fullName).asSubclass(RhoWebSettings.class);
            mWebSettings = wsClass.newInstance();
            
            mWebViewClient = new RhoWebViewClient();
            
            mInitialized = true;
        }
        catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }

    protected static WebView createWebView(Context context) {
        WebView view = new WebView(context);
        mWebSettings.setWebSettings(view);
        view.setWebChromeClient(mChromeClient);
        view.setWebViewClient(mWebViewClient);
        view.clearCache(true);
        
        return view;
    }
    
    public abstract WebView getGoogleWebView(int tab);
    public View getWebView(int tab) { return getGoogleWebView(tab); }
}
