package com.rhomobile.rhodes.extmanager;

import android.content.Context;
import android.view.View;

public interface IRhoExtManager {

    void registerExtension(String strName, IRhoExtension ext);
    IRhoExtension getExtByName(String strName);

    void addRhoListener(IRhoListener listener);

    /**
     * 
     */
    Context getContext();
    
    /**
     * Web view getter. In case of Motorola WebKit is used it returns object which may be directly casted to NeonEkihView
     * @return Current web view instance from MainView
     */
    View getWebView();
    
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

    void onUnhandledProperty(String moduleName,String name,String value, IRhoExtData ext);
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
    //void resizeBrowserWindow(RECT rc);
    void zoomPage(float fZoom);
    void zoomText(int nZoom);
    
    /**
     * Handler to notify RhoExtManager about new configuration value
     * @param name - configuration parameter name
     * @param value - configuration parameter value
     * @return true if value is processed by RhoExtManager
     */
    boolean onNewConfigValue(String name, String value);
    
    /**
     * Handler to notify RhoExtManager that new configuration is going to apply
     * @return true if it is allowed for extension to navigate start page itself
     */
    boolean onStartNewConfig();

}


