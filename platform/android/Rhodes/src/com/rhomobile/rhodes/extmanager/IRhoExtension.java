package com.rhomobile.rhodes.extmanager;

import android.graphics.Rect;

public interface IRhoExtension {

    void onSetPropertiesData(IRhoExtManager extManager, String propId, String data, int pos, int total, IRhoExtData ext);
    void onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoExtData ext);
    void onSetProperty(IRhoExtManager extManager, String name, String value, IRhoExtData ext);
    void onBeforeNavigate(IRhoExtManager extManager, String url, IRhoExtData ext);
    void onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoExtData ext);
    void onNavigateComplete(IRhoExtManager extManager, String url, IRhoExtData ext);
    void onDocumentComplete(IRhoExtManager extManager, String urlOfDocument, IRhoExtData ext);
    //bool onWndMsg(MSG& oMsg){return false;}
    //long onNavigateTimeout(IRhoExtManager extManager, String urlBeingNavigatedTo, IRhoExtData ext);
    //long onSIPState(IRhoExtManager extManager, boolean bSIPState, IRhoExtData ext);
    void onAlert(IRhoExtManager extManager, String message, IRhoExtData ext);
    void onConfirm(IRhoExtManager extManager, String message, IRhoExtData ext);
    void onPrompt(IRhoExtManager extManager, String message, String defaultResponse, IRhoExtData ext);
    void onSelect(IRhoExtManager extManager, String[] items, int selected, IRhoExtData ext);
    void onStatus(IRhoExtManager extManager, String status, IRhoExtData ext);
    void onTitle(IRhoExtManager extManager, String title, IRhoExtData ext);
    void onConsole(IRhoExtManager extManager, String message, IRhoExtData ext);
    void onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect area, IRhoExtData ext);
    void onNavigateError(IRhoExtManager extManager, String url, IRhoExtData ext);

    void onAppActivate(IRhoExtManager extManager, boolean bActivate);

    //EkiohLocation getCachedLocation(IRhoExtManager extManager, IRhoExtData ext);
    void startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoExtData ext); 
    void stopLocationUpdates(IRhoExtManager extManager, IRhoExtData ext);
    void onAuthRequired(IRhoExtManager extManager, String type, String url, String realm, IRhoExtData ext); 

}

