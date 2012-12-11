package com.rhomobile.rhodes.extmanager;

import android.graphics.Rect;

public abstract class AbstractRhoExtension implements IRhoExtension {

    @Override
    public IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex) {
        return null;
    }

    @Override
    public void onSetPropertiesData(IRhoExtManager extManager, String propId,
            String data, int pos, int total, IRhoWebView ext) {
    }

    @Override
    public void onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoWebView ext) {
    }

    @Override
    public void onSetProperty(IRhoExtManager extManager, String name, String value, IRhoWebView ext) {
    }

    @Override
    public void onBeforeNavigate(IRhoExtManager extManager, String url, IRhoWebView ext) {
    }

    @Override
    public void onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoWebView ext) {
    }

    @Override
    public void onNavigateComplete(IRhoExtManager extManager, String url, IRhoWebView ext) {
    }

    @Override
    public void onDocumentComplete(IRhoExtManager extManager, String urlOfDocument, IRhoWebView ext) {
    }

    @Override
    public void onAlert(IRhoExtManager extManager, String message, IRhoWebView ext) {
    }

    @Override
    public void onConfirm(IRhoExtManager extManager, String message, IRhoWebView ext) {
    }

    @Override
    public void onPrompt(IRhoExtManager extManager, String message, String defaultResponse, IRhoWebView ext) {
    }

    @Override
    public void onSelect(IRhoExtManager extManager, String[] items, int selected, IRhoWebView ext) {
    }

    @Override
    public void onStatus(IRhoExtManager extManager, String status, IRhoWebView ext) {
    }

    @Override
    public void onTitle(IRhoExtManager extManager, String title, IRhoWebView ext) {
    }

    @Override
    public void onConsole(IRhoExtManager extManager, String message, IRhoWebView ext) {
    }

    @Override
    public void onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect rect, IRhoWebView ext) {
    }

    @Override
    public void onNavigateError(IRhoExtManager extManager, String url, LoadErrorReason reason, IRhoWebView ext) {
    }

    @Override
    public void onAppActivate(IRhoExtManager extManager, boolean bActivate) {
    }

    //@Override
    //EkiohLocation getCachedLocation(IRhoExtManager extManager, IRhoExtData ext);

    @Override
    public void startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoWebView ext) {
    }

    @Override
    public void stopLocationUpdates(IRhoExtManager extManager, IRhoWebView ext) {
    }

    @Override
    public void onAuthRequired(IRhoExtManager extManager, String type, String url, String realm, IRhoWebView ext) {
    }

}
