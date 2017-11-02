package com.rhomobile.rhodes.webview;

import java.io.File;
import java.io.FileOutputStream;

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.osfunctionality.AndroidFunctionalityManager;
import com.rhomobile.rhodes.osfunctionality.OsVersionManager;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.RhoConf;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Picture;
import android.net.Uri;
import android.os.Build;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.webkit.WebBackForwardList;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebViewClient;

public class GoogleWebView implements IRhoWebView {
    private static final String TAG = GoogleWebView.class.getSimpleName(); 

    private WebChromeClient mChromeClient;
    private WebViewClient mWebViewClient;
    private static Boolean mInitialized = false;

    private android.webkit.WebView mWebView;
    private ViewGroup mContainerView;
    private TextZoom mTextZoom = TextZoom.NORMAL;
    private IRhoConfig mConfig;

    public GoogleWebView(Activity activity) {
        mWebView = new android.webkit.WebView(activity);
        mWebView.getSettings().setLoadWithOverviewMode(true);
        mWebView.getSettings().setUseWideViewPort(true);

        synchronized(mInitialized) {
            if (!mInitialized) {
                mWebView.clearCache(true);
                initWebStuff(activity);
            }
        }
        mWebViewClient = new RhoWebViewClient(this);
        mChromeClient = new RhoWebChromeClient(activity, this);
    }

    private static void initWebStuff(Activity activity) {

        Logger.I(TAG, "Initialize Google WEbView staff");
        
        OsVersionManager.registerSelector(IWebSettingsProvider.class, WebSettingsProviderBase.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.ECLAIR, IWebSettingsProvider.class, WebSettingsProviderEclair.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.ECLAIR_MR1, IWebSettingsProvider.class, WebSettingsProviderEclairMR1.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.FROYO, IWebSettingsProvider.class, WebSettingsProviderFroyo.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.HONEYCOMB_MR1, IWebSettingsProvider.class, WebSettingsProviderHoneycombMR1.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.JELLY_BEAN, IWebSettingsProvider.class, WebSettingsProviderJellyBean.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.KITKAT, IWebSettingsProvider.class, WebSettingsProviderKitKat.class.getCanonicalName());

        mInitialized = true;
    }
    
