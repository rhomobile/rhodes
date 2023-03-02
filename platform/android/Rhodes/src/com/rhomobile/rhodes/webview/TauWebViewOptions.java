package com.rhomobile.rhodes.webview;


import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.Collection;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;

public class TauWebViewOptions {

    public final static String TAU_WEBVIEW_OPTIONS_DEFAUlT_MASK = "*";
    public final static String TAU_WEBVIEW_OPTIONS_DISABLE_KEYBOARD = "DisableKeyboard";
    public final static String TAU_WEBVIEW_OPTIONS_DISABLE_SCROLL = "DisableScroll";
    public final static String TAU_WEBVIEW_OPTIONS_INJECT_META_VIEWPORT = "InjectMetaViewport";

    private static final String TAG = TauWebViewOptions.class.getSimpleName();

    private static TauWebViewOptions ourInstance = null;

    private Map<String, Map> mOptions = new Hashtable<String, Map>();

    public static TauWebViewOptions getInstance() {
        if (ourInstance == null) {
            ourInstance = new TauWebViewOptions();
        }
        return ourInstance;
    }

    public void addUrlOptions(String mask, Map<String, String> options) {
        Logger.T(TAG, "$$$ addUrlOptions with mask = " + mask);
        Iterator<String> iter = options.keySet().iterator();
        while (iter.hasNext()) {
            String key = iter.next();
            String value = options.get(key);
            Logger.T(TAG, "$$$ addUrlOptions         ["+key+"] = ["+value+"]");
        }
        mOptions.put(mask, options);
    }

    public Map<String, String> getOptionsForUrl(String url) {
        Iterator<String> iter = mOptions.keySet().iterator();
        Map<String, String> def_opt = new Hashtable<String, String>();
        while (iter.hasNext()) {
            String key = iter.next();
            if (key.equals(TAU_WEBVIEW_OPTIONS_DEFAUlT_MASK)) {
                def_opt = (Map<String, String>)mOptions.get(key);
            }
            if (url.contains(key)) {
                return (Map<String, String>)mOptions.get(key);
            }
        }
        return def_opt;
    }

    public boolean isKeyboardDisabled(String url) {
        Map<String, String> opt = getOptionsForUrl(url);
        String value = opt.get(TAU_WEBVIEW_OPTIONS_DISABLE_KEYBOARD);
        if (value != null) {
            return value.equals("1");
        }
        return false;
    }

    public boolean isScrollDisabled(String url) {
        Map<String, String> opt = getOptionsForUrl(url);
        String value = opt.get(TAU_WEBVIEW_OPTIONS_DISABLE_SCROLL);
        if (value != null) {
            return value.equals("1");
        }
        return false;
    }

    public String getMetaViewport(String url) {
        Map<String, String> opt = getOptionsForUrl(url);
        String value = opt.get(TAU_WEBVIEW_OPTIONS_INJECT_META_VIEWPORT);
        if (value != null) {
            return value;
        }
        return null;
    }


}
