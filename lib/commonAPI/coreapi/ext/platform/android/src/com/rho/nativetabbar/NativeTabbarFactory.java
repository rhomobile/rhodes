package com.rho.nativetabbar;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class NativeTabbarFactory
        extends RhoApiFactory< NativeTabbar, NativeTabbarSingleton>
        implements INativeTabbarFactory {

    @Override
    protected NativeTabbarSingleton createSingleton() {
        return new NativeTabbarSingleton(this);
    }

    @Override
    protected NativeTabbar createApiObject(String id) {
        return new NativeTabbar(id);
    }
}
