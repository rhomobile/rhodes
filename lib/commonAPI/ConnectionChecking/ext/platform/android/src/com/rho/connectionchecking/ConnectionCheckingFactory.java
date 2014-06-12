package com.rho.connectionchecking;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class ConnectionCheckingFactory
        extends RhoApiFactory< ConnectionChecking, ConnectionCheckingSingleton>
        implements IConnectionCheckingFactory {

    @Override
    protected ConnectionCheckingSingleton createSingleton() {
        return new ConnectionCheckingSingleton(this);
    }

    @Override
    protected ConnectionChecking createApiObject(String id) {
        return new ConnectionChecking(id);
    }
}
