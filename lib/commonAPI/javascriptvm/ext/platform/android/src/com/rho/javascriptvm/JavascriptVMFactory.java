package com.rho.javascriptvm;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class JavascriptVMFactory
        extends RhoApiFactory< JavascriptVM, JavascriptVMSingleton>
        implements IJavascriptVMFactory {

    @Override
    protected JavascriptVMSingleton createSingleton() {
        return new JavascriptVMSingleton(this);
    }

    @Override
    protected JavascriptVM createApiObject(String id) {
        return new JavascriptVM(id);
    }
}
