package com.rhomobile.rhodes.api;

import java.util.HashMap;
import java.util.Map;

public abstract class RhoApiFactory <IApi extends IRhoApiObject, IApiSingleton> {
    IApiSingleton mApiSingleton;
    Map<String, IApi> mApiObjects;
    
    public RhoApiFactory() {
        mApiObjects = new HashMap<String, IApi>();
    }

    public IApiSingleton getApiSingleton() {
        if (mApiSingleton == null)
            mApiSingleton = createSingleton();

        return mApiSingleton;
    }

    public IApi getApiObject(String id) {
        IApi obj = mApiObjects.get(id);
        if (obj == null) {
            obj = createApiObject(id);
            mApiObjects.put(id, obj);
        }
        return obj;
    }

    protected void removeApiObject( String id ) {
        mApiObjects.remove( id );
    }

    protected void clearApiObjects() {
        mApiObjects.clear();
    }

    protected abstract IApiSingleton createSingleton();
    protected abstract IApi createApiObject(String id);
}
