package com.rho.javascriptvm;

import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

public class JavascriptVM extends JavascriptVMBase implements IJavascriptVM {
    private static final boolean DEBUG = false;

    public static final String JS_TRACE_TAG = "JS VM";

    public JavascriptVM(String id) {
        super(id);
        if (DEBUG) {
            Logger.I(JS_TRACE_TAG, "JavascriptVMSingleton");
        }
    }

}