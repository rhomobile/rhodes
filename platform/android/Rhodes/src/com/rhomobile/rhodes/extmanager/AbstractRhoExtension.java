package com.rhomobile.rhodes.extmanager;

import com.rhomobile.rhodes.extmanager.IRhoExtension.IAlertResult;
import com.rhomobile.rhodes.extmanager.IRhoExtension.IPromptResult;
import com.rhomobile.rhodes.extmanager.IRhoExtension.IAuthRequest;

import android.graphics.Rect;

public abstract class AbstractRhoExtension implements IRhoExtension {

    @Override
    public IRhoWebView onCreateWebView(IRhoExtManager extManager, int tabIndex) {
        return null;
    }

    @Override
    public boolean onWebViewCreated(IRhoExtManager extManager, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onSetPropertiesData(IRhoExtManager extManager, String propId,
            String data, int pos, int total, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onSetProperty(IRhoExtManager extManager, String name, String value, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onBeforeNavigate(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateStarted(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onLoadResource(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateComplete(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onDocumentComplete(IRhoExtManager extManager, String urlOfDocument, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onGoBack(IRhoExtManager extManager, String current_url, String back_url, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onGoForward(IRhoExtManager extManager, String current_url, String forward_url, IRhoWebView ext, boolean res) {
        return res;
    }


    @Override
    public boolean onAlert(IRhoExtManager extManager, String message, IRhoWebView ext, IAlertResult alertResult, boolean res) {
        return res;
    }

    @Override
    public boolean onConfirm(IRhoExtManager extManager, String message, IRhoWebView ext, IAlertResult confirmResult, boolean res) {
        return res;
    }

    @Override
    public boolean onPrompt(IRhoExtManager extManager, String message, String defaultResponse, IRhoWebView ext, IPromptResult promptResult, boolean res) {
        return res;
    }

    @Override
    public boolean onSelect(IRhoExtManager extManager, String[] items, int selected, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onStatus(IRhoExtManager extManager, String status, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onTitle(IRhoExtManager extManager, String title, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onConsole(IRhoExtManager extManager, String message, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect rect, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNavigateError(IRhoExtManager extManager, String url, LoadErrorReason reason, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public void onAppActivate(IRhoExtManager extManager, boolean bActivate) {
    }


    @Override
    public boolean startLocationUpdates(IRhoExtManager extManager, boolean highAccuracy, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean stopLocationUpdates(IRhoExtManager extManager, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onAuthRequest(IRhoExtManager extManager, IAuthRequest request, IRhoWebView ext, boolean res) {
        return res;
    }

    @Override
    public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config, String name, boolean res) {
        return res;
    }

    @Override
    public String onGetProperty(IRhoExtManager extManager, String name) {
        return null;
    }

}
