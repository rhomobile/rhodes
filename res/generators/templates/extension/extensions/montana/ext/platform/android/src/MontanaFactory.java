package com.rho.<%= namefixed %>;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class <%= namecamelcase %>Factory
        extends RhoApiFactory< <%= namecamelcase %>, <%= namecamelcase %>Singleton>
        implements I<%= namecamelcase %>Factory {

    @Override
    protected <%= namecamelcase %>Singleton createSingleton() {
        return new <%= namecamelcase %>Singleton(this);
    }

    @Override
    protected <%= namecamelcase %> createApiObject(String id) {
        return new <%= namecamelcase %>(id);
    }
}