    private void applyWebSettings() {
        Logger.I(TAG, "applyWebSettings");
//        PerformOnUiThread.exec(new Runnable() {
//            @Override
//            public void run() {
                Logger.I(TAG, "Web settings is applying now  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                
                double z = getConfig().getDouble(WebViewConfig.PAGE_ZOOM);
                mWebView.setInitialScale((int)(z * 100));
                mWebView.setVerticalScrollBarEnabled(true);
                mWebView.setHorizontalScrollBarEnabled(true);
                mWebView.setVerticalScrollbarOverlay(true);
                mWebView.setHorizontalScrollbarOverlay(true);
                mWebView.setFocusableInTouchMode(true);
		int sdkVersion = Build.VERSION.SDK_INT;
    		  /*  if(sdkVersion <19){
    		    	  mWebView.getSettings().setLoadWithOverviewMode(true);
    		    	  mWebView.getSettings().setUseWideViewPort(true);
    		    }*/
    		    
                IWebSettingsProvider provider = OsVersionManager.getFeature(IWebSettingsProvider.class);
                provider.fillSettings(mWebView.getSettings(), mConfig);
                
                RhodesActivity.safeGetInstance().notifyUiCreated();
//            }
//        });
    }

    @Override
    public void setWebClient() {
        PerformOnUiThread.exec(new Runnable() {
            @Override
            public void run() {
                Logger.I(TAG, "Setting RhoWebChromeClient and RhoWebViewClient");
                mWebView.setWebChromeClient(mChromeClient);
                mWebView.setWebViewClient(mWebViewClient);
            }
        });
    }
    
    @Override
    public void setConfig(IRhoConfig config) {
        mConfig = config;
        applyWebSettings();
    }
    
    public IRhoConfig getConfig() {
        return mConfig;
    }

    @Override
    public View getView() {
        return mWebView;
    }

    @Override
    public void setContainerView(ViewGroup view) {
        mContainerView = view;
    }

    @Override
    public ViewGroup getContainerView() {
        return mContainerView;
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
        Logger.profStart("BROWSER_PAGE");
        
        RhoExtManager.getImplementationInstance().onBeforeNavigate(mWebView, url);

        Uri uri = LocalFileProvider.overrideUri(Uri.parse(url));
        if (uri != null) {
            url = Uri.decode(uri.toString());
            Logger.T(TAG, "Override URL: " + url);
        }
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
    public void setZoom(double scale) {
        mWebView.setInitialScale((int)(scale * 100));
    }

    @Override
    public void setTextZoom(TextZoom zoom) {
        WebSettings.TextSize googleTextZoom = WebSettings.TextSize.NORMAL;
        mTextZoom = TextZoom.NORMAL;
        if (zoom == TextZoom.SMALLEST)
        {
            googleTextZoom = WebSettings.TextSize.SMALLEST;
            mTextZoom = TextZoom.SMALLEST;
        }
        else if (zoom == TextZoom.SMALLER)
        {
            googleTextZoom = WebSettings.TextSize.SMALLER;
            mTextZoom = TextZoom.SMALLER;
        }
        else if (zoom == IRhoWebView.TextZoom.NORMAL)
        {
            googleTextZoom = WebSettings.TextSize.NORMAL;
            mTextZoom = TextZoom.NORMAL;
        }
        else if (zoom == IRhoWebView.TextZoom.LARGER)
        {
            googleTextZoom = WebSettings.TextSize.LARGER;
            mTextZoom = TextZoom.LARGER;
        }
        else if (zoom == IRhoWebView.TextZoom.LARGEST)
        {
            googleTextZoom = WebSettings.TextSize.LARGEST;
            mTextZoom = TextZoom.LARGEST;
        }
        mWebView.getSettings().setTextSize(googleTextZoom);
    }
    
    @Override
    public TextZoom getTextZoom()
    {
	return mTextZoom;
    }

    @Override
    public String getEngineId() {
        return "WEBKIT/GOOGLE/" + Build.VERSION.RELEASE;
    }

    @Override
    public void onPause() {
        //AndroidFunctionalityManager.getAndroidFunctionality().pauseWebView(mWebView,true);
    }

    @Override
    public void onResume() {
        //AndroidFunctionalityManager.getAndroidFunctionality().pauseWebView(mWebView,false);
    }

    @Override
    public void destroy() {
        mWebView.destroy();
    }
    
    @Override
    public WebBackForwardList copyBackForwardList() {
    	return mWebView.copyBackForwardList();
    }

    @Override
    public void capture(CaptureFormat format, String path) {
        switch (format) {
        /*case CAPTURE_FORMAT_HTML:
            Logger.T(TAG, "Capturing current page as HTML archive: " + path);
            mWebView.saveWebArchive(path);
            break;*/
        case CAPTURE_FORMAT_JPEG:
            Logger.T(TAG, "Capturing current page as JPEG image: " + path);
            saveJpeg(path);
            break;
        default:
            //Should never happen
            Logger.E(TAG, "Wrong capture format.");
            break;
        }
    }
    
    private void saveJpeg(String path) {
        Picture picture = mWebView.capturePicture();
        
        if ( ( picture == null ) || ( picture.getWidth() == 0 ) || picture.getHeight() == 0 ) {
            Logger.E(TAG, "Can't capture picture from WebView.");
            return;
        }
        
        Bitmap bitmap = Bitmap.createBitmap( picture.getWidth(), picture.getHeight(), Bitmap.Config.ARGB_8888); 
        Canvas canvas = new Canvas(bitmap); 
        picture.draw(canvas); 
        FileOutputStream fos = null; 
        try { 
            fos = new FileOutputStream(path); 
            if ( fos != null ) 
            { 
                bitmap.compress(Bitmap.CompressFormat.JPEG, 90, fos ); 
                fos.close(); 
            } 
        }
        catch (Throwable e){
            Logger.E(TAG, e);
        }
    }

    @Override
    public void addJSInterface(Object obj, String name) {
        Logger.I(TAG, "Adding new JS interface: " + name);
        mWebView.addJavascriptInterface(obj, name);
    }
}
