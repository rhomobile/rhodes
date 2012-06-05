package com.rhomobile.rhodes.extmanager;

import android.content.Context;
import android.graphics.Rect;
import android.view.inputmethod.InputMethodManager;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.ContextFactory;

public class WebkitExtension extends WebkitExtensionBase {
    static final String TAG = WebkitExtension.class.getSimpleName();

    public WebkitExtension(Config config) {
        super(config);
    }
    
    @Override
    public void onInputMethod(IRhoExtManager extManager, boolean enabled, String type, Rect rect, IRhoExtData ext) {
        InputMethodManager imm = (InputMethodManager) ContextFactory.getUiContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (enabled) {
            imm.showSoftInput(ext.getWebView(), 0);
        } else {
            imm.hideSoftInputFromWindow(ext.getWebView().getApplicationWindowToken(), 0);
        }
    }

}
