package com.rhomobile.rhodes.extmanager;

import android.graphics.Rect;

public interface IRhoExtension {
    
    public enum LoadErrorReason { STOP, INTERNAL_ERROR, BAD_LICENSE }

    IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex);
    void onSetPropertiesData(IRhoExtManager extManager, String propId, String data, int pos, int total, IRhoWebView ext);
    void onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoWebView ext);
    void onSetProperty(IRhoExtManager extManager, String name, String value, IRhoWebView ext);
    void onBeforeNavigate(IRhoExtManager extManager, String url, IRhoWebView ext);
    void onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoWebView ext);
    void onNavigateComplete(IRhoExtManager extManager, String url, IRhoWebView ext);
    void onDocumentComplete(IRhoExtManager extManager, String urlOfDocument, IRhoWebView ext);
    //bool onWndMsg(MSG& oMsg){return false;}
    //long onNavigateTimeout(IRhoExtManager extManager, String urlBeingNavigatedTo, IRhoExtData ext);
    //long onSIPState(IRhoExtManager extManager, boolean bSIPState, IRhoExtData ext);
    void onAlert(IRhoExtManager extManager, String message, IRhoWebView ext);
    void onConfirm(IRhoExtManager extManager, String message, IRhoWebView ext);
    void onPrompt(IRhoExtManager extManager, String message, String defaultResponse, IRhoWebView ext);
    void onSelect(IRhoExtManager extManager, String[] items, int selected, IRhoWebView ext);
    void onStatus(IRhoExtManager extManager, String status, IRhoWebView ext);
    void onTitle(IRhoExtManager extManager, String title, IRhoWebView ext);
    void onConsole(IRhoExtManager extManager, String message, IRhoWebView ext);
    void onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect area, IRhoWebView ext);
    void onNavigateError(IRhoExtManager extManager, String url, LoadErrorReason reason, IRhoWebView ext);

    void onAppActivate(IRhoExtManager extManager, boolean bActivate);

    //EkiohLocation getCachedLocation(IRhoExtManager extManager, IRhoExtData ext);
    void startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoWebView ext); 
    void stopLocationUpdates(IRhoExtManager extManager, IRhoWebView ext);
    void onAuthRequired(IRhoExtManager extManager, String type, String url, String realm, IRhoWebView ext); 

}

