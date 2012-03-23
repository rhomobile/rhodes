package com.rhomobile.rhodes.extmanager;

import android.view.View;

public interface IRhoExtManager {

    void registerExtension(String strName, IRhoExtension ext);
    IRhoExtension getExtByName(String strName);

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

    //void rhoLog(int nSeverity,String szModule,String szMsg,String szFile, int nLine);
    String getCurrentUrl();
    void stopNavigate();
    void historyForward();
    void historyBack();
    void refreshPage(boolean bFromCache);
    void quitApp();
    void minimizeApp();
    void restoreApp();
    //void resizeBrowserWindow(RECT rc);
    void zoomPage(float fZoom);
    void zoomText(int nZoom);
    
    // RE shared runtime detect
    boolean onStartNewConfig();

}


