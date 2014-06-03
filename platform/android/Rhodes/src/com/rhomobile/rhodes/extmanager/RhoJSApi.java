package com.rhomobile.rhodes.extmanager;

import android.webkit.JavascriptInterface;

import com.rhomobile.rhodes.Logger;

public class RhoJSApi {
    private static final String TAG = RhoJSApi.class.getSimpleName();
    
    @JavascriptInterface
    public String apiCall(String json, boolean async) {
        Logger.D(TAG, "Execute JS hook: " + json);
        String res = RhoExtManagerImpl.nativeJSCallEntryPoint(json);
        Logger.D(TAG, "JS result: " + res);
        return res;
    }

}
