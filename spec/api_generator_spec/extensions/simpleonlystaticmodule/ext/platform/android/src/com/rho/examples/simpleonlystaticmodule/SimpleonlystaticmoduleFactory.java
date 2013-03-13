package com.rho.examples.simpleonlystaticmodule;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class SimpleonlystaticmoduleFactory
        extends RhoApiFactory< Simpleonlystaticmodule, SimpleonlystaticmoduleSingleton>
        implements ISimpleOnlyStaticModuleFactory {

    @Override
    protected SimpleonlystaticmoduleSingleton createSingleton() {
        return new SimpleonlystaticmoduleSingleton(this);
    }

    @Override
    protected Simpleonlystaticmodule createApiObject(String id) {
        return new Simpleonlystaticmodule(id);
    }
}
