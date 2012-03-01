package com.rhomobile.rhodes.extmanager;

public abstract class AbstractRhoExtension implements IRhoExtension {

    @Override
    public void onSetPropertiesData(IRhoExtManager extManager, String propId,
            String data, int pos, int total, IRhoExtData ext) {
    }

    @Override
    public void onSetPropertiesDataEnd(IRhoExtManager extManager, IRhoExtData ext) {
    }

    @Override
    public void onSetProperty(IRhoExtManager extManager, String name, String value, IRhoExtData ext) {
    }

    @Override
    public void onBeforeNavigate(IRhoExtManager extManager, String url, IRhoExtData ext) {
    }

    @Override
    public void onNavigateProgress(IRhoExtManager extManager, String url, int pos, int total, IRhoExtData ext) {
    }

    @Override
    public void onNavigateComplete(IRhoExtManager extManager, String url, IRhoExtData ext) {
    }

    @Override
    public void onDocumentComplete(IRhoExtManager extManager, String urlOfDocument, IRhoExtData ext) {
    }

    @Override
    public void onAlert(IRhoExtManager extManager, String message, IRhoExtData ext) {
    }

    @Override
    public void onConfirm(IRhoExtManager extManager, String message, IRhoExtData ext) {
    }

    @Override
    public void onSelect(IRhoExtManager extManager, String[] items, int selected, IRhoExtData ext) {
    }

    @Override
    public void onStatus(IRhoExtManager extManager, String status, IRhoExtData ext) {
    }

    @Override
    public void onTitle(IRhoExtManager extManager, String title, IRhoExtData ext) {
    }

    @Override
    public void onConsole(IRhoExtManager extManager, String message, IRhoExtData ext) {
    }

    @Override
    public void onInputMethod(IRhoExtManager extManager, boolean enabled, IRhoExtData ext) {
    }

    @Override
    public void onNavigateError(IRhoExtManager extManager, String url, IRhoExtData ext) {
    }

    @Override
    public void onAppActivate(IRhoExtManager extManager, boolean bActivate) {
    }

}
