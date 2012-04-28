package com.rhomobile.rhodes.webview;

import java.lang.reflect.Constructor;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.osfunctionality.AndroidFunctionalityManager;

import android.app.Activity;
import android.os.Build;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebViewClient;

public class GoogleWebView implements IRhoWebView {

    private static WebChromeClient mChromeClient;
    private static WebViewClient mWebViewClient;
    //private static RhoWebSettings mWebSettings;
    private static Boolean mInitialized = false;

    private android.webkit.WebView mWebView; 

    public GoogleWebView(Activity activity) {
        synchronized(mInitialized) {
            if (!mInitialized) {
                initWebStuff(activity);
            }
        }
        mWebView = new android.webkit.WebView(activity);
        applyWebSettings();
        mWebView.setWebChromeClient(mChromeClient);
        mWebView.setWebViewClient(mWebViewClient);
        mWebView.clearCache(true);
    }

    private static void initWebStuff(Activity activity) {
        String ccName;
        //String wsName;
        int sdkVersion = Build.VERSION.SDK_INT;//Integer.parseInt(Build.VERSION.SDK);
        if (sdkVersion < Build.VERSION_CODES.ECLAIR_MR1) {
            ccName = "ChromeClientOld";
            //wsName = "RhoWebSettingsOld";
        }
        else {
            ccName = "ChromeClientNew";
            //wsName = "RhoWebSettingsNew";
        }
        
        try {
            String pkgname = ChromeClientOld.class.getPackage().getName();
            String fullName = pkgname + "." + ccName;
            Class<? extends WebChromeClient> ccClass =
                Class.forName(fullName).asSubclass(WebChromeClient.class);
            
            Constructor<? extends WebChromeClient> ctor = ccClass.getConstructor(RhodesActivity.class);
            mChromeClient = ctor.newInstance(activity);
            
            //pkgname = RhoWebSettings.class.getPackage().getName();
            //fullName = pkgname + "." + wsName;
            //Class<? extends RhoWebSettings> wsClass =
            //    Class.forName(fullName).asSubclass(RhoWebSettings.class);
            //mWebSettings = wsClass.newInstance();
            
            mWebViewClient = new RhoWebViewClient();
            
            mInitialized = true;
        }
        catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }
    
    public void applyWebSettings() {
        //mWebSettings.setWebSettings(mWebView);
        mWebView.setVerticalScrollBarEnabled(true);
        mWebView.setHorizontalScrollBarEnabled(true);
        mWebView.setVerticalScrollbarOverlay(true);
        mWebView.setHorizontalScrollbarOverlay(true);
        mWebView.setFocusableInTouchMode(true);
        AndroidFunctionalityManager.getAndroidFunctionality().applyWebSettings(mWebView);
    }

    @Override
    public View getView() {
        return mWebView;
    }

    @Override
    public boolean canGoBack() {
        return mWebView.canGoBack();
    }

    @Override
    public void goBack() {
        mWebView.goBack();
    }

    @Override
    public void goForward() {
        mWebView.goForward();
    }

    @Override
    public void reload() {
        mWebView.reload();
    }

    @Override
    public void clear() {
        mWebView.clearView();
        mWebView.clearCache(true);
        mWebView.invalidate();
        mWebView.loadData("", "", "");
    }

    @Override
    public String getUrl() {
        return mWebView.getUrl();
    }

    @Override
    public void loadUrl(String url) {
        mWebView.loadUrl(url);
    }

    @Override
    public void loadData(String data, String mime, String encoding) {
        mWebView.loadData(data, mime, encoding);
    }

    @Override
    public void loadDataWithBaseURL(String baseUrl, String data, String mimeType, String encoding, String historyUrl) {
        mWebView.loadDataWithBaseURL(baseUrl, data, mimeType, encoding, historyUrl);
    }

    @Override
    public void stopLoad() {
        mWebView.stopLoading();
    }

    @Override
    public void setZoom(int scale) {
        // TODO Auto-generated method stub
    }

    @Override
    public void setTextZoom(int scale) {
        //mWebView.getSettings().setTextZoom(scale);
    }

    @Override
    public String getEngineId() {
        return "WEBKIT/GOOGLE/" + Build.VERSION.RELEASE;
    }
}
