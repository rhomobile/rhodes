package com.rho.nativetoolbar;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class NativeToolbarFactory
        extends RhoApiFactory< NativeToolbar, NativeToolbarSingleton>
        implements INativeToolbarFactory {

    @Override
    protected NativeToolbarSingleton createSingleton() {
        return new NativeToolbarSingleton(this);
    }

    @Override
    protected NativeToolbar createApiObject(String id) {
        return new NativeToolbar(id);
    }
}
