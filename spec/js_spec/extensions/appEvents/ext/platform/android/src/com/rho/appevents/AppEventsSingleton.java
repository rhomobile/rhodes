package com.rho.appevents;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

class AppEventsSingleton extends AppEventsSingletonBase implements IAppEventsSingleton {
    public AppEventsSingleton(AppEventsFactory factory) {
        super(factory);
    }

    List<String> getIDs() {
        List<String> ids = new LinkedList<String>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }
    
    @Override
    protected String getInitialDefaultID() {
        return getIDs().get(0);
    }

    @Override
    public void enumerate(IMethodResult res) {
        res.set(getIDs());
    }
}