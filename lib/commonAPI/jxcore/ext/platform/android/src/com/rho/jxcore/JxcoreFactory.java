package com.rho.jxcore;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class JxcoreFactory
        extends RhoApiFactory< Jxcore, JxcoreSingleton>
        implements IJxcoreFactory {

    @Override
    protected JxcoreSingleton createSingleton() {
        return new JxcoreSingleton(this);
    }

    @Override
    protected Jxcore createApiObject(String id) {
        return new Jxcore(id);
    }
}
