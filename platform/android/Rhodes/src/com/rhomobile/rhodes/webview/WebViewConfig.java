package com.rhomobile.rhodes.webview;

import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.extmanager.IRhoConfig;

public class WebViewConfig implements IRhoConfig {

    private static final String TAG = WebViewConfig.class.getSimpleName();

    public static final String ENABLE_PAGE_LOADING_INDICATION = "enablePageLoadingIndication";
    public static final String ENABLE_ZOOM = "enableZoom";
    public static final String ENABLE_WEB_PLUGINS = "enableWebPlugins";
    public static final String ENABLE_CACHE = "enableCache";
    public static final String PAGE_ZOOM = "pageZoom";
    public static final String SETTING_SPLASHSCREEN_PATH = "splashscreenpath";
    public static final String SETTING_SPLASHSCREEN_DURATION = "splashscreenduration";
    public static final String USER_AGENT = "useragent";
    public static final String ENABLE_SCAN = "LowBatteryScan";
    public static final String ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE = "enableMediaPlaybackWithoutGesture";
    public static final String ENABLE_FUNCTION_KEY_F1 = "EnableFunctionKey_F1";
    public static final String ENABLE_FUNCTION_KEY_F2 = "EnableFunctionKey_F2";
    public static final String ENABLE_FUNCTION_KEY_F3 = "EnableFunctionKey_F3";
    public static final String ENABLE_FUNCTION_KEY_F4 = "EnableFunctionKey_F4";
    public static final String ENABLE_FUNCTION_KEY_F5 = "EnableFunctionKey_F5";
    public static final String ENABLE_FUNCTION_KEY_F6 = "EnableFunctionKey_F6";
    public static final String ENABLE_FUNCTION_KEY_F7 = "EnableFunctionKey_F7";
    public static final String ENABLE_FUNCTION_KEY_F8 = "EnableFunctionKey_F8";
    public static final String ENABLE_FUNCTION_KEY_F9 = "EnableFunctionKey_F9";
    public static final String ENABLE_FUNCTION_KEY_F10 = "EnableFunctionKey_F10";
    public static final String ENABLE_FUNCTION_KEY_F11 = "EnableFunctionKey_F11";
    public static final String ENABLE_FUNCTION_KEY_F12 = "EnableFunctionKey_F12";

    public static final String ENABLE_APPLICATION_KEY_A1 = "EnableApplicationKey_A1";
    public static final String ENABLE_APPLICATION_KEY_A2 = "EnableApplicationKey_A2";
    public static final String ENABLE_APPLICATION_KEY_A3 = "EnableApplicationKey_A3";
    public static final String ENABLE_APPLICATION_KEY_A4 = "EnableApplicationKey_A4";
    public static final String ENABLE_APPLICATION_KEY_A5 = "EnableApplicationKey_A5";
    public static final String ENABLE_APPLICATION_KEY_A6 = "EnableApplicationKey_A6";
    public static final String ENABLE_APPLICATION_KEY_A7 = "EnableApplicationKey_A7";
    public static final String ENABLE_APPLICATION_KEY_A8 = "EnableApplicationKey_A8";
    public static final String ENABLE_APPLICATION_KEY_A9 = "EnableApplicationKey_A9";
    public static final String ENABLE_APPLICATION_KEY_A10 = "EnableApplicationKey_A10";
    public static final String ENABLE_APPLICATION_KEY_A11 = "EnableApplicationKey_A11";
    public static final String ENABLE_APPLICATION_KEY_A12 = "EnableApplicationKey_A12";
    public static final String ENABLE_APPLICATION_KEY_A13 = "EnableApplicationKey_A13";
    public static final String ENABLE_APPLICATION_KEY_A14 = "EnableApplicationKey_A14";
    public static final String ENABLE_APPLICATION_KEY_A15 = "EnableApplicationKey_A15";
    public static final String ENABLE_APPLICATION_KEY_A16 = "EnableApplicationKey_A16";
    
	public static final boolean ENABLE_FUNCTION_KEY_F1_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F2_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F3_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F4_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F5_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F6_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F7_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F8_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F9_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F10_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F11_DEF = true;
	public static final boolean ENABLE_FUNCTION_KEY_F12_DEF = true;
	public static boolean IS_FUNCTIONS_KEY_DEF = true;
    public static final boolean ENABLE_PAGE_LOADING_INDICATION_DEF = true;
    public static final boolean ENABLE_ZOOM_DEF = true;
    public static final boolean ENABLE_WEB_PLUGINS_DEF = true;
    public static final boolean ENABLE_CACHE_DEF = true;
    public static final double  PAGE_ZOOM_DEF = 1.0;
    public static final String SETTING_SPLASHSCREEN_PATH_DEF = "apps/app/loading.png";
    public static final long SETTING_SPLASHSCREEN_DURATION_DEF = 0;
    public static final String USER_AGENT_DEF = RhoConf.getString("useragent");
    public static final boolean ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE_DEF = false;
   public static String AUTH_USERNAME ="username";
    public static String AUTH_PASSWORD = "password";
    public static String DISABLE_SCANNER_ON_NAVIGATION = "disablescanneronnavigation";
    public static int DISABLE_SCANNER_ON_NAVIGATION_DEFAULT = 1;
    Map<String, String> mConfigMap;
    
    public WebViewConfig() {
        mConfigMap = new HashMap<String, String>();
        mConfigMap.put(ENABLE_PAGE_LOADING_INDICATION, String.valueOf(ENABLE_PAGE_LOADING_INDICATION_DEF));
        mConfigMap.put(ENABLE_ZOOM, String.valueOf(ENABLE_ZOOM_DEF));
        mConfigMap.put(PAGE_ZOOM, String.valueOf(PAGE_ZOOM_DEF));
        mConfigMap.put(SETTING_SPLASHSCREEN_PATH, SETTING_SPLASHSCREEN_PATH_DEF);
        mConfigMap.put(SETTING_SPLASHSCREEN_DURATION, String.valueOf(SETTING_SPLASHSCREEN_DURATION_DEF));
        mConfigMap.put(ENABLE_WEB_PLUGINS, String.valueOf(ENABLE_WEB_PLUGINS_DEF));
        mConfigMap.put(ENABLE_CACHE, String.valueOf(ENABLE_CACHE_DEF));
        mConfigMap.put(USER_AGENT, USER_AGENT_DEF);
        mConfigMap.put(DISABLE_SCANNER_ON_NAVIGATION, String.valueOf(DISABLE_SCANNER_ON_NAVIGATION_DEFAULT));
        mConfigMap.put(ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE, String.valueOf(ENABLE_MEDIA_PLAYBACK_WITHOUT_GESTURE_DEF));
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
