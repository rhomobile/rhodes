package com.rhomobile.rhodes.extmanager;

import android.view.View;

public interface IRhoExtManager {

    void registerExtension(String strName, IRhoExtension ext);
//
//    IRhoExtension getExtByName(String strName);

    View getWebView();
    View getTopView();

    void onBeforeNavigate(String url);

    void onUnhandledProperty(String pModuleName,String pName,String pValue, IRhoExtData ext);
    void executeRubyCallback(String szCallback,String szCallbackBody,String szCallbackData, boolean waitForResponse);
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

}


