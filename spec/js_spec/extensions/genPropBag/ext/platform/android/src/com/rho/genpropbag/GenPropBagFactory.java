package com.rho.genpropbag;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class GenPropBagFactory
        extends RhoApiFactory< GenPropBag, GenPropBagSingleton>
        implements IGenPropBagFactory {

    @Override
    protected GenPropBagSingleton createSingleton() {
        return new GenPropBagSingleton(this);
    }

    @Override
    protected GenPropBag createApiObject(String id) {
        return new GenPropBag(id);
    }
}
