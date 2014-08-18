package com.rho.development;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class DevelopmentFactory
        extends RhoApiFactory< Development, DevelopmentSingleton>
        implements IDevelopmentFactory {

    @Override
    protected DevelopmentSingleton createSingleton() {
        return new DevelopmentSingleton(this);
    }

    @Override
    protected Development createApiObject(String id) {
        return new Development(id);
    }
}
