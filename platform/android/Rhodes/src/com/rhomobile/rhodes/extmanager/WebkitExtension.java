package com.rhomobile.rhodes.extmanager;

import android.content.Context;
import android.graphics.Rect;
import android.view.inputmethod.InputMethodManager;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.ContextFactory;

public class WebkitExtension extends AbstractRhoExtension implements IRhoExtension {
    public static final String EXTNAME = "MotorolaWebkit";
    private static final String TAG = WebkitExtension.class.getSimpleName();
    
    private static WebkitExtension mInstance;
    
    private Config mConfig;
    
    public static WebkitExtension getInstance() {
        return mInstance;
    }

    private WebkitExtension(Config config) {
        mConfig = config;
    }
    
    public Config getConfig() {
        return mConfig;
    }
    
    @Override
    public void onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect rect, IRhoExtData ext) {
        InputMethodManager imm = (InputMethodManager) ContextFactory.getUiContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (enabled) {
            imm.showSoftInput(ext.getWebView(), 0);
        } else {
            imm.hideSoftInputFromWindow(ext.getWebView().getApplicationWindowToken(), 0);
        }
    }

    @Override
    public void startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoExtData ext) {
    }

    @Override
    public void stopLocationUpdates(IRhoExtManager extManager, IRhoExtData ext) {
    }

    public static void registerWebkitExtension(Config config) {
        mInstance = new WebkitExtension(config);
        RhoExtManager.getInstance().registerExtension(EXTNAME, mInstance);
    }

}
