package com.rho.appevents;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class AppEventsFactory
        extends RhoApiFactory< AppEvents, AppEventsSingleton>
        implements IAppEventsFactory {

    @Override
    protected AppEventsSingleton createSingleton() {
        return new AppEventsSingleton(this);
    }

    @Override
    protected AppEvents createApiObject(String id) {
        return new AppEvents(id);
    }
}
