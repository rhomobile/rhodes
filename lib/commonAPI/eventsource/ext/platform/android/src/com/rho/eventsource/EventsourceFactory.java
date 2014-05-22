package com.rho.eventsource;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class EventsourceFactory
        extends RhoApiFactory< Eventsource, EventsourceSingleton>
        implements IEventsourceFactory {

    @Override
    protected EventsourceSingleton createSingleton() {
        return new EventsourceSingleton(this);
    }

    @Override
    protected Eventsource createApiObject(String id) {
        return new Eventsource(id);
    }
}
