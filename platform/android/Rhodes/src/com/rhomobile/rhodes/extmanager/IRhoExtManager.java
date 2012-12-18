package com.rhomobile.rhodes.extmanager;

import android.content.Context;
import android.view.View;

public interface IRhoExtManager {

    enum LicenseStatus {LICENSE_MISSED, LICENSE_FAILED, LICENSE_PASSED};

    void registerExtension(String strName, IRhoExtension ext);
    IRhoExtension getExtByName(String strName);

    void addRhoListener(IRhoListener listener);

    /**
     * Handler to notify RhoExtManager about new configuration
     * @param config - IRhoConfig implementation
     * @param name - configuration name used to identify the config like 'rhoelements' od 'rhoconfig'
     */
    void setConfig(String name, IRhoConfig config);
    IRhoConfig getConfig(String name);

    Context getContext();

    IRhoWebView getWebView();

    /**
     * RhodesActivity top level View getter
     * @return RhodesActivity top level View
     */
    View getTopView();

    /**
     * 
     * @param name - build configuration item name
     * @return build configuration item value
     */
    String getBuildConfigItem(String name);

    void onUnhandledProperty(String moduleName,String name,String value, IRhoWebView ext);
    void executeRubyCallback(String callback,String callbackBody,String callbackData, boolean waitForResponse);
    void executeRubyCallbackWithJsonBody(String szCallback, String callbackBody, String callbackData, boolean waitForResponse);
    void requireRubyFile(String szFilePath);

    void navigate(String url);
    //boolean existsJavascript(String jsFunction);
    void executeJavascript(String jsFunction);

    String getCurrentUrl();
    void stopNavigate(IRhoExtension.LoadErrorReason reason);
    void historyForward();
    void historyBack();
    void refreshPage(boolean bFromCache);
    void quitApp();
    void setFullScreen(boolean fullScreen);
    void minimizeApp();
    void restoreApp();
    void zoomPage(float fZoom);
    void zoomText(int nZoom);

    /**
     * Handler to notify RhoExtManager that new configuration is going to apply
     * @return true if it is allowed for extension to navigate start page itself
     */
    boolean onStartNewConfig();
    
    void setLicenseCredentials(String token, String company, String appName);
    LicenseStatus getLicenseStatus();

}


