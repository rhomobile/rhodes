package com.rho.instrumentation;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class InstrumentationFactory
        extends RhoApiFactory< Instrumentation, InstrumentationSingleton>
        implements IInstrumentationFactory {

    @Override
    protected InstrumentationSingleton createSingleton() {
        return new InstrumentationSingleton(this);
    }

    @Override
    protected Instrumentation createApiObject(String id) {
        return new Instrumentation(id);
    }
}
