package com.rhomobile.rhodes.extmanager;

public interface IRhoExtension {

    void onSetPropertiesData(String propId, String data, IRhoExtData ext);
    void onSetProperty(String name, String value, IRhoExtData ext);
    void onBeforeNavigate(String urlBeingNavigatedTo, IRhoExtData ext);
    void onNavigateComplete(String urlBeingNavigatedTo, IRhoExtData ext);
    void onDocumentComplete(String urlOfDocument, IRhoExtData ext);
    //bool onWndMsg(MSG& oMsg){return false;}
    long onNavigateTimeout(String urlBeingNavigatedTo, IRhoExtData ext);
    long onSIPState(boolean bSIPState, IRhoExtData ext);
    //long onAlertPopup(int nEnum, void pData, IRhoExtData ext);
    long onNavigateError(String urlBeingNavigatedTo, IRhoExtData ext);
    void onAppActivate(boolean bActivate, IRhoExtData ext);

}

