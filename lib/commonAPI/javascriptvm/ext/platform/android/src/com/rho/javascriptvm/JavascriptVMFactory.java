package com.rho.javascriptvm;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.RhoApiFactory;

public class JavascriptVMFactory
        extends RhoApiFactory< JavascriptVM, JavascriptVMSingleton>
        implements IJavascriptVMFactory {
    private static final boolean DEBUG = false;

    @Override
    protected JavascriptVMSingleton createSingleton() {
        if (DEBUG) {
            Logger.I(JavascriptVM.JS_TRACE_TAG, "createSingleton");
        }
        return new JavascriptVMSingleton(this);
    }

    @Override
    protected JavascriptVM createApiObject(String id) {
        if (DEBUG) {
            Logger.I(JavascriptVM.JS_TRACE_TAG, "createApiObject");
        }
        return new JavascriptVM(id);
    }
}
