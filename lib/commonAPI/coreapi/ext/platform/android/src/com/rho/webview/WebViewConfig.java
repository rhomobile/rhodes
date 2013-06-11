package com.rho.webview;

import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.extmanager.IRhoWebViewConfig;

public class WebViewConfig implements IRhoWebViewConfig {

    Map<String, String> mConfigMap = new HashMap<String, String>();
    
    WebViewConfig() {
    }
    
    public void set(String name, boolean val) {
        mConfigMap.put(name, String.valueOf(val));
    }

    public void set(String name, String val) {
        mConfigMap.put(name, val);
    }

    @Override
    public boolean getBool(String name) {
        return Boolean.parseBoolean(mConfigMap.get(name));
    }

    @Override
    public int getInt(String name) {
        return Integer.parseInt(mConfigMap.get(name));
    }

    @Override
    public double getDouble(String name) {
        return Double.parseDouble(mConfigMap.get(name));
    }

    @Override
    public String getString(String name) {
        return mConfigMap.get(name);
    }


}
