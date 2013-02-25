package com.rho.javascriptvm;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class JavascriptvmFactory
        extends RhoApiFactory< Javascriptvm, JavascriptvmSingleton>
        implements IJavascriptvmFactory {

    @Override
    protected JavascriptvmSingleton createSingleton() {
        return new JavascriptvmSingleton(this);
    }

    @Override
    protected Javascriptvm createApiObject(String id) {
        return new Javascriptvm(id);
    }
}
