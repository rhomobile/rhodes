package com.rhomobile.rhodes.extmanager;

public class WebkitExtensionBase extends AbstractRhoExtension implements IRhoExtension{

    public static final String EXTNAME = "MotorolaWebkit";
    static final String TAG = WebkitExtensionBase.class.getSimpleName();
    private static WebkitExtension mInstance;
    private Config mConfig;

    public WebkitExtensionBase(Config config) {
        mConfig = config;
    }

    public static WebkitExtension getInstance() {
        return mInstance;
    }

    public Config getConfig() {
        return mConfig;
    }

}