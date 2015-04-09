package com.rhomobile.rhodes.webview;

import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoConfig;

public class WebViewConfig implements IRhoConfig {

    private static final String TAG = WebViewConfig.class.getSimpleName();

    public static final String ENABLE_PAGE_LOADING_INDICATION = "enablePageLoadingIndication";
    public static final String ENABLE_ZOOM = "enableZoom";
    public static final String ENABLE_WEB_PLUGINS = "enableWebPlugins";
    public static final String ENABLE_CACHE = "enableCache";
    public static final String PAGE_ZOOM = "pageZoom";

    public static final boolean ENABLE_PAGE_LOADING_INDICATION_DEF = true;
    public static final boolean ENABLE_ZOOM_DEF = true;
    public static final boolean ENABLE_WEB_PLUGINS_DEF = true;
    public static final boolean ENABLE_CACHE_DEF = true;
    public static final double  PAGE_ZOOM_DEF = 1.0;

    Map<String, String> mConfigMap;
    
    public WebViewConfig() {
        mConfigMap = new HashMap<String, String>();
        mConfigMap.put(ENABLE_PAGE_LOADING_INDICATION, String.valueOf(ENABLE_PAGE_LOADING_INDICATION_DEF));
        mConfigMap.put(ENABLE_ZOOM, String.valueOf(ENABLE_ZOOM_DEF));
        mConfigMap.put(PAGE_ZOOM, String.valueOf(PAGE_ZOOM_DEF));
        mConfigMap.put(ENABLE_WEB_PLUGINS, String.valueOf(ENABLE_WEB_PLUGINS_DEF));
        mConfigMap.put(ENABLE_CACHE, String.valueOf(ENABLE_CACHE_DEF));
    }
    
    public void set(String name, boolean val) {
        mConfigMap.put(name, String.valueOf(val));
    }

    public void set(String name, double val) {
        mConfigMap.put(name, String.valueOf(val));
    }

    public void set(String name, String val) {
        mConfigMap.put(name, val);
    }

    @Override
    public boolean isExist(String name) {
        return mConfigMap.containsKey(name);
    }

    @Override
    public boolean getBool(String name) {
        String str = mConfigMap.get(name);
        if ((str == null) || (str.length() == 0)) {
            throw new ValueNotFoundException(name);
        }
        return Boolean.parseBoolean(str);
    }

    @Override
    public int getInt(String name) {
        String str = mConfigMap.get(name);
        if ((str == null) || (str.length() == 0)) {
            throw new ValueNotFoundException(name);
        }
        try {
            return Integer.parseInt(str);
        } catch (NumberFormatException e) {
            throw new ValueNotFoundException(name, e);
        }
    }

    @Override
    public double getDouble(String name) {
        String str = mConfigMap.get(name);
        if ((str == null) || (str.length() == 0)) {
            throw new ValueNotFoundException(name);
        }
        try {
            return Double.parseDouble(str);
        } catch (NumberFormatException e) {
            throw new ValueNotFoundException(name, e);
        }
    }

    @Override
    public String getString(String name) {
        return mConfigMap.get(name);
    }


}
