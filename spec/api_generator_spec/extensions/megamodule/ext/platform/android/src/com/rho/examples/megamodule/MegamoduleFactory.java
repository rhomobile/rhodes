package com.rho.examples.megamodule;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class MegamoduleFactory
        extends RhoApiFactory< Megamodule, MegamoduleSingleton>
        implements IMegamoduleFactory {

    @Override
    protected MegamoduleSingleton createSingleton() {
        return new MegamoduleSingleton(this);
    }

    @Override
    protected Megamodule createApiObject(String id) {
        return new Megamodule(id);
    }
}
